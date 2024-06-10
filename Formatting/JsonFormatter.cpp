#include "JsonFormatter.h"
#include "nlohmann/json.hpp"

namespace Formatting {
    Instruction JsonFormatter::GetInstruction(const char* receivedBytes) {
        nlohmann::json j = nlohmann::json::parse(receivedBytes);
        Instruction instruction = j.get<Instruction>();

        return instruction;
    }

    Protocol JsonFormatter::GetProtocol(const char *receivedBytes) {
        nlohmann::json j = nlohmann::json::parse(receivedBytes);
        Protocol protocol = j.get<Protocol>();

        return protocol;
    }

    std::string JsonFormatter::SerializeInstruction(const Instruction &instruction) {
        nlohmann::json j;
        to_json(j, instruction);
        return j.dump();
    }

    std::string JsonFormatter::SerializeProtocol(const Protocol &protocol) {
        nlohmann::json j;
        to_json(j, protocol);
        return j.dump() + "\n";
    }


}