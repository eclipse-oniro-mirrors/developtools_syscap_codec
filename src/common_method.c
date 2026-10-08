/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common_method.h"
#include "context_tool.h"
#include "endian_internal.h"
#include <stdlib.h>

void SafeFree(char *pointer)
{
    if ((pointer) != NULL) {
        free(pointer);
    }
}

int32_t ValidateRpcidInput(const char *input, uint32_t inputLen, uint16_t *sysCapLength)
{
    // validate input buffer length before reading to prevent out-of-bounds read
    if (input == NULL || inputLen < sizeof(uint32_t) + sizeof(uint16_t)) {
        PRINT_ERR("Input buffer too small, inputLen = %u.\n", inputLen);
        return -1;
    }
    *sysCapLength = NtohsInter(*(uint16_t *)(input + sizeof(uint32_t)));
    if (inputLen < sizeof(RPCIDHead) + sizeof(uint32_t) + *sysCapLength) {
        PRINT_ERR("Input buffer too small, inputLen = %u, sysCapLength = %u.\n",
                  inputLen, *sysCapLength);
        return -1;
    }
    return 0;
}
