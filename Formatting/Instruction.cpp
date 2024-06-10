#include "Instruction.h"
#include <iostream>
#include <vector>

namespace Formatting {
    std::ostream &operator<<(std::ostream& os, const Instruction& obj) {
        os << obj.GetInfo(true);
        return os;
    }

    InstructionType Instruction::GetType() const {
        return static_cast<InstructionType>(underlyingType);
    }

    void Instruction::SetType(InstructionType type) {
        underlyingType = static_cast<int>(type);
    }

    std::string Instruction::GetInfo(bool indent) const {
        std::string type;
        std::string result;

        switch (GetType()) {
            case Formatting::InstructionType::Comment:
                type = "Comment";
                break;

            case Formatting::InstructionType::SwitchIllumination:
                type = "SwitchIllumination";
                break;

            case Formatting::InstructionType::SwitchValve:
                type = "SwitchValve";
                break;

            case Formatting::InstructionType::RunMotor:
                type = "RunMotor";
                break;

            case Formatting::InstructionType::StopMotor:
                type = "StopMotor";
                break;

            case Formatting::InstructionType::Wait:
                type = "Wait";
                break;

            case Formatting::InstructionType::Calibrate:
                type = "Calibrate";
                break;

            case Formatting::InstructionType::Status:
                type = "Status";
                break;

            default:
                type = "Unknown";
                break;
        }

        if (indent) {
            result = "\t";
        }
        else {
            result = "";
        }

        result += "[" + std::to_string((int)CurrentStatus) + "] " + type + "(";

        for(auto it = Arguments.begin(); it != Arguments.end(); ++it) {
            //os << "[" << it->Id << "]";

            result += it->Name + ": " + it->Value;
            if(it != Arguments.end() - 1) {
                result += ", ";
            }
        }

        result += ");";
        return result;
    }


}

