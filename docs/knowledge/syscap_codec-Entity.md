# 业务实体知识

syscap_codec 仓核心业务实体的完整知识定义，涵盖静态概念、动态运行、规则约束与代码映射。> 源码路径均相对仓根 `developtools/syscap_codec`。

## SysCap 系统能力（SystemCapability / syscap_define.h）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | SysCap（SystemCapability） |
| 实体定义 | 设备能力的原子描述单位，具有全 OS 唯一的字符串标识与整数编号；是 PCID/RPCID 编码的元素集合 |
| 核心特征 | 字符串形如 `SystemCapability.XX.Yyy`，单条上限 273 字节（`SINGLE_SYSCAP_LEN = 256 + 17`）；主清单 386 项（枚举 `SystemCapabilityNum` 与 `g_arraySyscap[]` 1:1），30 项标 `// abandoned`（保留不删） |
| 类型/分类 | OS SysCap（在主清单有编号，编入位图，上限 960）与 Private SysCap（厂商私有，去前缀逗号串传输） |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 各部件在 bundle.json `component.syscap` 声明 → 构建系统 preloader 汇总为产品 SystemCapability.json → 构建期 generate_pcid 编码进 pcid.sc；编码时 `CreateWholeSyscapJsonObj()`（`src/context_tool.c:217`）按名称查编号分流 OS/Private |
| 状态流转 | 只增不减：新增末尾追加；废弃注释 `// abandoned`；运行期只读 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 枚举与数组必须 1:1 且升序；编号即 pcid 位图 bit 位；外部扩展编号 ≥ 500（`tools/syscap_config_merge.py`） |
| 并发约束 | `g_arraySyscap` 为 `const static` 只读数据，无并发问题 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | PCID/RPCID/字符串格式都以 SysCap 为元素 |
| 下游影响 | bundle.json 声明、sdk d.ts `@syscap` 注解、设备位图 |
| 平级关联 | 一致性检查工具（syscap_check.py 三方比对） |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | `SyscapWithNum{char str[273]; uint16_t num}`、枚举 `SystemCapabilityNum`（`include/codec_config/syscap_define.h`，834 行） |
| 核心方法 | `CreateWholeSyscapJsonObj()`（名称→编号 cJSON 对象）；`GetSyscapByIndex()`（编号→数组下标，`src/syscap_tool.c:626`） |

## PCID 设备能力标识（pcid.sc / PCIDMain）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | PCID（Product Compatibility ID） |
| 实体定义 | 一台设备（一个产品镜像）完整能力集的自描述二进制；比较操作中的"供给侧" |
| 核心特征 | `PCIDMain`：位域头 `apiVersion:15 + apiVersionType:1(恒0) + systemType:3 + reserved:13`（2 字）+ `manufacturerID`(u32) + `osSyscap[120]` 位图 = 128 字节，后接 private syscap 去前缀逗号尾串；多字节大端 |
| 类型/分类 | 二进制（pcid.sc，上限 1128 字节）/ JSON（pcid.json）/ 字符串（pcid.txt）；systemType：mini=0b001、small=0b010、standard=0b100 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 创建：构建期 `build_ext_component("generate_pcid")` 用 host syscap_tool 对 preloader 的 SystemCapability.json 执行 `-P -e` → `CreatePCID()`（`src/create_pcid.c:247`：`SetPCIDHeader`+`SetOsSyscap`+`SetPriSyscap`）→ `ohos_prebuilt_etc("pcid.sc")` 装入镜像 `/system/etc/pcid.sc`。消费：IDE 拿开发者导入的 pcid 解码（`DecodePCID`）；设备侧共享库 `EncodeOsSyscap`/`EncodePrivateSyscap` 读 `/system/etc/pcid.sc` |
| 状态流转 | json（api_version/system_type/manufacturer_id/syscap.os/syscap.private）→ 位图+尾串（编码）→ json/txt（解码/字符串化）；单向流转，无状态回写 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | OS SysCap 数量 ≤ 960（sector < 120 校验）；private 总长 ≤ 1000（`PRIVATE_SYSCAP_SIZE`，`GetPriSyscapLen` 校验）；解码侧 `apiVersionType != 0` 即拒识 |
| 并发约束 | 单进程顺序处理；设备上 `/system/etc/pcid.sc` 只读 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | 元素来自 SysCap 主清单；内容来自产品 SystemCapability.json |
| 下游影响 | 与 RPCID 比较判定设备是否可装某应用；NAPI/Taihe 查询的数据源 |
| 平级关联 | 字符串格式（`EncodePcidscToString`/`DecodeStringPCIDToJson`） |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | `PCIDMain`、`PCIDHeader`（`include/create_pcid.h:23-38`） |
| 核心方法 | `CreatePCID()`/`DecodePCID()`/`EncodePcidscToString()`/`DecodeStringPCIDToJson()`（`src/create_pcid.c:247/478/862/721`）；`SetPCIDHeader`/`SetOsSyscap`/`SetPriSyscap`（同文件） |

