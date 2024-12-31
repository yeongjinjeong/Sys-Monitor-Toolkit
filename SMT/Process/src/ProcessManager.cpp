#include "ProcessManager.h"
#include <iostream>
#include <stdexcept>

ProcessManager::ProcessManager() 
    : processHandler(std::make_unique<Process>()) {
}

ProcessManager::~ProcessManager() = default;

// 메인 메뉴 출력 함수
void ProcessManager::showMenu() {
    std::cout << "\n=== Process Manager ===\n"
              << "1. Monitor Process Resources\n"
              << "2. Adjust Process Priority\n"
              << "3. Manage Daemons\n"
              << "0. Back to Main Menu\n"
              << "Choose an option: ";
}

void ProcessManager::run() {
    try {
        int choice;
        
        while (true) {
            showMenu();
            std::cin >> choice;
            
            switch (choice) {
                case 1: //리소스 모니터링 시작
                    processHandler->monitorResources();
                    break;
                case 2: { //프로세스 우선순위 조정
                    pid_t pid;
                    int priority;
                    std::cout << "Enter PID: ";
                    std::cin >> pid;
                    std::cout << "Enter Priority (-20 to 19): ";
                    std::cin >> priority;
                    processHandler->adjustPriority(pid, priority);
                    break;
                }
                case 3: //데몬 프로세스 관리
                    processHandler->manageDaemons();
                    break;
                case 0:
                    return;
                default:
                    std::cout << "Invalid option!\n";
            }
        }
    } catch (const std::exception& e) { //예외처리
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
