#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "logging/stdout_logger.h"

int main(int argc, char* argv[]) {
    stdout_logger *logger = new stdout_logger();
    logger->INFO("info log");
    logger->INFO("info log");

    std::this_thread::sleep_for (std::chrono::seconds(2));
}