## RPCID 应用需求标识（rpcid.sc / RPCIDHead）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | RPCID（Required Product Compatibility ID） |
| 实体定义 | 一个应用所需能力集的二进制声明；比较操作中的"需求侧" |
| 核心特征 | `RPCIDHead{apiVersion:15, apiVersionType:1(恒1)}` + `sysCaptype`(u16，恒 2=app syscap) + `sysCapLength`(u16 字节) + N×256 字节定长记录（去 `SystemCapability.` 前缀，`\0` 补齐）；大端 |
| 类型/分类 | 二进制（rpcid.sc）/ JSON（rpcid.json，`api_version`+`syscap[]` 全称数组）/ 字符串（rpcid.txt） |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | IDE 依据应用配置与 API 版本生成 rpcid.json → `RPCIDEncode()`（`src/syscap_tool.c:145`：`FillOsCapLength` 逐条校验前缀并拷贝定长块）→ rpcid.sc；应用市场拿 rpcid 与设备 pcid 比较 |
| 状态流转 | json → sc → json/txt；`api_version` 取自 json 数字字段，写头时 `HtonsInter` |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 数组元素必须是 `SystemCapability.` 前缀字符串（`FillOsCapLength` 用 strchr+strncmp 校验）；`CheckRpcidFormat`（`src/context_tool.c:177`）校验 `apiVersionType==1`、`sysCaptype==2`、长度自洽；`ValidateRpcidInput` 先验 buffer 长度 |
| 并发约束 | 单进程顺序处理 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | 元素来自 SysCap 集合；内容由 IDE 生成 |
| 下游影响 | `ComparePcidWithRpcidString`/`ComparePcidString` 的需求输入；`DecodeRpcidToStringFormat`（共享库） |
| 平级关联 | PCID（互为比较双方）；字符串格式 |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | `RPCIDHead`（`include/context_tool.h:36-39`） |
| 核心方法 | `RPCIDEncode()`/`RPCIDDecode()`/`EncodeRpcidscToString()`（`src/syscap_tool.c:145/256/470`）；`FillOsCapLength`/`ParseRpcidToJson`；`CheckRpcidFormat`/`ValidateRpcidInput` |

## 字符串格式能力标识（pcid.txt / rpcid.txt）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | 字符串格式（string format） |
| 实体定义 | `.sc` 二进制的文本投影：32 个 uint32 十进制逗号连接 + private syscap 全称逗号列表；供文本通道传输与人读 |
| 核心特征 | 前 2 个 u32 = 二进制头 8 字节，后 30 个 u32 = 120 字节 OS 位图（`RPCID_OUT_BUFFER`/`PCID_OUT_BUFFER = 32`）；输入上限 1024 字节（`STRING_FORMAT_LEN_MAX`） |
| 类型/分类 | 由文件（txt）或内联字符串（`-sC` 直接传串）承载 |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 编码：`-Pes`/`-Res` 把 `.sc` 转字符串（`EncodePcidscToString`/`EncodeRpcidscToString`：头部 u32 原样打十进制 + OS syscap 查表转位图 + private 补前缀全称）。解码：`-Pdsi` 转回 json（`DecodeStringPCIDToJson`：`ParseStringSyscap` sscanf 拆头与位图）。比较：`-C`/`-sC`（`ComparePcidWithRpcidString`，TYPE_FILE/TYPE_STRING 双模式） |
| 状态流转 | `.sc` ↔ 字符串 ↔ json 三态互转；字符串解析后 `SeparateSyscapFromString`（`src/syscap_tool.c:594`）拆出 osArray[32] + priSyscap 定长块数组 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 解析按 32 个 u32 逐一 sscanf（`SeparateSyscapFromString`）；private 串按逗号 strtok_r 切分，每条定长 273 字节存储（`GetPriSyscapData`） |
| 并发约束 | 单线程；解析用 `strtok_r` 非重入版在自身拷贝上进行（`CopyInputString` 先复制输入） |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | 由 PCID/RPCID 二进制或 json 投影而来 |
| 下游影响 | 比较操作（CLI `-C` 与共享库 `ComparePcidString` 均以字符串为入口）；NAPI/Taihe 查询返回同构字符串 |
| 平级关联 | json 格式（同为中间形态） |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | 无独立结构，复用 `PCID_OUT_BUFFER[32]` u32 数组 + priSyscap 定长块 |
| 核心方法 | `SeparateSyscapFromString`/`GetPriSyscapData`/`CopyInputString`（`src/syscap_tool.c:594/557/531`）；`ParseStringSyscap`（`src/create_pcid.c:542`） |

