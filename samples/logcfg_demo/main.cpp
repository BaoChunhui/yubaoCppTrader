/*!
 * \file main.cpp
 * \project yubaoCppTrader
 *
 * \brief 最小闭环示例：读配置 → 初始化 Logger → 打几条日志
 *
 * 用法：运行前确保 logcfg.yaml 与可执行文件同目录（或指定路径）。
 * 编译时需 -I 指向 yubaoCppTrader/src，并链接 YCTUtils、YCTTools 及依赖。
 */
#include "YCTTools/YCTLogger.h"
#include "YCTUtils/YCTCfgLoader.h"
#include "Includes/YCTVariant.hpp"
#include "Share/StdUtils.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[])
{
	// 1) 确定日志配置文件路径（可选：从命令行或默认）
	const char* logCfgFile = (argc > 1) ? argv[1] : "logcfg.yaml";
	if (!StdFile::exists(logCfgFile))
	{
		std::cerr << "log config not found: " << logCfgFile
			<< ", logger will use fallback stdout only." << std::endl;
	}

	// 2) 初始化 Logger（内部会读 logCfgFile：YCTCfgLoader::load_from_file）
	YCTLogger::init(logCfgFile);

	// 3) 打几条日志（级别由 logcfg 中 root.level 控制）
	YCTLogger::debug("logcfg_demo started, level=debug");
	YCTLogger::info("Hello from yubaoCppTrader minimal loop: config + logger");
	YCTLogger::info("log config file: {}", logCfgFile);
	YCTLogger::warn("this is a warning line");
	YCTLogger::error("this is an error line (demo only)");

	// 可选：演示「先读业务配置，再根据配置初始化 Logger」的模式（与 WonderTrader TestTrader 一致）
	// YCTVariant* appCfg = YCTCfgLoader::load_from_file("config.yaml");
	// if (appCfg) {
	//     const char* logPath = appCfg->getCString("logcfg");
	//     if (logPath) YCTLogger::init(logPath);
	//     // ... 其他业务配置 ...
	//     appCfg->release();
	// }

	return 0;
}
