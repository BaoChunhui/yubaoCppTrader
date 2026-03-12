# 最小闭环示例：读配置 → 初始化 Logger → 打日志


## 流程

1. **读配置**：`YCTLogger::init(logCfgFile)` 内部通过 `YCTCfgLoader::load_from_file()` 读取 YAML/JSON。
2. **初始化 Logger**：根据配置创建 root logger（级别、文件/控制台 sink）。
3. **打日志**：`YCTLogger::debug/info/warn/error(...)`。

## 编译与运行

在 `samples/logcfg_demo` 目录下：

```bash
cd samples/logcfg_demo
mkdir -p build && cd build
cmake ..
make
```

回到 `samples/logcfg_demo` 运行（需与 `logcfg.yaml` 同目录）：

```bash
cd .. && ./build/logcfg_demo
```

或指定配置文件路径：`./build/logcfg_demo /path/to/logcfg.yaml`。  
日志会按配置写入 `Logs/logcfg_demo.log` 并同时输出到控制台。

**依赖**：CMake 3.14+、C++17、Boost（filesystem）、spdlog。rapidjson 由 CMake 自动 FetchContent 下载；yaml-cpp 使用仓库内 `src/YCTUtils/yamlcpp`。
