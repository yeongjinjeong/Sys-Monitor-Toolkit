#include "Network.h"

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
                break;
        }
    }
}

int NetworkManager::getUserChoice() {
    int choice;
    while (true) {
        std::cout << "Select an option:\n";
        std::cout << "1. Local Network Device Search (NetWork_Search)\n";
        std::cout << "2. Port Scan and Service Identification (NetWork_Port)\n";
        std::cout << "3. Wi-Fi Network Monitoring (NetWork_Status)\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number between 0 and 3.\n";
            std::cout << "=========================\n";
        } else if (choice >= 0 && choice <= 3) {
            return choice;
        } else {
            std::cout << "Invalid option! Please enter a number between 0 and 3.\n";
            std::cout << "=========================\n";
        }
    }
}
