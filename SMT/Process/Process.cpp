// 프로세스 관리를 위한 헤더 파일들
#include "Process.h"
#include "ProcessResource.h"
#include "ProcessPriority.h"
#include "ProcessDaemon.h"
#include <iostream>      // 표준 입출력을 위한 헤더
#include <iomanip>       // 출력 형식 지정을 위한 헤더
#include <thread>        // 스레드 관리를 위한 헤더
#include <chrono>        // 시간 관련 기능을 위한 헤더

// Process 클래스 생성자
// 각 관리 객체를 unique_ptr로 초기화
Process::Process()
    : resourceMonitor(std::make_unique<ProcessResource>())    // 리소스 모니터링 객체
    , priorityManager(std::make_unique<ProcessPriority>())    // 우선순위 관리 객체
    , daemonManager(std::make_unique<ProcessDaemon>()) {      // 데몬 관리 객체
}

// 소멸자 - unique_ptr 사용으로 자동 메모리 해제
Process::~Process() = default;

// 리소스 모니터링 함수
void Process::monitorResources() {
    char input;
    bool monitoring = true;

    // 모니터링을 위한 별도 스레드 생성
    // 람다 함수를 사용하여 모니터링 로직 구현
    std::thread monitorThread([this, &monitoring]() {
        while(monitoring) {
            system("clear");  // 화면 지우기
            std::cout << "\n=== Process Resource Monitor ===\n\n";
            resourceMonitor->updateProcessList();    // 프로세스 목록 갱신
            resourceMonitor->displayProcessInfo();   // 정보 출력
            std::cout << "\nPress 'q' to quit monitoring...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));  // 2초 대기
        }
    });

    // 사용자 입력 처리
    while(monitoring) {
        input = std::cin.get();
        if(input == 'q' || input == 'Q') {
            monitoring = false;
            break;
        }
    }

    // 모니터링 스레드 종료 대기
    monitorThread.join();
}

// 프로세스 우선순위 조정 함수
bool Process::adjustPriority(pid_t pid, int priority) {
    try {
        // 우선순위 변경 시도
        if(priorityManager->setPriority(pid, priority)) {
            // 성공 시 결과 출력
            std::cout << "\nPriority adjustment successful!\n";
            std::cout << "Process ID: " << pid << "\n";
            std::cout << "New Priority: " << priority << " (" 
                     << priorityManager->getPriorityLevel(priority) << ")\n";
            return true;
        } else {
            // 실패 시 에러 메시지 출력
            std::cout << "\nFailed to adjust priority.\n";
            return false;
        }
    } catch (const std::exception& e) {
        // 예외 발생 시 에러 처리
        std::cerr << "Error adjusting priority: " << e.what() << std::endl;
        return false;
    }
}

// 데몬 프로세스 관리 함수
void Process::manageDaemons() {
    while(true) {
        // 데몬 관리 메뉴 표시
        std::cout << "\n=== Daemon Management ===\n";
        std::cout << "1. List Active Daemons\n";
        std::cout << "2. Stop Daemon\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Select: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // 활성 데몬 목록 표시
                auto daemons = daemonManager->listDaemons();
                std::cout << "\nActive Daemons:\n";
                std::cout << std::setw(20) << "NAME" << std::setw(10) << "PID" << "\n";
                std::cout << std::string(30, '-') << "\n";

                // 각 데몬의 정보 출력
                for(const auto& daemon : daemons) {
                    std::cout << std::setw(20) << daemon.name 
                             << std::setw(10) << daemon.pid << "\n";
                }
                break;
            }
            case 2: {
                // 데몬 중지 기능
                std::string name;
                std::cout << "Enter daemon name to stop: ";
                std::cin >> name;

                // 데몬 중지 시도 및 결과 출력
                if (daemonManager->stopDaemon(name)) {
                    std::cout << "Daemon '" << name << "' stopped successfully!\n";
                } else {
                    std::cout << "Failed to stop daemon '" << name << "'\n";
                }
                break;
            }
            case 3:
                return;  // 메인 메뉴로 돌아가기
            default:
                std::cout << "Invalid option\n";
        }
    }
}
