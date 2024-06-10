#ifndef TCPDEBUGCLEAN_PROTOCOL_H
#define TCPDEBUGCLEAN_PROTOCOL_H

#include <vector>
#include "Instruction.h"

namespace Formatting {

    class Protocol {
    private:


    public:
        std::string Name;
        std::string Comment;
        std::vector<Instruction> Instructions;

        friend void to_json(nlohmann::json &j, const Protocol &protocol);
        friend void from_json(const nlohmann::json &j, Protocol &protocol);

        friend std::ostream& operator<<(std::ostream& os, const Protocol& obj);
    };

}

#endif
