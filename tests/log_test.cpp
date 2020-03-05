#include <spdlog/spdlog.h>
#include <memory>

int main(int argc, char* argv[]) {
    //std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("main_logger");
    //logger->info("JOB QUEUE FULL.");
    //logger->error("error");
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
}