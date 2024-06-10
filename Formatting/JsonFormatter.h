#ifndef TCPDEBUGCLEAN_JSONFORMATTER_H
#define TCPDEBUGCLEAN_JSONFORMATTER_H

#include "Instruction.h"
#include "nlohmann/json.hpp"
#include "Protocol.h"

namespace Formatting {

    class JsonFormatter {
    public:
        static Instruction GetInstruction(const char* receivedBytes);
        static Protocol GetProtocol(const char* receivedBytes);

        static std::string SerializeInstruction(const Instruction& instruction);
        static std::string SerializeProtocol(const Protocol& protocol);

    private:

    };

}

#endif
