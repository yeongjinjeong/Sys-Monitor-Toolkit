#include "System/System.h"
#include "Process/ProcessManager.h"
#include "Network/Network.h"
#include <iostream>
#include <memory>

void showMainMenu() {
    std::cout << "\n=== System Management Tool ===\n"
              << "1. Process Management\n"
              << "2. System Management\n"
              << "3. Network Management\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

int main() {
    int choice;
    std::unique_ptr<System> systemManager;
    std::unique_ptr<ProcessManager> processManager;
    std::unique_ptr<NetworkManager> networkManager;

    while (true) {
        showMainMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                if (!processManager) {
                    processManager = std::make_unique<ProcessManager>();
                }
                processManager->run();
                break;
            case 2:
                systemManager->run();
                break;
            case 3:
                networkManager->run();
                break;
            case 0:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid option!\n";
        }
    }

    return 0;
}
