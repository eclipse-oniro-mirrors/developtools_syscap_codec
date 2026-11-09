# 编码铁律

> 路径约定：源码路径均相对仓根 `developtools/syscap_codec`。依据：仓内 `AGENTS.md`「禁止事项」「syscap_define.h 规则」「验证」与源码实现。

## SysCap 主清单（syscap_define.h）

- **禁止**删除或重排 `include/codec_config/syscap_define.h` 中 `SystemCapabilityNum` 枚举值与 `g_arraySyscap` 数组项。新增 SysCap **必须**同时在枚举和数组**末尾**追加并保持 1:1 升序对应；废弃某项只在枚举后注释 `// abandoned`（当前 386 项中 30 项已废弃）。编号是 pcid 位图的 bit 位与跨仓引用键，重排即旧数据解析错位。
- 厂商扩展**必须**走 `syscap_codec_config_extern_path` 构建参数（编号 ≥ 500，由 `tools/syscap_config_merge.py` 合并生成 `syscap_define_custom.h`），**禁止**直接改仓内主清单塞入厂商项。生成的 `syscap_define_custom.h` 是构建产物，**禁止**手改或提交。

## ABI 与对外接口

- **禁止**修改 `libsyscap_interface_shared.versionscript` 中已发布的 7 个符号（ComparePcidString/DecodeOsSyscap/DecodePrivateSyscap/EncodeOsSyscap/EncodePrivateSyscap/DecodeRpcidToStringFormat/FreeCompareError），**禁止**删除或改名。新增导出函数须同步：`interfaces/inner_api/syscap_interface.h` 签名 → `syscap_interface.c` 实现 → versionscript global 段，**三文件必须一致**（AGENTS.md 规则）。
- **禁止**修改 `syscap_interface.h` 中已发布函数的参数类型、个数与返回值（只可新增函数）。返回码语义（E_ERROR=-1/E_OK=0/E_APIVERSION=1/E_SYSCAP=2，可按位组合）是对外契约，改动前须确认所有调用方兼容。
- **禁止**手改 Taihe 生成物（`*ani.cpp`、`*abi.c`、`@ohos.*.ets`）。改 Taihe API 只改 `taihe/syscap/idl/ohos.systemCapability.taihe`（IDL 源）与 `taihe/syscap/src/` 下手写实现。

## 编码格式与核心逻辑

- **禁止**变更二进制格式：`PCIDMain` 位域布局（`include/create_pcid.h`）、`RPCIDHead`、rpcid 定长记录（SINGLE_FEAT_LEN=256、sysCaptype=2）、大端字节序。`.sc` 是 IDE/应用市场/设备间的跨版本契约。
- **禁止**绕过 `endian_internal.h` 的 `HtonsInter/NtohsInter/HtonlInter/NtohlInter` 直接写多字节字段——三平台字节级一致依赖此封装（`src/endian_internal.c` 运行时检测本机序）。
- 修改 `src/` 下任何文件前**必须**确认对全部 4 个目标（CLI/共享库/NAPI/Taihe）的影响——同一套源文件被重复编译（`BUILD.gn:27-33` 的 `sources_platform_common`）。
- OS SysCap 数量硬上限 960（120 字节 × 8 bit，`SetOsSyscap`/`SetOsSysCapBitMap` 校验 sector < 120）；`EncodePcidscToString` 限制 pcid.sc ≤ 1128 字节；字符串输入上限 1024（`CopyInputString`）。新增能力不得突破这些边界。

## 安全编码（C/C++）

- **禁止**使用裸 `memcpy/strcpy/sprintf/strcat/strncpy/sscanf`——全仓统一 securec 系列（`memcpy_s/strcpy_s/sprintf_s/strcat_s/strncpy_s/strncat_s/sscanf_s`），返回值必须检查（EOK）。
- **禁止**在未校验长度前读取输入缓冲：rpcid 二进制先过 `ValidateRpcidInput`（`src/common_method.c`），文件先过 `GetFileContext` 的路径/权限/大小三重校验（`src/context_tool.c`）。
- 共享库类目标**必须**保持 `sanitize = { cfi = true, cfi_cross_dso = true, debug = false }` 与 `branch_protector_ret = "pac_ret"`（既有三目标均已开启，新目标对齐）。
- 输出缓冲分配后**必须** `memset_s` 清零再 `sprintf_s` 追加（既有模式，防止残留数据泄漏进产物）。

## 错误处理与资源释放

- 错误输出统一用 `PRINT_ERR` 宏（`include/context_tool.h`，自动带 `__FILE__:__LINE__`）；CLI 侧修改错误返回码前**必须**确认调用方（IDE 脚本）兼容。
- 每个编解码函数按既有分层 free 模式（`FREE_*_AFTER_*` 宏族）逐级释放：新增分支时**必须**把已分配资源挂入对应层级，**禁止**裸 return 跳过释放。
- `ComparePcidString` 的 `CompareError` 结果由调用方 `FreeCompareError` 释放；新增输出指针字段须同步释放路径并防越界（既有 `MAX_MISS_SYSCAP=512` 上限检查模式）。

## 构建与依赖

- **禁止**脱离 OHOS 源码树构建本仓（GN 目标 import `//build/ohos.gni` 等）。改 `BUILD.gn`/`config.gni` 后在源码树根跑 `hb build --build-only-gn` 确认生成不报错。
- 新增依赖须同步 `bundle.json` 的 `deps.components` 与 `BUILD.gn` external_deps；三方依赖（cJSON/bounds_checking_function/googletest）变更前检查 `OAT.xml` 白名单覆盖，未覆盖先更新 OAT 再引入。
- 新增平台条件编译对照 `BUILD.gn:47-53` 既有分支（`is_mingw` → `_POSIX_`；`ohos_lite` + liteos_m → `PATH_MAX=1024`、`-Wno-int-conversion`）。
- Taihe/NAPI 目标只在 `support_jsapi`（及标准系统）下构建，**禁止**假设其总是存在。

## 验证（改动后必跑）

- 语法：改动的 `.c/.h` 跑 `gcc -fsyntax-only -I include -I interfaces/inner_api <file>`；`.py` 跑 `python3 -m py_compile`。
- 单元测试：`ninja -C out/{board} developtools/syscap_codec/test/unittest/common:unittest` 后运行产物（标准系统目标 `syscap_codec_test`，881 行 gtest 用例覆盖 encode/decode/compare/endian/边界）。
- `syscap_define.h` 改动：`python3 tools/syscap_check.py -p <ohos_root> -t component_codec`。
- ABI 改动：确认 versionscript 已发布符号未变，编译 `:syscap_interface_shared` 并跑 unittest。
- CLI 行为改动：同步更新 `src/main.c` 的 `PrintHelp()`；手工跑 `-h`/`-v` 及对应命令组合。
