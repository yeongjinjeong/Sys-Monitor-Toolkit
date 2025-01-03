#include "../include/Network.h"

void NetworkManager::run() {
    NetworkTools networkTools;
    bool exitProgram = false;

    while (!exitProgram) {
        int choice = getUserChoice();

        switch (choice) {
            case 1:
                networkTools.searchDevices();
                break;
            case 2:
                networkTools.findUsedPorts();
                break;
            case 3:
                networkTools.monitorWiFi();
                break;
            case 0:
                std::cout << "End\n";
                exitProgram = true;
                break;
            default:
                std::cout << "Invalid option! Please try again.\n";
        }

        std::cout << "-------------------------\n";
    }
}

int NetworkManager::getUserChoice() {
    int choice;
    std::cout << "Select an option:\n";
    std::cout << "1. Local Network Device Search (NetWork_Search)\n";
    std::cout << "2. Port Scan and Service Identification (NetWork_Port)\n";
    std::cout << "3. Wi-Fi Network Monitoring (NetWork_Status)\n";
    std::cout << "0. Exit\n";
    std::cin >> choice;
    return choice;
}