## syscap_tool 命令会话（CLI）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | syscap_tool 命令行会话 |
| 实体定义 | 一次 CLI 调用的完整生命周期：参数解析 → 语义组合 → 单一操作分发 → 输出落盘 |
| 核心特征 | getopt_long 长短选项（`g_longOptions`，`src/main.c:51`）；选项语义由 `SetBitMap` 累积到 16 位 `bitMap`，`OperateByBitMap` 按组合魔数分发；版本常量 `SYSCAP_VERSION "2.0.1"` |
| 类型/分类 | 组合命令：0x109(-Rei)、0x10A(-Rdi)、0x10D(-Resi)、0x115(-Pesi)、0x111(-Pei)、0x112(-Pdi)、0x116(-Pdsi)、0x60(-C 文件比较)、0x64(-sC 字符串比较)、0x80(-v)；0x10E(-Rdsi) 显式不支持；其余落 help |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | `main()`：getcwd 取缺省输出目录 → getopt_long 循环调 `SetBitMap`（`-i` 记 inputfile、`-C` 记 pcid/rpcid 两文件、`-o` 改 outputpath）→ `OperateByBitMap` 分发 → 结果写 `outputpath/rpcid.sc` 等固定文件名 |
| 状态流转 | 全局 `g_customerfileinfo`（inputfile/pcidfile/rpcidfile）随参数解析填充，进程结束即销毁；`-C` 要求 argc==4 且 optind 合法 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | `-o` 缺省 CWD；输出文件名固定（rpcid.sc/pcid.sc/rpcid.json/pcid.json/rpcid.txt/pcid.txt）；比较结果与错误走 stdout（`PRINT_ERR` 带 `__FILE__:__LINE__`） |
| 并发约束 | 单进程单次会话；无持久状态 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | IDE/构建系统（generate_pcid）以命令行驱动 |
| 下游影响 | 调用 `src/syscap_tool.c`/`src/create_pcid.c` 全部核心函数 |
| 平级关联 | 共享库 API（同源不同包装） |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | 无；`g_longOptions`/`g_customerfileinfo`（`src/main.c:51/65`） |
| 核心方法 | `main()`（`src/main.c:71`）、`SetBitMap()`（:146）、`OperateByBitMap()`（:106）、`PrintHelp()`（:197） |

## 共享库 ABI（syscap_interface_shared / CompareError）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | 内部 API 共享库及其 ABI 面 |
| 实体定义 | 供其他 OHOS 部件动态链接的设备侧能力原语集合；bundle.json `inner_kits` 声明（头文件 `interfaces/inner_api/syscap_interface.h`） |
| 核心特征 | versionscript 白名单导出 7 符号：`EncodeOsSyscap`/`EncodePrivateSyscap`/`DecodeOsSyscap`/`DecodePrivateSyscap`/`DecodeRpcidToStringFormat`/`ComparePcidString`/`FreeCompareError`；错误码 `E_ERROR(-1)/E_OK(0)/E_APIVERSION(1)/E_SYSCAP(2)` 可按位组合 |
| 类型/分类 | 编码类（读 `/system/etc/pcid.sc`）/ 解码类（位图/尾串 → 字符串数组）/ 比较类（结构化结果） |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 编码：`EncodeOsSyscap(output,128)` 从 `PCID_PATH` 拷贝前 128 字节；`EncodePrivateSyscap` 拷尾串。解码：`DecodeOsSyscap(input[128], &out, &cnt)` 展开位图为字符串数组；`DecodePrivateSyscap` 拆逗号串补前缀。比较：`ComparePcidString(pcidStr, rpcidStr, &result)` 填 `CompareError`，调用方 `FreeCompareError` 释放 |
| 状态流转 | 无内部状态；每次调用独立。`CompareError{syscap[512], missSyscapNum, targetApiVersion}` 生命周期 = 调用→释放，missSyscapNum 记缺失数、targetApiVersion 记所需版本（仅 E_APIVERSION 时有意义） |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | `MAX_MISS_SYSCAP=512` 缺失上限（越界即失败释放）；输出数组由调用方 free；`PCID_MAIN_BYTES=128`/`PCID_MAIN_INTS=32` 固定 |
| 并发约束 | 无锁，纯函数式；多线程可并发调用（只读设备文件） |
| 安全约束 | `EncodePrivateSyscap` 校验 bufferLen ∈ (129, INT32_MAX]；`DecodeOsSyscap` 输出先 zero 再填；cfi+pac_ret 编译加固 |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | NAPI/Taihe 查询链直接调用（`napi/napi_query_syscap.cpp`、`taihe/syscap/src/ohos.systemCapability.impl.cpp`） |
| 下游影响 | 下游部件（如包管理相关）按 7 符号链接 |
| 平级关联 | CLI（同核不同壳）；versionscript 与头文件三处一致约束（见 [coding-standards.md](coding-standards.md)） |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | `CompareError`（`interfaces/inner_api/syscap_interface.h:40-44`） |
| 核心方法 | 7 个导出函数实现于 `interfaces/inner_api/syscap_interface.c`（`EncodeOsSyscap`:81、`DecodeOsSyscap`:157、`ComparePcidString`:655、`FreeCompareError`:697 等） |

