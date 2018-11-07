//
// Created by PulsarV on 18-11-2.
//

#include <flight_control_sample.hpp>

int main(int argc, char **argv) {
    LinuxSetup linuxEnvironment(2, "./UserConfig.txt");
    Vehicle*   vehicle = linuxEnvironment.getVehicle();
    monitoredTakeoff(vehicle);
    if (vehicle == nullptr)
    {
        std::cout << "Vehicle not initialized, exiting.\n";
        return -1;
    }
    moveByPositionOffset(vehicle, 0, 0, 2, 0);
//    moveByPositionOffset(vehicle, 1, 0, -1, -30);
//    moveByPositionOffset(vehicle, -1, -1, 0, 0);
    monitoredLanding(vehicle);
    return 0;
}