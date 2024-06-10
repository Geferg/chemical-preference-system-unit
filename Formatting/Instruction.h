#ifndef TCPDEBUGCLEAN_INSTRUCTION_H
#define TCPDEBUGCLEAN_INSTRUCTION_H

#include <vector>
#include "Argument.h"
#include "Enums.h"

namespace Formatting {

    class Instruction {
    private:
        int underlyingType;

    public:
        enum Status {
            Processing = 102,
            OK = 200,
            Bad = 400
        };

        Status CurrentStatus;
        std::vector<Argument> Arguments;

        [[nodiscard]] InstructionType GetType() const;
        void SetType(InstructionType type);
        [[nodiscard]] std::string GetInfo(bool indent) const;

        friend void to_json(nlohmann::json& j, const Instruction& instr);
        friend void from_json(const nlohmann::json& j, Instruction& instr);

        friend std::ostream& operator<<(std::ostream& os, const Instruction& obj);

    };

}

#endif
