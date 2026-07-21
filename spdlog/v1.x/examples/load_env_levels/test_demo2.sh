
echo 'SPDLOG_LEVEL="logger1=debug,logger2=trace,logger3=error"'
# 格式：SPDLOG_LEVEL=logger1=debug,logger2=trace,logger3=error
export SPDLOG_LEVEL="logger1=debug,logger2=trace,logger3=error"
./demo2

# 全局级别 + 特定logger级别
echo 'SPDLOG_LEVEL="info,logger1=debug,logger3=trace"'
export SPDLOG_LEVEL="info,logger1=debug,logger3=trace"
./demo2
