# 术语表

> 路径约定：源码路径均相对仓根 `developtools/syscap_codec`。数值与默认值均为当前代码实测。

## SysCap（SystemCapability / 系统能力）
OpenHarmony 设备能力的原子描述单位，字符串格式 `SystemCapability.XX.Yyy`。全 OS 权威清单在 `include/codec_config/syscap_define.h`：`SystemCapabilityNum` 枚举（386 项）与 `g_arraySyscap[]` 数组（`SyscapWithNum{str[273], num}`，386 项）1:1 对应，30 项已标 `// abandoned`。单条上限 `SINGLE_SYSCAP_LEN = 256 + 17 = 273` 字节（含 `SystemCapability.` 前缀 17 字节）。与"部件"区别：SysCap 是对应用暴露的能力面，部件是构建/裁剪单位（bundle.json `component.syscap` 声明部件提供哪些 SysCap）。

## OS SysCap / Private SysCap
两类 SysCap 的编码处理方式不同：**OS SysCap** 在主清单中有编号（num），编码进 pcid 的 120 字节位图（bit 位 = num），上限 960 个；**Private SysCap**（厂商私有）不占位图，以去前缀（去掉 `SystemCapability.`）逗号串形式追加在 PCIDMain 结构之后。区分逻辑：编码时查 `CreateWholeSyscapJsonObj()`（`src/context_tool.c`）构建的名称→编号映射，命中即 OS 类，未命中即 Private 类。

## PCID（Product Compatibility ID / `PCIDMain`）
设备能力标识。二进制结构（`include/create_pcid.h`）：位域头 `apiVersion:15 | apiVersionType:1(=0) | systemType:3 | reserved:13`（uint16×2 字）+ `manufacturerID`(uint32) + `osSyscap[120]` 位图 = 128 字节，随后为 private syscap 逗号尾串。`systemType` 编码：mini=0b001、small=0b010、standard=0b100。多字节字段大端。文件名 `pcid.sc`；字符串形式 `pcid.txt`。设备上位于 `/system/etc/pcid.sc`（`syscap_interface.c` 的 `PCID_PATH`）。

## RPCID（Required Product Compatibility ID / `RPCIDHead`）
应用所需能力标识。二进制结构（`include/context_tool.h`）：`RPCIDHead{apiVersion:15, apiVersionType:1(=1)}` + `sysCaptype`(uint16，=2 表示 app syscap) + `sysCapLength`(uint16，字节数) + N 条 256 字节定长字符串（去前缀，不足补 `\0`）。由 IDE 依据应用配置生成 rpcid.json 后编码。`apiVersionType` 是 PCID(0)/RPCID(1) 的判别位。

## 字符串格式（string format / pcid.txt、rpcid.txt）
`.sc` 的文本形态：32 个 uint32 十进制逗号连接（前 2 个 u32 即二进制头 8 字节，后 30 个 u32 即 120 字节 OS 位图），再接 private syscap 全称逗号列表。应用市场/包管理器与设备 NAPI 查询返回的都是该格式。CLI 用 `-s` 选项指示字符串形态输入/输出（如 `-Pdsi`、`-sC`）。

## apiVersionType
PCID/RPCID 头中的 1 bit 判别位：pcid 恒为 0（`SetPCIDHeader`），rpcid 恒为 1（`FillOsCapLength`）；解码侧据此校验（`DecodePCID` 拒绝非 0，`CheckRpcidFormat` 拒绝非 1）。

## 比较结果（CompareError / E_APIVERSION / E_SYSCAP）
`ComparePcidString`（共享库）的结构化输出与返回码：`CompareError{syscap[512], missSyscapNum, targetApiVersion}`；返回码 `E_ERROR(-1)` 失败、`E_OK(0)` 满足、`E_APIVERSION(1)` 设备 API 版本不足（`targetApiVersion` 给出所需版本）、`E_SYSCAP(2)` 缺能力、二者可按位组合。CLI 侧 `-C/-sC` 输出人读文本（`Missing: <syscap>` 逐行 + 总结行）。

