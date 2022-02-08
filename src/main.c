/*
 * Copyright (c) 2022 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "syscap_tool.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>

extern char *optarg;

int main(int argc, char **argv)
{
    int32_t ret, optIndex;
    char curpath[PATH_MAX] = {0};
    int32_t rpcid = 0;
    int32_t pcid = 0;
    int32_t encode = 0;
    int32_t decode = 0;
    int32_t help = 0;
    char *inputfile = NULL;
    char *outputpath = getcwd(curpath, sizeof(curpath));

    while (1) {
        static struct option long_options[] = {
            {"help",   no_argument,       0,  'h' },
            {"RPCID",  no_argument,       0,  'R' },
            {"PCID",   no_argument,       0,  'P' },
            {"encode", no_argument,       0,  'e' },
            {"decode", no_argument,       0,  'd' },
            {"input",  required_argument, 0,  'i' },
            {"output", required_argument, 0,  'o' },
            {0,        0,                 0,  0 }
        };

        int32_t flag = getopt_long(argc, argv, "hRPedi:o:", long_options, &optIndex);
        if (flag == -1) {
            break;
        }
        switch (flag) {
            case 'e':
                encode = 1;
                break;
            case 'd':
                decode = 1;
                break;
            case 'R':
                rpcid = 1;
                break;
            case 'P':
                pcid = 1;
                break;
            case 'i':
                inputfile = optarg;
                break;
            case 'o':
                outputpath = optarg;
                break;
            case 'h':
            default:
                help = 1;
        }
    }

    if (rpcid && !pcid && encode && !decode && inputfile && !help) {
        ret = RPCIDEncode(inputfile, outputpath);
    } else if (rpcid && !pcid && !encode && decode && inputfile && !help) {
        ret = RPCIDDecode(inputfile, outputpath);
    } else if (!rpcid && pcid && encode && !decode && inputfile && !help) {
        ret = PCIDEncode(inputfile, outputpath);
    } else if (!rpcid && pcid && !encode && decode && inputfile && !help) {
        ret = PCIDDecode(inputfile, outputpath);
    } else {
        printf("syscap_tool -R/P -e/d -i filepath [-o outpath]\n");
        printf("-h, --help : how to use\n");
        printf("-R, --RPCID : encode or decode RPCID\n");
        printf("-P, --PCID : encode or decode PCID\n");
        printf("-e, --encode : to encode\n");
        printf("-d, --encode : to decode\n");
        printf("-i filepath, --input filepath : input file\n");
        printf("-o outpath, --input outpath : output path\n");
        exit(0);
    }

    if (ret != 0) {
        printf("ERROR: in file %s at line %d -> ", __FILE__, __LINE__); 
        printf("input file(%s) prase failed\n", inputfile);
    }

    return 0;
}