#ifndef GIT_CONTROLLER
#define GIT_CONTROLLER

#include "controller/IController.hpp"

class GitController : public IController {
public:
    GitController() = default;
    ~GitController() = default;

    std::string getKeyword() const override;
    std::string execute(io::CommandHandle &cmd) override;

private:
    std::string sync();
    std::string syncRepo(const std::string& label, const std::string& git_prefix);
    std::string syncDotfiles();

    static std::string currentDate();
    static std::string home();
};

#endif // GIT_CONTROLLER
