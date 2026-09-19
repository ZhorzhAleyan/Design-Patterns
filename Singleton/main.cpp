#include "Logger.h"

void doWork() {
    // Same instance is reachable from anywhere, no need to pass it around.
    Logger::instance().log(Logger::Level::INFO, "doWork() started");
    Logger::instance().log(Logger::Level::WARNING, "doWork() found something odd");
}

int main() {
    Logger& logger = Logger::instance();
    logger.log(Logger::Level::INFO, "Application starting");

    doWork();

    logger.log(Logger::Level::ERROR, "Something went wrong");

    std::cout << "Total messages logged: " << Logger::instance().messageCount() << std::endl;

    return 0;
}
