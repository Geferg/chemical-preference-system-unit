#include "Protocol.h"

namespace Formatting {
    std::ostream &operator<<(std::ostream &os, const Protocol &obj) {
        os << obj.Name << "\n";
        for (const Instruction& instruction : obj.Instructions) {
            os << instruction.GetInfo(true) << "\n";
        }

        return os;
    }
}