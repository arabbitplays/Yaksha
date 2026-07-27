#include "../include/app/DesktopManager.hpp"
#include <memory>

#include "configuration/LoggingConfiguration.hpp"


int main(int argc, char* argv[])
{
    bool dev_mode = false;
    if (argc == 2 && std::string(argv[1]) == "--dev")
    {
        dev_mode = true;
    }

    LoggingConfiguration::configure(dev_mode);
    std::shared_ptr<DesktopManager> app;
    try
    {
        app = std::make_shared<DesktopManager>(dev_mode);
        app->initApp();
    } catch (std::exception& e)
    {
        Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<DesktopManager>();
        logger->error("Failed to initialize app: " + std::string(e.what()));
        exit(EXIT_FAILURE);
    }

    try
    {
        app->run();
    } catch (std::exception& e) {
        Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<DesktopManager>();
        logger->error("Unhandled exception while running app: " + std::string(e.what()));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
