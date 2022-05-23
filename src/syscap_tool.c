/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <securec.h>
#include "endian_internal.h"
#include "cJSON.h"
#include "syscap_tool.h"

typedef struct ProductCompatibilityIDHead {
    uint16_t apiVersion : 15;
    uint16_t apiVersionType : 1;
    uint16_t systemType : 3;
    uint16_t reserved : 13;
    uint32_t manufacturerID;
} PCIDHead;

typedef struct RequiredProductCompatibilityIDHead {
    uint16_t apiVersion : 15;
    uint16_t apiVersionType : 1;
} RPCIDHead;

#define SINGLE_FEAT_LENGTH  (32 * 8)

#define PRINT_ERR(...) \
    do { \
        printf("ERROR: in file %s at line %d -> ", __FILE__, __LINE__); \
        printf(__VA_ARGS__); \
    } while (0)

static void FreeContextBuffer(char *contextBuffer)
{
    (void)free(contextBuffer);
}

static int32_t GetFileContext(char *inputFile, char **contextBufPtr, uint32_t *bufferLen)
{
    int32_t ret;
    FILE *fp = NULL;
    struct stat statBuf;
    char *contextBuffer = NULL;
    char path[PATH_MAX + 1] = {0x00};

#ifdef _POSIX_
    if (strlen(inputFile) > PATH_MAX || strncpy_s(path, PATH_MAX, inputFile, strlen(inputFile)) != EOK) {
        PRINT_ERR("get path(%s) failed\n", inputFile);
        return -1;
    }
#else
    if (strlen(inputFile) > PATH_MAX || realpath(inputFile, path) == NULL) {
        PRINT_ERR("get file(%s) real path failed\n", inputFile);
        return -1;
    }
#endif

    ret = stat(path, &statBuf);
    if (ret != 0) {
        PRINT_ERR("get file(%s) st_mode failed, errno = %d\n", path, errno);
        return -1;
    }
    if (!(statBuf.st_mode & S_IRUSR)) {
        PRINT_ERR("don't have permission to read the file(%s)\n", path);
        return -1;
    }
    contextBuffer = (char *)malloc(statBuf.st_size + 1);
    if (contextBuffer == NULL) {
        PRINT_ERR("malloc buffer failed, size = %d, errno = %d\n", (int32_t)statBuf.st_size + 1, errno);
        return -1;
    }
    fp = fopen(path, "rb");
    if (fp == NULL) {
        PRINT_ERR("open file(%s) failed, errno = %d\n", path, errno);
        FreeContextBuffer(contextBuffer);
        return -1;
    }
    size_t retFread = fread(contextBuffer, statBuf.st_size, 1, fp);
    if (retFread != 1) {
        PRINT_ERR("read file(%s) failed, errno = %d\n", path, errno);
        FreeContextBuffer(contextBuffer);
        (void)fclose(fp);
        return -1;
    }
    contextBuffer[statBuf.st_size] = '\0';
    (void)fclose(fp);

    *contextBufPtr = contextBuffer;
    *bufferLen = statBuf.st_size + 1;
    return 0;
}

static int32_t ConvertedContextSaveAsFile(char *outDirPath, char *filename, char *convertedBuffer, uint32_t bufferLen)
{
    int32_t ret;
    FILE *fp = NULL;
    char path[PATH_MAX + 1] = {0x00};
    int32_t pathLen = strlen(outDirPath);

#ifdef _POSIX_
    if (strlen(outDirPath) > PATH_MAX || strncpy_s(path, PATH_MAX, outDirPath, strlen(outDirPath)) != EOK) {
        PRINT_ERR("get path(%s) failed\n", outDirPath);
        return -1;
    }
#else
    if (strlen(outDirPath) > PATH_MAX || realpath(outDirPath, path) == NULL) {
        PRINT_ERR("get file(%s) real path failed\n", outDirPath);
        return -1;
    }
#endif

    if (path[pathLen - 1] != '/' && path[pathLen - 1] != '\\') {
        path[pathLen] = '/';
    }

    if (strlen(filename) + 1 > PATH_MAX) {
        PRINT_ERR("filename(%s) too long.\n", filename);
        return -1;
    }
    ret = strncat_s(path, PATH_MAX, filename, strlen(filename));
    if (ret != 0) {
        PRINT_ERR("strncat_s failed, (%s, %d, %s, %d), errno = %d\n",
                  path, PATH_MAX, filename, (int32_t)strlen(filename) + 1, errno);
        return -1;
    }

    fp = fopen(path, "wb");
    if (fp == NULL) {
        PRINT_ERR("can`t create file(%s), errno = %d\n", path, errno);
        return -1;
    }

    size_t retFwrite = fwrite(convertedBuffer, bufferLen, 1, fp);
    if (retFwrite != 1) {
        PRINT_ERR("can`t write file(%s),errno = %d\n", path, errno);
        (void)fclose(fp);
        return -1;
    }

    (void)fclose(fp);

    return 0;
}

