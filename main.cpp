#include "Formatting/Instruction.h"
#include "Formatting/Argument.h"
#include "Formatting/JsonFormatter.h"
//#include "Tcp/TcpConnection.h"
#include "TcpLinuxFuture/ClientQueue.h"
#include "TcpLinuxFuture/ServerHandler.h"
#include "Hardware/HEnums.h"
#include "Hardware/ValveHandler.h"
#include "Hardware/MotorHandler.h"
#include "Hardware/HardwareControl.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <thread>
#include <atomic>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")

void reply(const Formatting::Protocol& protocol, int clientSocket) {
    std::string reply = Formatting::JsonFormatter::SerializeProtocol(protocol);
    Server::ServerHandler::Respond(clientSocket, reply);
}

Formatting::Instruction::Status handleComment(Formatting::Instruction instruction) {
    return Formatting::Instruction::Status::OK;
}

Formatting::Instruction::Status handleIllumination(Formatting::Instruction instruction) {
    std::string stateArg;

    for (auto arg : instruction.Arguments) {
        if(arg.Name == "state") {
            stateArg = arg.Value;
        }
    }

    if (stateArg == "on") {
        if (Hardware::HardwareControl::enableLed() == 0) {
            return Formatting::Instruction::Status::OK;
        }
    }
    else if (stateArg == "off") {
        if (Hardware::HardwareControl::disableLed() == 0) {
            return Formatting::Instruction::Status::OK;
        }
    }

    return Formatting::Instruction::Status::Bad;
}

Formatting::Instruction::Status handleValve(Formatting::Instruction instruction) {
    std::string clusterArg;
    std::string modeArg;

    for (auto arg : instruction.Arguments) {
        if(arg.Name == "cluster") {
            clusterArg = arg.Value;
        }
        else if (arg.Name == "mode") {
            modeArg = arg.Value;
        }
    }

    Hardware::ValveCluster cluster;
    Hardware::ValveMode mode;

    if (clusterArg == "left") cluster = Hardware::ValveCluster::left;
    else if (clusterArg == "right") cluster = Hardware::ValveCluster::right;
    else if (clusterArg == "bottom") cluster = Hardware::ValveCluster::bottom;
    else return Formatting::Instruction::Status::Bad;

    if (modeArg == "buffer") mode = Hardware::ValveMode::buffer;
    else if (modeArg == "product") mode = Hardware::ValveMode::product;
    else if (modeArg == "circuit") mode = Hardware::ValveMode::circuit;
    else if (modeArg == "empty") mode = Hardware::ValveMode::trash;
    else return Formatting::Instruction::Status::Bad;

    if (Hardware::HardwareControl::setValveMode(cluster, mode) == 0) {
        return Formatting::Instruction::Status::OK;
    }

    return Formatting::Instruction::Status::Bad;
}

Formatting::Instruction::Status handleRunMotor(Formatting::Instruction instruction) {
    std::string dirArg;
    std::string runPeriodArg;
    std::string stepPeriodArg;

    for (auto arg : instruction.Arguments) {
        if(arg.Name == "direction") {
            dirArg = arg.Value;
        }
        else if (arg.Name == "timePeriod") {
            runPeriodArg = arg.Value;
        }
        else if (arg.Name == "stepPeriod") {
            stepPeriodArg = arg.Value;
        }
    }

    Hardware::MotorDirection dir;
    int stepPeriod;
    int runPeriod;

    if (dirArg == "up") dir = Hardware::MotorDirection::up;
    else if (dirArg == "down") dir = Hardware::MotorDirection::down;
    else return Formatting::Instruction::Status::Bad;

    try {
        stepPeriod = std::stoi(stepPeriodArg);
        runPeriod = std::stoi(runPeriodArg);
    } catch (const std::invalid_argument& e) {
        return Formatting::Instruction::Status::Bad;
    } catch (const std::out_of_range& e) {
        return Formatting::Instruction::Status::Bad;
    }

    if (Hardware::HardwareControl::runMotorByTime(dir, runPeriod * 1000, stepPeriod) == 0) {
        return Formatting::Instruction::Status::OK;
    }

    return Formatting::Instruction::Status::Bad;
}

Formatting::Instruction::Status handleStopMotor(Formatting::Instruction instruction) {
    return Formatting::Instruction::Status::Bad;
}