## 设备能力查询 API（querySystemCapabilities）

### 实体概念
| 子维度 | 内容 |
|---|---|
| 实体名称 | 设备侧能力查询 API（NAPI `@ohos.systemCapability` 与 Taihe `querySystemCapabilitie`） |
| 实体定义 | 应用在设备上获取"当前系统支持哪些 SysCap"的异步接口；返回 pcid.txt 同构字符串 |
| 核心特征 | NAPI：`querySystemCapabilities()`，async work + promise/回调双形态（`napi/napi_query_syscap.cpp`），模块名 `systemCapability`，JS 桥 `napi/query_syscap.js`（`requireInternal`）。Taihe：IDL `taihe/syscap/idl/ohos.systemCapability.taihe`（`@gen_async`+`@gen_promise` 注解），生成 `.ani.cpp/.abi.c/@ohos.systemCapability.ets`→boot abc（`/system/framework/systemCapability.abc`） |
| 类型/分类 | 双通道：NAPI（旧 FA/JS）与 Taihe（新 ArkTS native） |

### 上下文与场景
| 子维度 | 内容 |
|---|---|
| 交互流程 | 两通道共同调 `GetSystemCapability()`（NAPI 侧 `napi_query_syscap.cpp:110`、Taihe 侧 impl.cpp 同名复刻）：`EncodeOsSyscap` 读 pcid 头 128B → 32 个 u32 转十进制字符串 → `EncodePrivateSyscap`+`DecodePrivateSyscap` 补全称 → `CalculateAllStringLength` 逗号拼接 → NAPI 经 async work 回调/promise 返回；Taihe 侧失败走 `taihe::set_business_error(-1, "key does not exist")` |
| 状态流转 | 无状态查询；NAPI 上下文 `SystemCapabilityAsyncContext` 随 async work 创建销毁 |

### 规格与约束
| 子维度 | 内容 |
|---|---|
| 业务规则 | 依赖设备存在 `/system/etc/pcid.sc`（缺失即报 "key does not exist"）；NAPI 参数 ≤ 1（回调可选） |
| 并发约束 | NAPI 重活在 async work 线程执行，主线程仅回调；Taihe 为同步实现包异步签名 |
| 安全约束 | Taihe IDL `@!sts_inject` 声明 `loadLibraryWithPermissionCheck`；ABC 为 boot abc |

### 知识关联
| 子维度 | 内容 |
|---|---|
| 上层依赖 | 共享库 7 符号中的 Encode/Decode 四个 |
| 下游影响 | 应用运行时能力判断 |
| 平级关联 | NAPI 与 Taihe 逻辑同构（`GetSystemCapability` 两份实现保持一致，改动须双侧同步） |

### 代码与符号
| 子维度 | 内容 |
|---|---|
| 核心类 | `SystemCapabilityAsyncContext`（NAPI 侧）；`g_systemCapabilityModule`（模块注册） |
| 核心方法 | `QuerySystemCapability`/`GetSystemCapability`/`CalculateAllStringLength`（`napi/napi_query_syscap.cpp:179/110/53`）；Taihe 侧 `querySystemCapabilitie()`（`taihe/syscap/src/ohos.systemCapability.impl.cpp:144`）与 `ANI_Constructor`（`ani_constructor.cpp:17`） |