int32_t PCIDEncode(char *inputFile, char *outDirPath)
{
    int32_t ret;
    char productName[NAME_MAX] = {0};
    char *contextBuffer = NULL;
    uint32_t bufferLen, osCapSize, privateCapSize;
    char *convertedBuffer = NULL;
    uint32_t convertedBufLen = sizeof(PCIDHead);
    char *systemType = NULL;
    PCIDHead *headPtr = NULL;
    char *fillTmpPtr = NULL;
    cJSON *cjsonObjectRoot = NULL;
    cJSON *cjsonObjectPtr = NULL;
    cJSON *osCapPtr = NULL;
    cJSON *privateCapPtr = NULL;
    cJSON *arrayItemPtr = NULL;

    ret = GetFileContext(inputFile, &contextBuffer, &bufferLen);
    if (ret != 0) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", inputFile);
        return -1;
    }

    cjsonObjectRoot = cJSON_ParseWithLength(contextBuffer, bufferLen);
    if (cjsonObjectRoot == NULL) {
        PRINT_ERR("cJSON_Parse failed, context buffer is:\n%s\n", contextBuffer);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    cjsonObjectPtr = cJSON_GetObjectItem(cjsonObjectRoot, "syscap");
    if (cjsonObjectPtr == NULL || !cJSON_IsObject(cjsonObjectPtr)) {
        PRINT_ERR("get \"syscap\" object failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    osCapPtr = cJSON_GetObjectItem(cjsonObjectPtr, "os");
    if (osCapPtr == NULL || !cJSON_IsArray(osCapPtr)) {
        PRINT_ERR("get \"os\" array failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    ret = cJSON_GetArraySize(osCapPtr);
    if (ret < 0) {
        PRINT_ERR("get \"os\" array size failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    osCapSize = (uint32_t)ret;
    // 2, to save osSysCaptype & osSysCapLength
    convertedBufLen += (2 * sizeof(uint16_t) + osCapSize * SINGLE_FEAT_LENGTH);

    privateCapPtr = cJSON_GetObjectItem(cjsonObjectPtr, "private");
    if (privateCapPtr != NULL && cJSON_IsArray(privateCapPtr)) {
        privateCapSize = (uint32_t)cJSON_GetArraySize(privateCapPtr);
        // 2, to save privateSysCaptype & privateSysCapLength
        convertedBufLen += (2 * sizeof(uint16_t) * !!privateCapSize +
            privateCapSize * SINGLE_FEAT_LENGTH);
    } else if (privateCapPtr == NULL) {
        privateCapSize = 0;
    } else {
        PRINT_ERR("get \"private\" array failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    convertedBuffer = (char *)malloc(convertedBufLen);
    if (convertedBuffer == NULL) {
        PRINT_ERR("malloc failed.\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    (void)memset_s(convertedBuffer, convertedBufLen, 0, convertedBufLen);

    headPtr = (PCIDHead *)convertedBuffer;
    cjsonObjectPtr = cJSON_GetObjectItem(cjsonObjectRoot, "api_version");
    if (cjsonObjectPtr == NULL || !cJSON_IsNumber(cjsonObjectPtr)) {
        PRINT_ERR("get \"api_version\" failed\n");
        ret = -1;
        goto FREE_CONVERT_OUT;
    }
    headPtr->apiVersion = HtonsInter((uint16_t)cjsonObjectPtr->valueint);
    headPtr->apiVersionType = 0;

    cjsonObjectPtr = cJSON_GetObjectItem(cjsonObjectRoot, "system_type");
    if (cjsonObjectPtr == NULL || !cJSON_IsString(cjsonObjectPtr)) {
        PRINT_ERR("get \"system_type\" failed.\n");
        ret = -1;
        goto FREE_CONVERT_OUT;
    }
    systemType = cjsonObjectPtr->valuestring;
    headPtr->systemType = !strcmp(systemType, "mini") ? 0b001 :
                          (!strcmp(systemType, "small") ? 0b010 :
                          (!strcmp(systemType, "standard") ? 0b100 : 0));
    if (headPtr->systemType == 0) {
        PRINT_ERR("\"system_type\" is invalid, systemType = \"%s\"\n", systemType);
        ret = -1;
        goto FREE_CONVERT_OUT;
    }

    cjsonObjectPtr = cJSON_GetObjectItem(cjsonObjectRoot, "manufacturer_id");
    if (cjsonObjectPtr == NULL || !cJSON_IsNumber(cjsonObjectPtr)) {
        PRINT_ERR("get \"manufacturer_id\" failed\n");
        ret = -1;
        goto FREE_CONVERT_OUT;
    }
    headPtr->manufacturerID = HtonlInter((uint32_t)cjsonObjectPtr->valueint);

    fillTmpPtr = convertedBuffer + sizeof(PCIDHead);

    *(uint16_t *)fillTmpPtr = HtonsInter(0); // 0, SysCap Type, 0: osCap
    fillTmpPtr += sizeof(uint16_t);
    // fill osCap Length
    *(uint16_t *)fillTmpPtr = HtonsInter((uint16_t)(osCapSize * SINGLE_FEAT_LENGTH));
    fillTmpPtr += sizeof(uint16_t);
    for (uint32_t i = 0; i < osCapSize; i++) {
        arrayItemPtr = cJSON_GetArrayItem(osCapPtr, i);
        char *pointPos = strchr(arrayItemPtr->valuestring, '.');
        if (pointPos == NULL) {
            PRINT_ERR("context of \"os\" array is invalid\n");
            ret = -1;
            goto FREE_CONVERT_OUT;
        }
        ret = strncmp(arrayItemPtr->valuestring, "SystemCapability.", pointPos - arrayItemPtr->valuestring + 1);
        if (ret != 0) {
            PRINT_ERR("context of \"os\" array is invalid\n");
            ret = -1;
            goto FREE_CONVERT_OUT;
        }

        ret = memcpy_s(fillTmpPtr, SINGLE_FEAT_LENGTH, pointPos + 1, strlen(pointPos + 1));
        if (ret != 0) {
            PRINT_ERR("context of \"os\" array is invalid\n");
            ret = -1;
            goto FREE_CONVERT_OUT;
        }
        fillTmpPtr += SINGLE_FEAT_LENGTH;
    }

    if (privateCapSize != 0) {
        *(uint16_t *)fillTmpPtr = HtonsInter(1); // 1, SysCap Type, 1: privateCap
        fillTmpPtr += sizeof(uint16_t);
        // fill privateCap Length
        *(uint16_t *)fillTmpPtr = HtonsInter((uint16_t)(privateCapSize * SINGLE_FEAT_LENGTH));
        fillTmpPtr += sizeof(uint16_t);
        for (uint32_t i = 0; i < privateCapSize; i++) {
            arrayItemPtr = cJSON_GetArrayItem(privateCapPtr, i);
            char *pointPos = strchr(arrayItemPtr->valuestring, '.');
            if (pointPos == NULL) {
                PRINT_ERR("context of \"private\" array is invalid\n");
                ret = -1;
                goto FREE_CONVERT_OUT;
            }
            ret = strncmp(arrayItemPtr->valuestring, "SystemCapability.", pointPos - arrayItemPtr->valuestring + 1);
            if (ret != 0) {
                PRINT_ERR("context of \"private\" array is invalid\n");
                ret = -1;
                goto FREE_CONVERT_OUT;
            }

            ret = memcpy_s(fillTmpPtr, SINGLE_FEAT_LENGTH, pointPos + 1, strlen(pointPos + 1));
            if (ret != 0) {
                PRINT_ERR("context of \"private\" array is invalid\n");
                ret = -1;
                goto FREE_CONVERT_OUT;
            }
            fillTmpPtr += SINGLE_FEAT_LENGTH;
        }
    }

    cjsonObjectPtr = cJSON_GetObjectItem(cjsonObjectRoot, "product");
    if (cjsonObjectPtr == NULL || !cJSON_IsString(cjsonObjectPtr)) {
        PRINT_ERR("get \"product\" failed\n");
        ret = -1;
        goto FREE_CONVERT_OUT;
    }

    ret = strncpy_s(productName, NAME_MAX, cjsonObjectPtr->valuestring, strlen(cjsonObjectPtr->valuestring));
    if (ret != 0) {
        PRINT_ERR("strncpy_s failed, source string:%s, len = %d, errno = %d\n",
                  cjsonObjectPtr->valuestring, (int32_t)strlen(cjsonObjectPtr->valuestring), errno);
        ret = -1;
        goto FREE_CONVERT_OUT;
    }

    ret = strncat_s(productName, NAME_MAX, ".sc", 4); // 4. '.' 's' 'c' '\0'
    if (ret != 0) {
        PRINT_ERR("strncat_s failed, (%s, %d, \".sc\", 4), errno = %d\n", productName, NAME_MAX, errno);
        ret = -1;
        goto FREE_CONVERT_OUT;
    }

    productName[NAME_MAX - 1] = '\0';
    ret = ConvertedContextSaveAsFile(outDirPath, productName, convertedBuffer, convertedBufLen);
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outDirPath:%s, filename:%s\n", outDirPath, productName);
        ret = -1;
        goto FREE_CONVERT_OUT;
    }
    ret = 0;

FREE_CONVERT_OUT:
    free(convertedBuffer);
FREE_CONTEXT_OUT:
    FreeContextBuffer(contextBuffer);
    return ret;
}

int32_t PCIDDecode(char *inputFile, char *outDirPath)
{
    int32_t ret;
    char *contextBuffer = NULL;
    char *contextBufferTail = NULL;
    uint32_t bufferLen;
    char *convertedBuffer = NULL;
    uint16_t sysCaptype, sysCapLength;
    PCIDHead *headPtr = NULL;
    char *osCapArrayPtr = NULL;
    char *privateCapArrayPtr = NULL;
    cJSON *cjsonObjectRoot = NULL;
    cJSON *sysCapObjectPtr = NULL;
    cJSON *capVectorPtr = NULL;
    char productName[NAME_MAX] = {0};
    char *inputFileName = basename(inputFile);
    uint16_t inputFileNameLen = strlen(inputFileName);
    char *pointPos = strchr(inputFileName, '.');
    uint16_t productNameLen = pointPos ? (pointPos - inputFileName) : inputFileNameLen;

    ret = strncpy_s(productName, NAME_MAX, inputFileName, productNameLen);
    if (ret != 0) {
        PRINT_ERR("strncpy_s failed, source string:%s, len = %d\n", inputFileName, productNameLen);
        return -1;
    }
    productName[NAME_MAX - 1] = '\0';

    ret = GetFileContext(inputFile, &contextBuffer, &bufferLen);
    if (ret != 0) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", inputFile);
        return -1;
    }

    contextBufferTail = contextBuffer + bufferLen;
    // 2, to save osSysCaptype & osSysCapLength
    osCapArrayPtr = contextBuffer + sizeof(PCIDHead) + 2 * sizeof(uint16_t);
    if (contextBufferTail <= osCapArrayPtr) {
        PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    headPtr = (PCIDHead *)contextBuffer;
    if (headPtr->apiVersionType != 0) {
        PRINT_ERR("prase file failed, apiVersionType is invalid, input file : %s\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    char *systemType = headPtr->systemType == 0b001 ? "mini" :
                       (headPtr->systemType == 0b010 ? "small" :
                       (headPtr->systemType == 0b100 ? "standard" : NULL));
    if (systemType == NULL) {
        PRINT_ERR("prase file failed, systemType is invalid, %d\n", headPtr->systemType);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    sysCaptype = NtohsInter(*(uint16_t *)(osCapArrayPtr - 2 * sizeof(uint16_t))); // 2, for type & length
    if (sysCaptype != 0) {
        PRINT_ERR("prase file failed, sysCaptype is invalid, %d\n", sysCaptype);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    sysCapLength = NtohsInter(*(uint16_t *)(osCapArrayPtr - sizeof(uint16_t)));
    if (contextBufferTail < osCapArrayPtr + sysCapLength) {
        PRINT_ERR("prase file(%s) failed\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    sysCapObjectPtr = cJSON_CreateObject();
    if (sysCapObjectPtr == NULL) {
        PRINT_ERR("cJSON_CreateObject failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    capVectorPtr = cJSON_CreateArray();
    if (capVectorPtr == NULL) {
        PRINT_ERR("cJSON_CreateArray failed\n");
        ret = -1;
        goto FREE_SYSCAP_OUT;
    }
    for (uint32_t i = 0; i < (sysCapLength / SINGLE_FEAT_LENGTH); i++) {
        if (*(osCapArrayPtr + (i + 1) * SINGLE_FEAT_LENGTH - 1) != '\0') {
            PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
            ret = -1;
            goto FREE_VECTOR_OUT;
        }
        char buffer[SINGLE_FEAT_LENGTH + 17] = "SystemCapability."; // 17, sizeof "SystemCapability."

        ret = strncat_s(buffer, sizeof(buffer), osCapArrayPtr + i * SINGLE_FEAT_LENGTH, SINGLE_FEAT_LENGTH);
        if (ret != 0) {
            PRINT_ERR("strncat_s failed, (%s, %d, %s, %d)\n",
                      buffer, (int32_t)sizeof(buffer), osCapArrayPtr + i * SINGLE_FEAT_LENGTH, SINGLE_FEAT_LENGTH);
            ret = -1;
            goto FREE_CONVERT_OUT;
        }
        if (!cJSON_AddItemToArray(capVectorPtr, cJSON_CreateString(buffer))) {
            PRINT_ERR("cJSON_AddItemToArray or cJSON_CreateString failed\n");
            ret = -1;
            goto FREE_VECTOR_OUT;
        }
    }
    if (!cJSON_AddItemToObject(sysCapObjectPtr, "os", capVectorPtr)) {
        PRINT_ERR("cJSON_AddItemToObject failed\n");
        ret = -1;
        goto FREE_VECTOR_OUT;
    }
    capVectorPtr = NULL;
    privateCapArrayPtr = osCapArrayPtr + sysCapLength + 2 * sizeof(uint16_t); // 2, for type & length
    if (contextBufferTail >= privateCapArrayPtr) {
        sysCaptype = NtohsInter(*(uint16_t *)(privateCapArrayPtr - 2 * sizeof(uint16_t))); // 2, for type & length
        if (sysCaptype != 1) {
            PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
            ret = -1;
            goto FREE_SYSCAP_OUT;
        }
        sysCapLength = NtohsInter(*(uint16_t *)(privateCapArrayPtr - sizeof(uint16_t)));
        if (contextBufferTail < privateCapArrayPtr + sysCapLength) {
            PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
            ret = -1;
            goto FREE_SYSCAP_OUT;
        }

        capVectorPtr = cJSON_CreateArray();
        if (capVectorPtr == NULL) {
            PRINT_ERR("cJSON_CreateArray failed\n");
            ret = -1;
            goto FREE_SYSCAP_OUT;
        }

        for (uint32_t i = 0; i < (sysCapLength / SINGLE_FEAT_LENGTH); i++) {
            if (*(privateCapArrayPtr + (i + 1) * SINGLE_FEAT_LENGTH - 1) != '\0') {
                PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
                ret = -1;
                goto FREE_VECTOR_OUT;
            }

            char buffer[SINGLE_FEAT_LENGTH + 17] = "SystemCapability."; // 17, sizeof "SystemCapability."

            ret = strncat_s(buffer, sizeof(buffer), privateCapArrayPtr + i * SINGLE_FEAT_LENGTH, SINGLE_FEAT_LENGTH);
            if (ret != 0) {
                PRINT_ERR("strncat_s failed, (%s, %d, %s, %d)\n", buffer,
                          (int32_t)sizeof(buffer), privateCapArrayPtr + i * SINGLE_FEAT_LENGTH, SINGLE_FEAT_LENGTH);
                ret = -1;
                goto FREE_CONVERT_OUT;
            }

            if (!cJSON_AddItemToArray(capVectorPtr, cJSON_CreateString(buffer))) {
                PRINT_ERR("cJSON_AddItemToArray or cJSON_CreateString failed\n");
                ret = -1;
                goto FREE_VECTOR_OUT;
            }
        }
        if (!cJSON_AddItemToObject(sysCapObjectPtr, "private", capVectorPtr)) {
            PRINT_ERR("cJSON_AddItemToObject failed\n");
            ret = -1;
            goto FREE_VECTOR_OUT;
        }
        capVectorPtr = NULL;
    }

    cjsonObjectRoot = cJSON_CreateObject();
    if (cjsonObjectRoot == NULL) {
        PRINT_ERR("cJSON_CreateObject failed\n");
        ret = -1;
        goto FREE_SYSCAP_OUT;
    }
    if (!cJSON_AddStringToObject(cjsonObjectRoot, "product", productName)) {
        PRINT_ERR("cJSON_AddStringToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    if (!cJSON_AddNumberToObject(cjsonObjectRoot, "api_version", NtohsInter(headPtr->apiVersion))) {
        PRINT_ERR("cJSON_AddNumberToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    if (!cJSON_AddNumberToObject(cjsonObjectRoot, "manufacturer_id", NtohlInter(headPtr->manufacturerID))) {
        PRINT_ERR("cJSON_AddNumberToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    if (!cJSON_AddStringToObject(cjsonObjectRoot, "system_type", systemType)) {
        PRINT_ERR("cJSON_AddStringToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    if (!cJSON_AddItemToObject(cjsonObjectRoot, "syscap", sysCapObjectPtr)) {
        PRINT_ERR("cJSON_AddItemToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    sysCapObjectPtr = NULL;
    convertedBuffer = cJSON_Print(cjsonObjectRoot);

    ret = strncat_s(productName, NAME_MAX, ".json", 6); // 6. '.' 'j' 's' 'o' 'n' '\0'
    if (ret != 0) {
        PRINT_ERR("strncat_s failed, (%s, %d, .json, 6), errno = %d\n", productName, NAME_MAX, errno);
        goto FREE_CONVERT_OUT;
    }

    productName[NAME_MAX - 1] = '\0';
    ret = ConvertedContextSaveAsFile(outDirPath, productName, convertedBuffer, strlen(convertedBuffer));
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outDirPath:%s, filename:%s\n", outDirPath, productName);
        goto FREE_CONVERT_OUT;
    }

FREE_CONVERT_OUT:
    free(convertedBuffer);
FREE_ROOT_OUT:
    cJSON_Delete(cjsonObjectRoot);
FREE_VECTOR_OUT:
    cJSON_Delete(capVectorPtr);
FREE_SYSCAP_OUT:
    cJSON_Delete(sysCapObjectPtr);
FREE_CONTEXT_OUT:
    FreeContextBuffer(contextBuffer);
    return ret;
}

int32_t RPCIDEncode(char *inputFile, char *outDirPath)
{
    int32_t ret;
    char *contextBuffer = NULL;
    uint32_t bufferLen, sysCapSize;
    char *convertedBuffer = NULL;
    uint32_t convertedBufLen = sizeof(RPCIDHead);
    RPCIDHead *headPtr = NULL;
    char *fillTmpPtr = NULL;
    cJSON *cjsonObjectRoot = NULL;
    cJSON *apiVerItem = NULL;
    cJSON *sysCapPtr = NULL;
    cJSON *arrayItemPtr = NULL;

    ret = GetFileContext(inputFile, &contextBuffer, &bufferLen);
    if (ret != 0) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", inputFile);
        return ret;
    }

    cjsonObjectRoot = cJSON_ParseWithLength(contextBuffer, bufferLen);
    if (cjsonObjectRoot == NULL) {
        PRINT_ERR("cJSON_Parse failed, context buffer is:\n%s\n", contextBuffer);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    sysCapPtr = cJSON_GetObjectItem(cjsonObjectRoot, "syscap");
    if (sysCapPtr == NULL || !cJSON_IsArray(sysCapPtr)) {
        PRINT_ERR("get \"syscap\" object failed.\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    ret = cJSON_GetArraySize(sysCapPtr);
    if (ret < 0) {
        PRINT_ERR("get \"syscap\" array size failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    sysCapSize = (uint32_t)ret;
    // 2, to save SysCaptype & SysCapLength
    convertedBufLen += (2 * sizeof(uint16_t) + sysCapSize * SINGLE_FEAT_LENGTH);

    convertedBuffer = (char *)malloc(convertedBufLen);
    if (convertedBuffer == NULL) {
        PRINT_ERR("malloc failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    (void)memset_s(convertedBuffer, convertedBufLen, 0, convertedBufLen);

    headPtr = (RPCIDHead *)convertedBuffer;
    apiVerItem = cJSON_GetObjectItem(cjsonObjectRoot, "api_version");
    if (apiVerItem == NULL || !cJSON_IsNumber(apiVerItem)) {
        PRINT_ERR("get \"api_version\" failed\n");
        ret = -1;
        goto FREE_CONVERT_OUT;
    }
    headPtr->apiVersion = HtonsInter((uint16_t)apiVerItem->valueint);
    headPtr->apiVersionType = 1;

    fillTmpPtr = convertedBuffer + sizeof(RPCIDHead);

    *(uint16_t *)fillTmpPtr = HtonsInter(2); // 2, SysCap Type, 2: request Cap
    fillTmpPtr += sizeof(uint16_t);
    // fill osCap Length
    *(uint16_t *)fillTmpPtr = HtonsInter((uint16_t)(sysCapSize * SINGLE_FEAT_LENGTH));
    fillTmpPtr += sizeof(uint16_t);
    for (uint32_t i = 0; i < sysCapSize; i++) {
        arrayItemPtr = cJSON_GetArrayItem(sysCapPtr, i);
        char *pointPos = strchr(arrayItemPtr->valuestring, '.');
        if (pointPos == NULL) {
            PRINT_ERR("context of \"syscap\" array is invalid\n");
            ret = -1;
            goto FREE_CONVERT_OUT;
        }
        ret = strncmp(arrayItemPtr->valuestring, "SystemCapability.", pointPos - arrayItemPtr->valuestring + 1);
        if (ret != 0) {
            PRINT_ERR("context of \"syscap\" array is invalid\n");
            ret = -1;
            goto FREE_CONVERT_OUT;
        }

        ret = memcpy_s(fillTmpPtr, SINGLE_FEAT_LENGTH, pointPos + 1, strlen(pointPos + 1));
        if (ret != 0) {
            PRINT_ERR("context of \"syscap\" array is invalid\n");
            ret = -1;
            goto FREE_CONVERT_OUT;
        }
        fillTmpPtr += SINGLE_FEAT_LENGTH;
    }

    ret = ConvertedContextSaveAsFile(outDirPath, "rpcid.sc", convertedBuffer, convertedBufLen);
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outDirPath:%s, filename:rpcid.sc\n", outDirPath);
        goto FREE_CONVERT_OUT;
    }

FREE_CONVERT_OUT:
    free(convertedBuffer);
FREE_CONTEXT_OUT:
    FreeContextBuffer(contextBuffer);
    return ret;
}

int32_t RPCIDDecode(char *inputFile, char *outDirPath)
{
    int32_t ret;
    char *contextBuffer = NULL;
    char *contextBufferTail = NULL;
    uint32_t bufferLen;
    char *convertedBuffer = NULL;
    uint16_t sysCaptype, sysCapLength;
    RPCIDHead *headPtr = NULL;
    char *sysCapArrayPtr = NULL;
    cJSON *cjsonObjectRoot = NULL;
    cJSON *sysCapObjectPtr = NULL;

    ret = GetFileContext(inputFile, &contextBuffer, &bufferLen);
    if (ret != 0) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", inputFile);
        return -1;
    }

    contextBufferTail = contextBuffer + bufferLen;
    // 2, to save osSysCaptype & osSysCapLength
    sysCapArrayPtr = contextBuffer + sizeof(RPCIDHead) + 2 * sizeof(uint16_t);
    if (contextBufferTail <= sysCapArrayPtr) {
        PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    headPtr = (RPCIDHead *)contextBuffer;
    if (headPtr->apiVersionType != 1) {
        PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    sysCaptype = NtohsInter(*(uint16_t *)(sysCapArrayPtr - 2 * sizeof(uint16_t))); // 2, for type & length
    if (sysCaptype != 2) { // 2, app syscap type
        PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    sysCapLength = NtohsInter(*(uint16_t *)(sysCapArrayPtr - sizeof(uint16_t)));
    if (contextBufferTail < sysCapArrayPtr + sysCapLength) {
        PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }

    sysCapObjectPtr = cJSON_CreateArray();
    if (sysCapObjectPtr == NULL) {
        PRINT_ERR("cJSON_CreateArray failed\n");
        ret = -1;
        goto FREE_CONTEXT_OUT;
    }
    for (uint32_t i = 0; i < (sysCapLength / SINGLE_FEAT_LENGTH); i++) {
        if (*(sysCapArrayPtr + (i + 1) * SINGLE_FEAT_LENGTH - 1) != '\0') {
            PRINT_ERR("prase file failed, format is invalid, input file : %s\n", inputFile);
            ret = -1;
            goto FREE_SYSCAP_OUT;
        }
        char buffer[SINGLE_FEAT_LENGTH + 17] = "SystemCapability."; // 17, sizeof "SystemCapability."

        ret = strncat_s(buffer, sizeof(buffer), sysCapArrayPtr + i * SINGLE_FEAT_LENGTH, SINGLE_FEAT_LENGTH);
        if (ret != 0) {
            PRINT_ERR("strncat_s failed, (%s, %d, %s, %d)\n",
                      buffer, (int32_t)sizeof(buffer), sysCapArrayPtr + i * SINGLE_FEAT_LENGTH, SINGLE_FEAT_LENGTH);
            ret = -1;
            goto FREE_SYSCAP_OUT;
        }
        if (!cJSON_AddItemToArray(sysCapObjectPtr, cJSON_CreateString(buffer))) {
            PRINT_ERR("cJSON_AddItemToArray or cJSON_CreateString failed\n");
            ret = -1;
            goto FREE_SYSCAP_OUT;
        }
    }

    cjsonObjectRoot = cJSON_CreateObject();
    if (cjsonObjectRoot == NULL) {
        PRINT_ERR("cJSON_CreateObject failed\n");
        ret = -1;
        goto FREE_SYSCAP_OUT;
    }
    if (!cJSON_AddNumberToObject(cjsonObjectRoot, "api_version", NtohsInter(headPtr->apiVersion))) {
        PRINT_ERR("cJSON_AddNumberToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    if (!cJSON_AddItemToObject(cjsonObjectRoot, "syscap", sysCapObjectPtr)) {
        PRINT_ERR("cJSON_AddNumberToObject failed\n");
        ret = -1;
        goto FREE_ROOT_OUT;
    }
    sysCapObjectPtr = NULL;

    convertedBuffer = cJSON_Print(cjsonObjectRoot);

    ret = ConvertedContextSaveAsFile(outDirPath, "rpcid.json", convertedBuffer, strlen(convertedBuffer));
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outDirPath:%s, filename:rpcid.json\n", outDirPath);
        goto FREE_CONVERT_OUT;
    }

FREE_CONVERT_OUT:
    free(convertedBuffer);
FREE_ROOT_OUT:
    cJSON_Delete(cjsonObjectRoot);
FREE_SYSCAP_OUT:
    cJSON_Delete(sysCapObjectPtr);
FREE_CONTEXT_OUT:
    FreeContextBuffer(contextBuffer);
    return ret;
}