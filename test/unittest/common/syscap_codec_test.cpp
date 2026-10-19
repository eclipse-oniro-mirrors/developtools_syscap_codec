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

#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <securec.h>
#include <string>
#include "syscap_interface.h"
#include "syscap_codec_test.h"
#include "common_method.h"
#include "endian_internal.h"
#include "cJSON.h"
#include "context_tool.h"
#include "syscap_tool.h"
#include "syscap_define.h"

/* internal functions defined in src/syscap_tool.c and interfaces/inner_api/syscap_interface.c */
extern "C" {
char *CopyInputString(const char *inputString);
int32_t GetPriSyscapData(char *input, char **priSyscap, uint32_t *priSyscapLen);
int32_t GetSyscapByIndex(uint32_t index);
int32_t ComparePriSyscap(char *pcid, char *rpcid, uint32_t pcidLen, uint32_t rpcidLen);
int32_t GetPriSyscapCount(char *input);
}

using namespace testing::ext;
using namespace std;

namespace Syscap {
void SyscapCodecTest::SetUpTestCase() {}
void SyscapCodecTest::TearDownTestCase() {}
void SyscapCodecTest::SetUp() {}
void SyscapCodecTest::TearDown() {}

/*
 * @tc.name: EncodeOsSyscap
 * @tc.desc: Check the OsSyscap Coding.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, EncodeOsSyscap, TestSize.Level1)
{
    int pcidLen = PCID_MAIN_BYTES;
    char OsInput[SINGLE_SYSCAP_LEN] = {0};
    EXPECT_TRUE(EncodeOsSyscap(OsInput, pcidLen));
}

/*
 * @tc.name: EncodePrivateSyscap
 * @tc.desc: Check the PrivateSyscap Coding.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, EncodePrivateSyscap, TestSize.Level1)
{
    char *charPriInput = nullptr;
    int priOutLen;
    EXPECT_TRUE(EncodePrivateSyscap(&charPriInput, &priOutLen));
    // Currently, private syscap is null.
    EXPECT_EQ(*charPriInput, '\0');
    EXPECT_EQ(priOutLen, 0);
    free(charPriInput);
}

/*
 * @tc.name: DecodeOsSyscap
 * @tc.desc: Check the OsSyscap Decoding.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, DecodeOsSyscap, TestSize.Level1)
{
    int osSyscap[32] = {1, 3, 3};
    char (*osOutput)[SINGLE_SYSCAP_LEN] = nullptr;
    int decodeOsCnt;
    char expectOsOutput001[] = "SystemCapability.Account.AppAccount";
    char expectOsOutput002[] = "SystemCapability.Account.OsAccount";
    EXPECT_TRUE(DecodeOsSyscap((char *)osSyscap, &osOutput, &decodeOsCnt));
    char (*tmpOsOutput)[SINGLE_SYSCAP_LEN] = osOutput;
    EXPECT_STREQ(*tmpOsOutput, expectOsOutput001);
    EXPECT_STREQ(*(tmpOsOutput + 1), expectOsOutput002);
    EXPECT_EQ(decodeOsCnt, 2);
    free(osOutput);
}

/*
 * @tc.name: DecodePrivateSyscap
 * @tc.desc: Check the null PrivateSyscap Decoding.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, DecodePrivateSyscap, TestSize.Level1)
{
    char (*priOutput)[SINGLE_SYSCAP_LEN] = nullptr;
    char priSyscap[] = "Device.syscap1GEDR,Device.syscap2WREGW,Vendor."
        "syscap3RGD,Vendor.syscap4RWEG,Vendor.syscap5REWGWE,";
    int decodePriCnt;
    char expectPriOutput001[] = "SystemCapability.Device.syscap1GEDR";
    char expectPriOutput002[] = "SystemCapability.Device.syscap2WREGW";
    char expectPriOutput003[] = "SystemCapability.Vendor.syscap3RGD";
    char expectPriOutput004[] = "SystemCapability.Vendor.syscap4RWEG";
    char expectPriOutput005[] = "SystemCapability.Vendor.syscap5REWGWE";
    EXPECT_TRUE(DecodePrivateSyscap(priSyscap, &priOutput, &decodePriCnt));
    char (*tmpPtiOutput)[SINGLE_SYSCAP_LEN] = priOutput;
    EXPECT_STREQ(*tmpPtiOutput++, expectPriOutput001);
    EXPECT_STREQ(*tmpPtiOutput++, expectPriOutput002);
    EXPECT_STREQ(*tmpPtiOutput++, expectPriOutput003);
    EXPECT_STREQ(*tmpPtiOutput++, expectPriOutput004);
    EXPECT_STREQ(*tmpPtiOutput, expectPriOutput005);
    EXPECT_EQ(decodePriCnt, 5);
    free(priOutput);
}

/*
 * @tc.name: DecodePrivateSyscap1
 * @tc.desc: Check the PrivateSyscap Decoding.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, DecodePrivateSyscap1, TestSize.Level1)
{
    char *charPriInput = nullptr;
    char (*priOutput)[SINGLE_SYSCAP_LEN] = nullptr;
    int priOutLen;
    int decodePriCnt;

    EXPECT_TRUE(EncodePrivateSyscap(&charPriInput, &priOutLen));
    if (priOutLen == 0) {
        EXPECT_TRUE(DecodePrivateSyscap(charPriInput, &priOutput, &decodePriCnt));
        EXPECT_EQ((void *)priOutput, (void *)nullptr);
        EXPECT_EQ(decodePriCnt, 0);
        free(priOutput);
    }
    free(charPriInput);
}

/*
 * @tc.name: ComparePcidString
 * @tc.desc: Check the DecodeRpcidToStringFormat Decoding.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePcidString, TestSize.Level1)
{
    CompareError result = {{0}, 0, 0};
    const char pcidString[] = "263168,0,3473408,0,0,0,1634,0,0,0,0,0,0,0,0,0,0,"\
                              "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SystemCapability.vendor.xxxxx3,"\
                              "SystemCapability.device.xxxxx4";
    const char rpcidString[] = "33588992,1766370052,65536,276824064,0,0,0,0,0,0,"\
                               "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"\
                               "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    const char expect[][256] = {"SystemCapability.HiviewDFX.HiLogLite",
                                "SystemCapability.HiviewDFX.Hiview.FaultLogger",
                                "SystemCapability.vendor.xxxxx1",
                                "SystemCapability.device.xxxxx2"};
    int32_t ret = ComparePcidString(pcidString, rpcidString, &result);
    EXPECT_EQ(ret, 3);
    EXPECT_EQ(result.targetApiVersion, 7);
    EXPECT_EQ(result.missSyscapNum, 4);
    for (int i = 0; i < result.missSyscapNum; i++) {
        EXPECT_STREQ(result.syscap[i], expect[i]);
    }
    (void)FreeCompareError(&result);
    EXPECT_EQ(result.targetApiVersion, 0);
    EXPECT_EQ(result.missSyscapNum, 0);
    for (int i = 0; i < MAX_MISS_SYSCAP; i++) {
        EXPECT_EQ((void *)result.syscap[i], (void *)nullptr);
    }
}

constexpr uint32_t RPCID_SYSCAP_LEN_OFFSET = sizeof(uint32_t);

static void SetRpcidSysCapLength(char *input, uint16_t sysCapLength)
{
    *reinterpret_cast<uint16_t *>(input + RPCID_SYSCAP_LEN_OFFSET) = HtonsInter(sysCapLength);
}

/*
 * @tc.name: ValidateRpcidInputNullInput
 * @tc.desc: Check the null Input Validation.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ValidateRpcidInputNullInput, TestSize.Level1)
{
    uint16_t sysCapLength = 0;
    int32_t ret = ValidateRpcidInput(nullptr, sizeof(uint32_t) + sizeof(uint16_t), &sysCapLength);
    EXPECT_EQ(ret, -1);
}

/*
 * @tc.name: ValidateRpcidInputTooShortLength
 * @tc.desc: Check the too short Input Validation.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ValidateRpcidInputTooShortLength, TestSize.Level1)
{
    char input[sizeof(uint32_t) + sizeof(uint16_t)] = {0};
    uint16_t sysCapLength = 0;
    int32_t ret = ValidateRpcidInput(input, sizeof(input) - 1, &sysCapLength);
    EXPECT_EQ(ret, -1);
}

/*
 * @tc.name: ValidateRpcidInputSyscapOverflow
 * @tc.desc: Check the syscap Overflow Validation.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ValidateRpcidInputSyscapOverflow, TestSize.Level1)
{
    char input[16] = {0};
    uint16_t sysCapLength = 0;
    SetRpcidSysCapLength(input, 11);
    int32_t ret = ValidateRpcidInput(input, sizeof(input), &sysCapLength);
    EXPECT_EQ(ret, -1);
    EXPECT_EQ(sysCapLength, 11);
}

/*
 * @tc.name: ValidateRpcidInputEmptySyscap
 * @tc.desc: Check the empty Syscap Validation.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ValidateRpcidInputEmptySyscap, TestSize.Level1)
{
    char input[16] = {0};
    uint16_t sysCapLength = 0;
    int32_t ret = ValidateRpcidInput(input, sizeof(input), &sysCapLength);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(sysCapLength, 0);
}

/*
 * @tc.name: ValidateRpcidInputNormal
 * @tc.desc: Check the normal Rpcid Validation.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ValidateRpcidInputNormal, TestSize.Level1)
{
    char input[32] = {0};
    uint16_t sysCapLength = 0;
    SetRpcidSysCapLength(input, 10);
    int32_t ret = ValidateRpcidInput(input, sizeof(input), &sysCapLength);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(sysCapLength, 10);
}

/*
 * @tc.name: CompareOsSyscapNoMissing
 * @tc.desc: Check the CompareOsSyscap with no missing syscap.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CompareOsSyscapNoMissing, TestSize.Level1)
{
    uint32_t pcidOsArray[PCID_MAIN_INTS] = {0};
    uint32_t rpcidOsArray[PCID_MAIN_INTS] = {0};
    for (int i = 0; i < PCID_MAIN_INTS; i++) {
        pcidOsArray[i] = 0xFFFFFFFF;
        rpcidOsArray[i] = 0xFFFFFFFF;
    }
    int32_t ret = CompareOsSyscap(pcidOsArray, rpcidOsArray);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: CompareOsSyscapNoDifference
 * @tc.desc: Check the CompareOsSyscap with no difference.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CompareOsSyscapNoDifference, TestSize.Level1)
{
    uint32_t pcidOsArray[PCID_MAIN_INTS] = {0};
    uint32_t rpcidOsArray[PCID_MAIN_INTS] = {0};
    for (int i = 0; i < PCID_MAIN_INTS; i++) {
        pcidOsArray[i] = 0x12345678;
        rpcidOsArray[i] = 0x12345678;
    }
    int32_t ret = CompareOsSyscap(pcidOsArray, rpcidOsArray);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: CompareOsSyscapOneMissing
 * @tc.desc: Check the CompareOsSyscap with one missing syscap.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CompareOsSyscapOneMissing, TestSize.Level1)
{
    uint32_t pcidOsArray[PCID_MAIN_INTS] = {0};
    uint32_t rpcidOsArray[PCID_MAIN_INTS] = {0};
    for (int i = 0; i < PCID_MAIN_INTS; i++) {
        pcidOsArray[i] = 0xFFFFFFFF;
        rpcidOsArray[i] = 0xFFFFFFFF;
    }
    // 2 is the pcid & rpcid header length in uint32_t, pos = (2 - 2) * 32 + 0 = 0.
    pcidOsArray[2] &= ~(1U << 0);
    rpcidOsArray[2] |= (1U << 0);
    int32_t ret = CompareOsSyscap(pcidOsArray, rpcidOsArray);
    EXPECT_EQ(ret, 1);
}

/*
 * @tc.name: CompareOsSyscapMultipleMissing
 * @tc.desc: Check the CompareOsSyscap with multiple missing syscap.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CompareOsSyscapMultipleMissing, TestSize.Level1)
{
    uint32_t pcidOsArray[PCID_MAIN_INTS] = {0};
    uint32_t rpcidOsArray[PCID_MAIN_INTS] = {0};
    for (int i = 0; i < PCID_MAIN_INTS; i++) {
        pcidOsArray[i] = 0xFFFFFFFF;
        rpcidOsArray[i] = 0xFFFFFFFF;
    }
    // 2 is the pcid & rpcid header length in uint32_t.
    pcidOsArray[2] &= ~(1U << 0);
    rpcidOsArray[2] |= (1U << 0);
    pcidOsArray[2] &= ~(1U << 1);
    rpcidOsArray[2] |= (1U << 1);
    int32_t ret = CompareOsSyscap(pcidOsArray, rpcidOsArray);
    EXPECT_EQ(ret, 2);
}

/*
 * @tc.name: CompareOsSyscapDifferenceInPcidOnly
 * @tc.desc: Check the CompareOsSyscap with difference in pcid only.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CompareOsSyscapDifferenceInPcidOnly, TestSize.Level1)
{
    uint32_t pcidOsArray[PCID_MAIN_INTS] = {0};
    uint32_t rpcidOsArray[PCID_MAIN_INTS] = {0};
    for (int i = 0; i < PCID_MAIN_INTS; i++) {
        pcidOsArray[i] = 0xFFFFFFFF;
        rpcidOsArray[i] = 0xFFFFFFFF;
    }
    // 2 is the pcid & rpcid header length in uint32_t.
    pcidOsArray[2] |= (1U << 0);
    rpcidOsArray[2] &= ~(1U << 0);
    int32_t ret = CompareOsSyscap(pcidOsArray, rpcidOsArray);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: CompareOsSyscapBreakOnPosOutOfRange
 * @tc.desc: Check the CompareOsSyscap breaks when pos is out of range.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CompareOsSyscapBreakOnPosOutOfRange, TestSize.Level1)
{
    uint32_t pcidOsArray[PCID_MAIN_INTS] = {0};
    uint32_t rpcidOsArray[PCID_MAIN_INTS] = {0};
    for (int i = 0; i < PCID_MAIN_INTS; i++) {
        pcidOsArray[i] = 0xFFFFFFFF;
        rpcidOsArray[i] = 0xFFFFFFFF;
    }
    // pos = (31 - 2) * 32 + 31 = 959, far beyond the size of g_arraySyscap.
    pcidOsArray[PCID_MAIN_INTS - 1] &= ~(1U << 31);
    rpcidOsArray[PCID_MAIN_INTS - 1] |= (1U << 31);
    int32_t ret = CompareOsSyscap(pcidOsArray, rpcidOsArray);
    EXPECT_EQ(ret, 0);
}

/*
 * @tc.name: EndianConvert16RoundTrip
 * @tc.desc: Check the 16-bit endian conversion round trip.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, EndianConvert16RoundTrip, TestSize.Level1)
{
    const uint16_t input[] = {0x1234, 0xABCD, 0x8000, 0x0001};
    for (size_t i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
        EXPECT_EQ(NtohsInter(HtonsInter(input[i])), input[i]);
        EXPECT_EQ(HtonsInter(NtohsInter(input[i])), input[i]);
        EXPECT_EQ(HtonsInter(HtonsInter(input[i])), input[i]);
        EXPECT_EQ(NtohsInter(NtohsInter(input[i])), input[i]);
    }
}

/*
 * @tc.name: EndianConvert32RoundTrip
 * @tc.desc: Check the 32-bit endian conversion round trip.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, EndianConvert32RoundTrip, TestSize.Level1)
{
    const uint32_t input[] = {0x12345678, 0xDEADBEEF, 0x80000001, 0x000000FF};
    for (size_t i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
        EXPECT_EQ(NtohlInter(HtonlInter(input[i])), input[i]);
        EXPECT_EQ(HtonlInter(NtohlInter(input[i])), input[i]);
        EXPECT_EQ(HtonlInter(HtonlInter(input[i])), input[i]);
        EXPECT_EQ(NtohlInter(NtohlInter(input[i])), input[i]);
    }
}

/*
 * @tc.name: SafeFreeValidPointer
 * @tc.desc: Check SafeFree with a valid malloced pointer.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, SafeFreeValidPointer, TestSize.Level1)
{
    char *pointer = static_cast<char *>(malloc(SINGLE_SYSCAP_LEN));
    ASSERT_NE(pointer, nullptr);
    (void)memset_s(pointer, SINGLE_SYSCAP_LEN, 0, SINGLE_SYSCAP_LEN);
    SafeFree(pointer);
}

/*
 * @tc.name: CreateWholeSyscapJsonObjNormal
 * @tc.desc: Check the whole syscap json object creation and lookup.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CreateWholeSyscapJsonObjNormal, TestSize.Level1)
{
    cJSON *syscapJsonObj = CreateWholeSyscapJsonObj();
    ASSERT_NE(syscapJsonObj, nullptr);
    cJSON *item = cJSON_GetObjectItem(syscapJsonObj, "SystemCapability.Account.AppAccount");
    ASSERT_NE(item, nullptr);
    EXPECT_TRUE(cJSON_IsNumber(item));
    EXPECT_EQ(item->valueint, ACCOUNT_APPACCOUNT);
    item = cJSON_GetObjectItem(syscapJsonObj, "SystemCapability.HiviewDFX.HiLogLite");
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(item->valueint, HIVIEWDFX_HILOGLITE);
    EXPECT_EQ(cJSON_GetObjectItem(syscapJsonObj, "SystemCapability.not.exist.syscap"), nullptr);
    cJSON_Delete(syscapJsonObj);
}

/*
 * @tc.name: EncodeOsSyscapInvalidLength
 * @tc.desc: Check EncodeOsSyscap with invalid input length.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, EncodeOsSyscapInvalidLength, TestSize.Level1)
{
    char output[PCID_MAIN_BYTES] = {0};
    EXPECT_FALSE(EncodeOsSyscap(output, PCID_MAIN_BYTES - 1));
    EXPECT_FALSE(EncodeOsSyscap(output, PCID_MAIN_BYTES + 1));
}

/*
 * @tc.name: DecodeOsSyscapUnmappedIndex
 * @tc.desc: Check DecodeOsSyscap skips os syscap index not defined in g_arraySyscap.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, DecodeOsSyscapUnmappedIndex, TestSize.Level1)
{
    char input[PCID_MAIN_BYTES] = {0};
    char (*osOutput)[SINGLE_SYSCAP_LEN] = nullptr;
    int decodeOsCnt = 0;
    // 8, pcid header size in bytes; bit 0 of the first os syscap byte, index 0.
    input[8] = 0x01;
    // bit 7 of the last os syscap byte, index 959, not defined in g_arraySyscap.
    input[8 + 120 - 1] = 0x80;
    EXPECT_TRUE(DecodeOsSyscap(input, &osOutput, &decodeOsCnt));
    ASSERT_NE(osOutput, nullptr);
    EXPECT_EQ(decodeOsCnt, 2); // 2, one mapped index and one unmapped index
    EXPECT_STREQ(osOutput[0], "SystemCapability.Account.AppAccount");
    EXPECT_STREQ(osOutput[1], "");
    free(osOutput);
}

/*
 * @tc.name: DecodePrivateSyscapNullInput
 * @tc.desc: Check DecodePrivateSyscap with null input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, DecodePrivateSyscapNullInput, TestSize.Level1)
{
    char (*priOutput)[SINGLE_SYSCAP_LEN] = nullptr;
    int decodePriCnt = 0;
    EXPECT_FALSE(DecodePrivateSyscap(nullptr, &priOutput, &decodePriCnt));
}

/*
 * @tc.name: GetPriSyscapCountEmpty
 * @tc.desc: Check GetPriSyscapCount with empty input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, GetPriSyscapCountEmpty, TestSize.Level1)
{
    char input[] = "";
    EXPECT_EQ(GetPriSyscapCount(input), 0);
}

/*
 * @tc.name: GetPriSyscapCountNormal
 * @tc.desc: Check GetPriSyscapCount with normal input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, GetPriSyscapCountNormal, TestSize.Level1)
{
    char inputNoSeparator[] = "Device.syscap1";
    EXPECT_EQ(GetPriSyscapCount(inputNoSeparator), 0);
    char inputNormal[] = "Device.syscap1,Device.syscap2,Vendor.syscap3";
    EXPECT_EQ(GetPriSyscapCount(inputNormal), 2);
}

/*
 * @tc.name: CopyInputStringNull
 * @tc.desc: Check CopyInputString with null input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CopyInputStringNull, TestSize.Level1)
{
    EXPECT_EQ((void *)CopyInputString(nullptr), (void *)nullptr);
}

/*
 * @tc.name: CopyInputStringEmpty
 * @tc.desc: Check CopyInputString with empty input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CopyInputStringEmpty, TestSize.Level1)
{
    char input[] = "";
    EXPECT_EQ((void *)CopyInputString(input), (void *)nullptr);
}

/*
 * @tc.name: CopyInputStringNormal
 * @tc.desc: Check CopyInputString with normal input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CopyInputStringNormal, TestSize.Level1)
{
    const char input[] = "SystemCapability.Device.syscap1";
    char *output = CopyInputString(input);
    ASSERT_NE(output, nullptr);
    EXPECT_STREQ(output, input);
    free(output);
}

/*
 * @tc.name: CopyInputStringMaxLength
 * @tc.desc: Check CopyInputString with max allowed length input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CopyInputStringMaxLength, TestSize.Level1)
{
    // 1024, STRING_FORMAT_LEN_MAX defined in src/syscap_tool.c
    std::string maxLength(1024, 'a');
    char *output = CopyInputString(maxLength.c_str());
    ASSERT_NE(output, nullptr);
    EXPECT_EQ(strlen(output), maxLength.size());
    EXPECT_STREQ(output, maxLength.c_str());
    free(output);
}

/*
 * @tc.name: CopyInputStringTooLong
 * @tc.desc: Check CopyInputString with too long input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, CopyInputStringTooLong, TestSize.Level1)
{
    // 1025, one byte longer than STRING_FORMAT_LEN_MAX defined in src/syscap_tool.c
    std::string tooLong(1025, 'a');
    EXPECT_EQ((void *)CopyInputString(tooLong.c_str()), (void *)nullptr);
}

/*
 * @tc.name: GetPriSyscapDataNormal
 * @tc.desc: Check GetPriSyscapData with normal input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, GetPriSyscapDataNormal, TestSize.Level1)
{
    char input[] = "Device.syscap1,Vendor.syscap2";
    char *priSyscap = nullptr;
    uint32_t priSyscapLen = 0;
    EXPECT_EQ(GetPriSyscapData(input, &priSyscap, &priSyscapLen), 0);
    ASSERT_NE(priSyscap, nullptr);
    EXPECT_EQ(priSyscapLen, 2U);
    EXPECT_STREQ(priSyscap, "Device.syscap1");
    EXPECT_STREQ(priSyscap + SINGLE_SYSCAP_LEN, "Vendor.syscap2");
    free(priSyscap);
}

/*
 * @tc.name: GetPriSyscapDataSingle
 * @tc.desc: Check GetPriSyscapData with single private syscap.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, GetPriSyscapDataSingle, TestSize.Level1)
{
    char input[] = "Device.syscap1";
    char *priSyscap = nullptr;
    uint32_t priSyscapLen = 0;
    EXPECT_EQ(GetPriSyscapData(input, &priSyscap, &priSyscapLen), 0);
    ASSERT_NE(priSyscap, nullptr);
    EXPECT_EQ(priSyscapLen, 1U);
    EXPECT_STREQ(priSyscap, "Device.syscap1");
    free(priSyscap);
}

/*
 * @tc.name: SeparateSyscapFromStringInvalidOsArraySize
 * @tc.desc: Check SeparateSyscapFromString with invalid os array size.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, SeparateSyscapFromStringInvalidOsArraySize, TestSize.Level1)
{
    uint32_t osArray[PCID_MAIN_INTS] = {0};
    char *priSyscap = nullptr;
    uint32_t priSyscapLen = 0;
    EXPECT_EQ(SeparateSyscapFromString("1792,0", osArray, PCID_MAIN_INTS - 1, &priSyscap, &priSyscapLen), -1);
}

/*
 * @tc.name: SeparateSyscapFromStringNullInput
 * @tc.desc: Check SeparateSyscapFromString with null input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, SeparateSyscapFromStringNullInput, TestSize.Level1)
{
    uint32_t osArray[PCID_MAIN_INTS] = {0};
    char *priSyscap = nullptr;
    uint32_t priSyscapLen = 0;
    EXPECT_EQ(SeparateSyscapFromString(nullptr, osArray, PCID_MAIN_INTS, &priSyscap, &priSyscapLen), -1);
}

/*
 * @tc.name: SeparateSyscapFromStringNormal
 * @tc.desc: Check SeparateSyscapFromString with normal pcid string.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, SeparateSyscapFromStringNormal, TestSize.Level1)
{
    // 32 header & os syscap numbers + 2 private syscaps, api version is 7 (1792 >> 8).
    const char inputString[] = "1792,0,65536,276824064,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    uint32_t osArray[PCID_MAIN_INTS] = {0};
    char *priSyscap = nullptr;
    uint32_t priSyscapLen = 0;
    EXPECT_EQ(SeparateSyscapFromString(inputString, osArray, PCID_MAIN_INTS, &priSyscap, &priSyscapLen), 0);
    EXPECT_EQ(osArray[0], 1792U);
    EXPECT_EQ(osArray[1], 0U);
    EXPECT_EQ(osArray[2], 65536U);
    EXPECT_EQ(osArray[3], 276824064U);
    EXPECT_EQ(osArray[PCID_MAIN_INTS - 1], 0U);
    ASSERT_NE(priSyscap, nullptr);
    EXPECT_EQ(priSyscapLen, 2U);
    EXPECT_STREQ(priSyscap, "SystemCapability.vendor.xxxxx1");
    EXPECT_STREQ(priSyscap + SINGLE_SYSCAP_LEN, "SystemCapability.device.xxxxx2");
    free(priSyscap);
}

/*
 * @tc.name: GetSyscapByIndexValid
 * @tc.desc: Check GetSyscapByIndex with valid syscap index.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, GetSyscapByIndexValid, TestSize.Level1)
{
    int32_t ret = GetSyscapByIndex(ACCOUNT_APPACCOUNT);
    ASSERT_GE(ret, 0);
    EXPECT_EQ(g_arraySyscap[ret].num, ACCOUNT_APPACCOUNT);
    EXPECT_STREQ(g_arraySyscap[ret].str, "SystemCapability.Account.AppAccount");
    ret = GetSyscapByIndex(HIVIEWDFX_HILOGLITE);
    ASSERT_GE(ret, 0);
    EXPECT_EQ(g_arraySyscap[ret].num, HIVIEWDFX_HILOGLITE);
    EXPECT_STREQ(g_arraySyscap[ret].str, "SystemCapability.HiviewDFX.HiLogLite");
}

/*
 * @tc.name: GetSyscapByIndexInvalid
 * @tc.desc: Check GetSyscapByIndex with invalid syscap index.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, GetSyscapByIndexInvalid, TestSize.Level1)
{
    const uint32_t allSyscapNum = sizeof(g_arraySyscap) / sizeof(SyscapWithNum);
    EXPECT_EQ(GetSyscapByIndex(allSyscapNum), -1);
    // 959, max os syscap index (120 * 8 - 1), not defined in g_arraySyscap.
    EXPECT_EQ(GetSyscapByIndex(959), -1);
}

static int32_t FillPriSyscapBuffer(char *buffer, const char *const *syscaps, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++) {
        int32_t ret = snprintf_s(buffer + i * SINGLE_SYSCAP_LEN, SINGLE_SYSCAP_LEN, SINGLE_SYSCAP_LEN - 1, "%s",
                                 syscaps[i]);
        if (ret < 0) {
            return -1;
        }
    }
    return 0;
}

/*
 * @tc.name: ComparePriSyscapNoMissing
 * @tc.desc: Check ComparePriSyscap with pcid containing all rpcid private syscaps.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePriSyscapNoMissing, TestSize.Level1)
{
    char pcidBuffer[SINGLE_SYSCAP_LEN * 2] = {0};
    char rpcidBuffer[SINGLE_SYSCAP_LEN * 2] = {0};
    const char *pcidSyscaps[] = {"SystemCapability.vendor.xxxxx1", "SystemCapability.device.xxxxx2"};
    const char *rpcidSyscaps[] = {"SystemCapability.vendor.xxxxx1", "SystemCapability.device.xxxxx2"};
    ASSERT_EQ(FillPriSyscapBuffer(pcidBuffer, pcidSyscaps, 2), 0);
    ASSERT_EQ(FillPriSyscapBuffer(rpcidBuffer, rpcidSyscaps, 2), 0);
    EXPECT_EQ(ComparePriSyscap(pcidBuffer, rpcidBuffer, 2, 2), 0);
}

/*
 * @tc.name: ComparePriSyscapOneMissing
 * @tc.desc: Check ComparePriSyscap with one missing private syscap.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePriSyscapOneMissing, TestSize.Level1)
{
    char pcidBuffer[SINGLE_SYSCAP_LEN * 2] = {0};
    char rpcidBuffer[SINGLE_SYSCAP_LEN * 2] = {0};
    const char *pcidSyscaps[] = {"SystemCapability.vendor.xxxxx1", ""};
    const char *rpcidSyscaps[] = {"SystemCapability.vendor.xxxxx1", "SystemCapability.device.xxxxx2"};
    ASSERT_EQ(FillPriSyscapBuffer(pcidBuffer, pcidSyscaps, 2), 0);
    ASSERT_EQ(FillPriSyscapBuffer(rpcidBuffer, rpcidSyscaps, 2), 0);
    EXPECT_EQ(ComparePriSyscap(pcidBuffer, rpcidBuffer, 2, 2), 1);
}

/*
 * @tc.name: ComparePriSyscapPcidSuperset
 * @tc.desc: Check ComparePriSyscap with pcid private syscaps more than rpcid.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePriSyscapPcidSuperset, TestSize.Level1)
{
    char pcidBuffer[SINGLE_SYSCAP_LEN * 3] = {0};
    char rpcidBuffer[SINGLE_SYSCAP_LEN * 2] = {0};
    const char *pcidSyscaps[] = {"SystemCapability.vendor.xxxxx1", "SystemCapability.device.xxxxx2",
                                 "SystemCapability.vendor.xxxxx3"};
    const char *rpcidSyscaps[] = {"SystemCapability.device.xxxxx2", "SystemCapability.vendor.xxxxx1"};
    ASSERT_EQ(FillPriSyscapBuffer(pcidBuffer, pcidSyscaps, 3), 0);
    ASSERT_EQ(FillPriSyscapBuffer(rpcidBuffer, rpcidSyscaps, 2), 0);
    EXPECT_EQ(ComparePriSyscap(pcidBuffer, rpcidBuffer, 3, 2), 0);
}

/*
 * @tc.name: ComparePcidStringMeetRequirements
 * @tc.desc: Check ComparePcidString returns E_OK when pcid meets rpcid.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePcidStringMeetRequirements, TestSize.Level1)
{
    CompareError result = {{0}, 0, 0};
    // api version 7 (1792 >> 8), os syscap index 16 / 55 / 60, 2 private syscaps.
    const char pcidString[] = "1792,0,65536,276824064,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                              "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                              "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    const char rpcidString[] = "1792,0,65536,276824064,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    int32_t ret = ComparePcidString(pcidString, rpcidString, &result);
    EXPECT_EQ(ret, E_OK);
    EXPECT_EQ(result.missSyscapNum, 0);
    EXPECT_EQ(result.targetApiVersion, 0);
    EXPECT_EQ(FreeCompareError(&result), 0);
}

/*
 * @tc.name: ComparePcidStringApiVersionTooLow
 * @tc.desc: Check ComparePcidString returns E_APIVERSION when pcid api version is too low.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePcidStringApiVersionTooLow, TestSize.Level1)
{
    CompareError result = {{0}, 0, 0};
    // pcid api version is 4 (263168 >> 8), rpcid api version is 7 (1792 >> 8).
    const char pcidString[] = "263168,0,65536,276824064,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                              "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                              "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    const char rpcidString[] = "1792,0,65536,276824064,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    int32_t ret = ComparePcidString(pcidString, rpcidString, &result);
    EXPECT_EQ(ret, E_APIVERSION);
    EXPECT_EQ(result.targetApiVersion, 7);
    EXPECT_EQ(result.missSyscapNum, 0);
    EXPECT_EQ(FreeCompareError(&result), 0);
    EXPECT_EQ(result.targetApiVersion, 0);
}

/*
 * @tc.name: ComparePcidStringMissOsSyscap
 * @tc.desc: Check ComparePcidString returns E_SYSCAP when os syscaps are missing.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePcidStringMissOsSyscap, TestSize.Level1)
{
    CompareError result = {{0}, 0, 0};
    // pcid misses os syscap index 55 & 60 (word 3 bits 23 & 28 are not set).
    const char pcidString[] = "1792,0,65536,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                              "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                              "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    const char rpcidString[] = "1792,0,65536,276824064,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
                               "SystemCapability.vendor.xxxxx1,SystemCapability.device.xxxxx2";
    int32_t ret = ComparePcidString(pcidString, rpcidString, &result);
    EXPECT_EQ(ret, E_SYSCAP);
    EXPECT_EQ(result.missSyscapNum, 2);
    EXPECT_EQ(result.targetApiVersion, 0);
    EXPECT_STREQ(result.syscap[0], "SystemCapability.HiviewDFX.HiLogLite");
    EXPECT_STREQ(result.syscap[1], "SystemCapability.HiviewDFX.Hiview.FaultLogger");
    EXPECT_EQ(FreeCompareError(&result), 0);
    EXPECT_EQ(result.missSyscapNum, 0);
    EXPECT_EQ((void *)result.syscap[0], (void *)nullptr);
    EXPECT_EQ((void *)result.syscap[1], (void *)nullptr);
}

/*
 * @tc.name: ComparePcidStringNullInput
 * @tc.desc: Check ComparePcidString with null input.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, ComparePcidStringNullInput, TestSize.Level1)
{
    CompareError result = {{0}, 0, 0};
    EXPECT_EQ(ComparePcidString(nullptr, nullptr, &result), E_ERROR);
}

/*
 * @tc.name: FreeCompareErrorNull
 * @tc.desc: Check FreeCompareError with null result.
 * @tc.type: FUNC
 */
HWTEST_F(SyscapCodecTest, FreeCompareErrorNull, TestSize.Level1)
{
    EXPECT_EQ(FreeCompareError(nullptr), 0);
}
} // namespace Syscap
