#include "Process/include/ProcessManager.h"
#include <iostream>
#include <memory>

void showMainMenu() {
    std::cout << "\n=== System Management Tool ===\n"
              << "1. Process Management\n"
              << "2. Network Management\n"
              << "3. Storage Management\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

int main() {
    int choice;
    std::unique_ptr<ProcessManager> processManager;

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
                std::cout << "Network Management - Not implemented yet\n";
                break;
            case 3:
                std::cout << "Storage Management - Not implemented yet\n";
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