Formatting::Instruction::Status handleWait(Formatting::Instruction instruction) {
    std::string durationArg;
    std::string unitArg;

    for (auto arg : instruction.Arguments) {
        if(arg.Name == "duration") {
            durationArg = arg.Value;
        }
        else if (arg.Name == "unit") {
            unitArg = arg.Value;
        }
    }

    int duration;
    int unitMod;

    try {
        duration = std::stoi(durationArg);
    } catch (const std::invalid_argument& e) {
        return Formatting::Instruction::Status::Bad;
    } catch (const std::out_of_range& e) {
        return Formatting::Instruction::Status::Bad;
    }

    if (unitArg == "us") return Formatting::Instruction::Status::Bad;
    else if (unitArg == "ms") unitMod = 1;
    else if (unitArg == "s") unitMod = 1000;
    else if (unitArg == "m") unitMod = 60000;
    else return Formatting::Instruction::Status::Bad;

    Hardware::HardwareControl::delay(duration * unitMod);
    return Formatting::Instruction::Status::OK;
}

Formatting::Instruction::Status handleCalibrate(Formatting::Instruction instruction) {
    std::string stepPeriodArg;

    for (auto arg : instruction.Arguments) {
        if (arg.Name == "stepPeriod") {
            stepPeriodArg = arg.Value;
        }
    }

    int stepPeriod;

    try {
        stepPeriod = std::stoi(stepPeriodArg);
    } catch (const std::invalid_argument& e) {
        return Formatting::Instruction::Status::Bad;
    } catch (const std::out_of_range& e) {
        return Formatting::Instruction::Status::Bad;
    }

    Hardware::HardwareControl::setValveMode(Hardware::left, Hardware::buffer);
    Hardware::HardwareControl::setValveMode(Hardware::right, Hardware::buffer);
    Hardware::HardwareControl::setValveMode(Hardware::bottom, Hardware::trash);

    Hardware::HardwareControl::runMotorByTime(Hardware::down, 100000000, stepPeriod);

    Hardware::HardwareControl::setValveMode(Hardware::ValveCluster::left, Hardware::ValveMode::circuit);
    Hardware::HardwareControl::setValveMode(Hardware::ValveCluster::right, Hardware::ValveMode::circuit);
    Hardware::HardwareControl::setValveMode(Hardware::ValveCluster::bottom, Hardware::ValveMode::circuit);

    Hardware::HardwareControl::runMotorByTime(Hardware::up, 100000000, stepPeriod);

    return Formatting::Instruction::Status::OK;
}

Formatting::Instruction::Status handleStatus(Formatting::Instruction instruction) {
    return Formatting::Instruction::Status::Bad;
}

int main() {
    //Tcp::TcpConnection server;
    Server::ServerHandler server;

    if(Hardware::HardwareControl::initialize() < 0) {
        return -1;
    }

    if (!Hardware::HardwareControl::isReady()) {
        return -1;
    }

    if(server.Initialize(8080)) {
        server.Listen();
    }

    while(true) {
        //Tcp::TcpConnection::QueuedData data = server.dataQueue.pop();
        Server::ServerHandler::QueuedData data = server.dataQueue.pop();
        Formatting::Protocol protocol = Formatting::JsonFormatter::GetProtocol(data.data.c_str());
        std::cout << "Received protocol:\n";
        std::cout << protocol << "\n";

        // Handle each instruction
        for (Formatting::Instruction &instruction : protocol.Instructions) {
            instruction.CurrentStatus = Formatting::Instruction::Status::Processing;

            reply(protocol, data.clientSocket);

            switch (instruction.GetType()) {
                case Formatting::Comment:
                    instruction.CurrentStatus = handleComment(instruction);
                    break;

                case Formatting::SwitchIllumination:
                    instruction.CurrentStatus = handleIllumination(instruction);
                    break;

                case Formatting::SwitchValve:
                    instruction.CurrentStatus = handleValve(instruction);
                    break;

                case Formatting::RunMotor:
                    instruction.CurrentStatus = handleRunMotor(instruction);
                    break;

                case Formatting::StopMotor:
                    instruction.CurrentStatus = handleStopMotor(instruction);
                    break;

                case Formatting::Wait:
                    instruction.CurrentStatus = handleWait(instruction);
                    break;

                case Formatting::Calibrate:
                    instruction.CurrentStatus = handleCalibrate(instruction);
                    break;

                case Formatting::Status:
                    instruction.CurrentStatus = handleStatus(instruction);
                    break;
            }

            reply(protocol, data.clientSocket);

            if (instruction.CurrentStatus == Formatting::Instruction::Status::Bad) {
                break;
            }
        }


        // Close client socket
    }

    std::string command;
    while (true) {
        std::cin >> command;
        if (command == "exit") {
            server.Close();
            break;
        }
    }

}



