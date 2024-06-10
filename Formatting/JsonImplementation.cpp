#include <nlohmann/json.hpp>
#include "Argument.h"
#include "Instruction.h"
#include "Protocol.h"

using nlohmann::json;

namespace Formatting {

    // Argument
    void to_json(json &j, const Argument &a) {
        j = json{{"Id", a.Id}, {"Name", a.Name}, {"Value", a.Value}};
    }

    void from_json(const json &j, Argument &a) {
        a.Id = j.at("Id").get<int>();
        a.Name = j.at("Name").get<std::string>();
        a.Value = j.at("Value").get<std::string>();
    }

    // vector<Argument>
    void to_json(json& j, const std::vector<Argument>& vec) {
        j = json::array();
        for (const auto& arg : vec) {
            json arg_json;
            to_json(arg_json, arg);
            j.push_back(arg_json);
        }
    }

    void from_json(const json& j, std::vector<Argument>& vec) {
        vec.clear();
        for (const auto& item : j) {
            Argument arg;
            from_json(item, arg);
            vec.push_back(arg);
        }
    }

    // Instruction
    void to_json(json &j, const Instruction &instr) {
        j = json{
            {"Type", instr.GetType()},
            {"Arguments", instr.Arguments},
            {"CurrentStatus", instr.CurrentStatus}
        };
    }

    void from_json(const json &j, Instruction &instr) {
        instr.SetType(j.at("Type").get<InstructionType>());
        instr.Arguments = j.at("Arguments").get<std::vector<Argument>>();
        instr.CurrentStatus = j.at("CurrentStatus").get<Formatting::Instruction::Status>();
    }

    // vector<Instruction>
    void to_json(json &j, const std::vector<Instruction> &vec) {
        j = json::array();
        for (const auto &instr: vec) {
            json instr_json;
            to_json(instr_json, instr);
            j.push_back(instr_json);
        }
    }

    void from_json(const json &j, std::vector<Instruction> &vec) {
        vec.clear();
        for (const auto &item: j) {
            Instruction instr;
            from_json(item, instr);
            vec.push_back(instr);
        }
    }

    // Protocol
    void to_json(json &j, const Protocol &protocol) {
        j = json{
            {"Name", protocol.Name},
            {"Comment", protocol.Comment},
            {"Instructions", protocol.Instructions}
        };
    }

    void from_json(const json &j, Protocol &protocol) {
        protocol.Name = j.at("Name").get<std::string>();
        protocol.Comment = j.at("Comment").get<std::string>();
        protocol.Instructions = j.at("Instructions").get<std::vector<Instruction>>();
    }


}