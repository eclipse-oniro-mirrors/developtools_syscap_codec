# 业务背景

> 路径约定：源码路径均相对仓根 `developtools/syscap_codec`（`@ohos/syscap_codec`，部件 `syscap_codec`，子系统 `developtools`，版本 4.0.2）。

## 应用定位

### 核心职责

系统能力（SystemCapability，下称 SysCap）编解码工具，解决"设备能力与应用需求如何用紧凑、可比较的格式表达"的问题。核心职责：

1. **pcid/rpcid 编码**：把描述 SysCap 集合的 JSON 文件编码为二进制 `.sc` 格式（`RPCIDEncode`/`CreatePCID`，`src/syscap_tool.c`、`src/create_pcid.c`）。
2. **pcid/rpcid 解码**：把 `.sc` 二进制还原为 JSON（`RPCIDDecode`/`DecodePCID`）。
3. **字符串格式转换**：`.sc` ↔ 逗号分隔的字符串格式（`EncodeRpcidscToString`/`EncodePcidscToString`/`DecodeStringPCIDToJson`），便于应用市场/包管理器在文本通道传输。
4. **pcid 与 rpcid 比较**：判定设备能力是否满足应用要求，输出缺失项（`ComparePcidWithRpcidString`、共享库 `ComparePcidString`）。
5. **SysCap 主清单维护**：`include/codec_config/syscap_define.h` 是全 OS SysCap 的编号权威源（386 项，枚举与数组 1:1 对应），并支持厂商外部扩展（`config.gni` + `tools/syscap_config_merge.py`）。
6. **设备侧能力查询**：设备上通过 `@ohos.systemCapability` JS API（`napi/`）与 Taihe/ArkTS native API（`taihe/syscap/`）查询当前系统能力串。

四类产物共享同一套 C 核心源码（`src/*.c`）：

| 产物 | 目标 | 使用方 |
|---|---|---|
| `syscap_tool` CLI | `:syscap_tool_bin` | IDE（rpcid 生成/pcid 解码，对用户不可见）、构建系统（构建期生成 pcid.sc） |
| `libsyscap_interface_shared.z.so` | `:syscap_interface_shared` | 其他 OHOS 部件（inner_kits，7 个导出符号） |
| `libsystemcapability.z.so`（NAPI 模块） | `napi:systemcapability` | 设备侧 JS 应用 `@ohos.systemCapability` |
| `libsystemCapability_taihe_native.z.so` + `systemCapability.abc` | `taihe/syscap:systemCapability_taihe_native` | 新式 Taihe/ArkTS native API（`.taihe` IDL 驱动） |

### 职责边界（本组件不负责）

| 不负责事项 | 承担方 | 依据 |
|---|---|---|
| rpcid.json 的生成（按应用配置与 API 版本） | IDE | README_ZH.md：IDE 根据应用 SysCap 配置生成 rpcid json 再调用本工具 |
| 设备侧 SysCap 集合的内容定义 | preloader 生成的 `out/preloader/{product}/system/etc/SystemCapability.json` | `BUILD.gn` 的 `generate_pcid` 只读取该文件编码 |
| SysCap 在部件上的声明 | 各部件 `bundle.json` 的 `component.syscap` 字段 | 本仓 `tools/syscap_check.py` 只做一致性比对 |
| JS/ETS API 的 d.ts 声明与 SDK 分发 | `interface/sdk-js/api` | syscap_check.py 的 component_sdk/sdk_codec 检查对象 |
| 应用分发时选择设备 | 应用市场/包管理器 | 本仓只提供比较原语（ComparePcidString 等） |
| SysCap 运行时拦截（应用调用不支持 API 的行为管控） | 各运行时 | 本仓只提供能力查询，不执行管控 |

## 系统上下文

