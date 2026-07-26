#ifndef ICONTROLLER
#define ICONTROLLER

#include "io/CommandParser.hpp"

class IController {
public:
    IController() = default;
    virtual ~IController() = default;

    virtual std::string execute(io::CommandHandle& cmd) = 0;
    [[nodiscard]] virtual std::string getKeyword() const = 0;
private:
};

#endif // ICONTROLLER