## syscap_tool（CLI）
本仓主产物（目标 `syscap_tool_bin`，输出名 `syscap_tool`），版本常量 `SYSCAP_VERSION "2.0.1"`（`src/main.c`）。命令形态 `syscap_tool -R/P -e/d -i filepath [-o outpath]`，参数组合由 `SetBitMap` 累积位图、`OperateByBitMap` 按魔数分发（如 0x109=-Rei、0x111=-Pei、0x60=-C、0x64=-sC）。仅供 IDE/构建系统集成，对最终用户不可见。

## versionscript（版本脚本 / ABI 契约）
`libsyscap_interface_shared.versionscript`：`global:` 段白名单导出 7 个符号，`local: *` 隐藏其余。是 `syscap_interface_shared` 共享库的 ABI 契约，破坏即下游动态链接失败。Taihe 侧另有 `taihe/syscap/libsystemCapability_taihe_native.versionscript`。

## NAPI 模块（systemcapability）
`napi/` 目录产物（目标 `napi:systemcapability`，`relative_install_dir = "module"`）：`napi_query_syscap.cpp` 注册 JS 方法 `querySystemCapabilities`（async work 异步、promise/回调双形态），`query_syscap.js` 经 `requireInternal('systemCapability')` 转发给 `@ohos.systemCapability`。仅 `support_jsapi && is_standard_system` 时构建。

## Taihe
新式 IDL 驱动 API 框架：`taihe/syscap/idl/ohos.systemCapability.taihe` 为唯一源（注解 `@gen_async`/`@gen_promise`/`@!sts_inject` 控制生成形状），构建期 `ohos_taihe` 生成 `ohos.systemCapability.ani.cpp`/`ohos.systemCapability.abi.c`/`@ohos.systemCapability.ets`（再编为 boot abc 装到 `/system/framework/systemCapability.abc`）；手写 `src/ohos.systemCapability.impl.cpp` 实现 `querySystemCapabilitie`，`ani_constructor.cpp` 做 ANI 运行时注册。生成物在 `$taihe_file_path/out/developtools/syscap_codec/` 下，不入版本管理。

## syscap_define_custom.h（自定义扩展清单）
构建参数 `syscap_codec_config_extern_path`（`config.gni`）非空时，GN action `gen_syscap_define_custom` 调 `tools/syscap_config_merge.py` 把仓内主清单与外部头合并的产物（`${root_build_dir}/syscap_define_custom.h`），并定义 `-DSYSCAP_DEFINE_EXTERN_ENABLE` 使 `src/*.c` 改 include 它。外部扩展枚举首项须标记 ≥ 500 的最小值。

## generate_pcid（构建期 pcid 生成）
`BUILD.gn` 的 `build_ext_component("generate_pcid")`：依赖 host clang 工具链编译的 `syscap_tool`（`syscap_tool_bin_linux` group），对 preloader 输出的 `${preload_path}/system/etc/SystemCapability.json` 执行 `-P -e` 编码为 `$root_out_dir/pcid.sc`；再由 `ohos_prebuilt_etc("pcid.sc")` 装入镜像 `/system/etc/pcid.sc`（lite 分支额外拷贝到 `$exec_path/system/etc/`）。

## 一致性检查（syscap_check.py）
独立 Python 工具（依赖 prettytable==3.3.0），三种检查目标：`component_codec`（部件 bundle.json `component.syscap` ↔ syscap_define.h）、`component_sdk`（部件 ↔ `interface/sdk-js/api/*.d.ts` 的 `@syscap`）、`sdk_codec`（d.ts ↔ syscap_define.h），输出 PrettyTable 差异报告。`-b` 可指定 bundle（仅 component_codec 生效）。

## syscap_collector.py（产品 SysCap 收集）
独立 Python 工具：从源码树的产品部件组成（parts_info）+ 各部件 bundle.json syscap 字段（`=false` 的排除）收集每个产品的 SysCap 集合，输出 `{product}.json`（`{"SysCaps": [...]}`），重复项去重并告警。

## ohos_lite 分支
本仓对小型/轻量系统的适配：liteos_m 上 `syscap_interface_shared` 退化为空 group（无动态库）、CLI 加 `PATH_MAX=1024` 编译宏；liteos_a 用原生 `shared_library`（`-rdynamic` + `--version-script`）替代 `ohos_shared_library`；cJSON 依赖改走 `//build/lite/config/component/cJSON`。bundle.json `adapted_system_type: ["small", "standard"]`。
