#include "DesktopManager.hpp"
#include "io/CommandParser.hpp"
#include <memory>

#include "configuration/LoggingConfiguration.hpp"


int main(int argc, char* argv[]) {
    bool dev_mode = false;
    if (argc == 2 && std::string(argv[1]) == "--dev") {
        dev_mode = true;
    }

    LoggingConfiguration::configure();
    std::shared_ptr<DesktopManager> app = std::make_shared<DesktopManager>(dev_mode);
    app->run();
}
