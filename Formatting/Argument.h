#ifndef TCPDEBUGCLEAN_ARGUMENT_H
#define TCPDEBUGCLEAN_ARGUMENT_H

#include <string>
#include "nlohmann/json.hpp"

namespace Formatting {
    class Argument {
    public:
        int Id;
        std::string Name;
        std::string Value;
    };
}

#endif
