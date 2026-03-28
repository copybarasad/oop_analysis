#include "GameLoop.h"
#include "ConsoleLogSink.h"
#include "FileLogSink.h"

#include <memory>
#include <string>

int main(int argc, char* argv[]) {
    std::unique_ptr<LogSink> sink(new ConsoleLogSink());
    std::string controls_path = "controls.txt";

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg.rfind("--log=file:", 0) == 0) {
            sink.reset(new FileLogSink(arg.substr(std::string("--log=file:").size())));
        } else if (arg == "--log=console") {
            sink.reset(new ConsoleLogSink());
        } else if (arg.rfind("--controls=", 0) == 0) {
            controls_path = arg.substr(std::string("--controls=").size());
        }
    }

    GameLoop loop(std::move(sink), controls_path);
    loop.Run();
    return 0;
}