```plantuml
@startuml
skinparam componentStyle rectangle
left to right direction

actor "IDE" as ide
actor "应用市场 / 包管理器" as market
actor "JS/ArkTS 应用" as app
participant "构建系统" as build

package "syscap_codec 仓" {
  component "syscap_tool CLI\n(src/main.c + src/*.c)" as cli
  component "C 核心编解码\n(syscap_tool.c / create_pcid.c\ncontext_tool.c / endian_internal.c / common_method.c)" as core
  component "syscap_interface_shared\n(interfaces/inner_api/)" as sosh
  component "NAPI 模块\n(napi/napi_query_syscap.cpp)" as napi
  component "Taihe native\n(taihe/syscap/)" as taihe
  component "syscap_define.h\n(386 项 SysCap 主清单)" as define
}

build -> cli : 构建期 generate_pcid\n对 SystemCapability.json 编码
cli -> core
build <- cli : pcid.sc → /system/etc/pcid.sc
ide -> cli : rpcid.json → rpcid.sc\npcid.sc → pcid.json
market -> cli : -C 比较 pcid/rpcid\n(或调共享库 ComparePcidString)
market -> sosh : ComparePcidString 等 7 符号
sosh -> core
app -> napi : @ohos.systemCapability\n.querySystemCapabilities()
napi -> sosh : EncodeOsSyscap / EncodePrivateSyscap\n读 /system/etc/pcid.sc
app -> taihe : querySystemCapabilitie()\n(.abc → native .so)
taihe -> core
cli -> define
core -> define
@enduml
```

## 核心输入

| 输入 | 来源 | 主要内容 | 触发时机 |
|---|---|---|---|
| rpcid json | IDE 生成 | `api_version`（数字）+ `syscap`（字符串数组，`SystemCapability.XX.Yyy` 全称） | `syscap_tool -Rei`（RPCIDEncode） |
| pcid json | 构建产物/手工构造 | `api_version`、`system_type`（mini/small/standard）、`manufacturer_id`、`syscap.os`（数组）、`syscap.private`（可选数组） | `syscap_tool -Pei`（CreatePCID） |
| rpcid.sc / pcid.sc | 本工具编码输出 | 二进制格式（见 [architecture.md](architecture.md)） | 解码/字符串化/比较命令 |
| 字符串格式 pcid/rpcid | 应用市场等文本通道 | 32 个 uint32 逗号连接 + private syscap 列表 | `-C`/`-sC` 比较、`-Pdsi` 解码 |
| 设备 pcid.sc | `/system/etc/pcid.sc`（构建期 generate_pcid 生成并预装） | 128 字节 PCIDMain + private syscap 尾串 | 设备侧共享库 EncodeOsSyscap/EncodePrivateSyscap（NAPI/Taihe 查询） |
| SystemCapability.json | 构建系统 preloader 输出（`${preload_path}/system/etc/`） | 产品 SysCap 集合 | 构建期 `build_ext_component("generate_pcid")` |
| 外部 syscap 扩展头 | `syscap_codec_config_extern_path` 构建参数指定 | 厂商自定义 SysCap 枚举+数组（编号须 ≥ 500） | GN 生成期触发 `tools/syscap_config_merge.py` |
| 部件 bundle.json / sdk d.ts | 全源码树 | `component.syscap` 字段 / `@syscap` 注解 | `tools/syscap_check.py` 一致性检查（独立运行） |

## 核心输出

| 输出 | 位置 | 接收方 | 业务作用 |
|---|---|---|---|
| rpcid.sc / pcid.sc | `-o` 指定目录（缺省 CWD），文件名固定 | IDE / 应用市场 | 紧凑二进制能力标识（rpcid.sc 为定长记录；pcid.sc 为位图+尾串） |
| rpcid.json / pcid.json | 同上 | IDE / 开发者 | 人可读的能力集合还原 |
| rpcid.txt / pcid.txt | 同上 | 应用市场/包管理器 | 字符串格式（32×u32 + private 列表，逗号分隔） |
| 比较结果 | stdout | 调用方 | `Succeed! The pcid meets the rpcid.` 或逐项 `Missing: <syscap>` + 版本不足信息 |
| 设备能力串 | NAPI/Taihe 查询返回的字符串（同 pcid.txt 格式） | JS/ArkTS 应用 | 运行时判断设备是否支持某能力 |
| pcid.sc（镜像内） | `/system/etc/pcid.sc`（`ohos_prebuilt_etc("pcid.sc")` 安装） | 设备侧共享库 | 设备自我描述的权威数据源 |
| syscap_define_custom.h | `${root_build_dir}/syscap_define_custom.h`（生成物） | 本仓 4 个编译目标 | 厂商扩展后的合并 SysCap 清单 |
| 一致性检查报告 | stdout（PrettyTable） | CI/维护者 | 部件 syscap ↔ syscap_define.h ↔ sdk d.ts 三方一致性 |
| 产品 SysCaps json | `syscap_collector.py` 输出 `{product}.json` | 产品配置侧 | 各产品实际包含的 SysCap 集合清单 |
