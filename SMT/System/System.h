#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/statvfs.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <unistd.h>

class System{
public:
    float getCPUUsage();
    void getMemoryUsage();
    void getDiskUsage(const std::string& path = "/");
    void run() {
        char input;
        bool monitoring = true;

        // 모니터링 작업을 수행하는 별도 스레드
        std::thread monitorThread([this, &monitoring]() {
            while (monitoring) {
                system("clear");  // 화면 지우기
                std::cout << "=== System Resource Monitor ===\n\n";
                std::cout << "CPU Usage: " << getCPUUsage() << "%\n";
                getMemoryUsage();
                getDiskUsage("/");
                std::cout << "\nPress 'q' to quit monitoring...\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));  // 1초 대기
            }
        });

        // 사용자 입력 처리 루프
        while (monitoring) {
            input = std::cin.get();
            if (input == 'q') {
                monitoring = false;
                break;
            }
        }

        // 모니터링 스레드 종료 대기
        monitorThread.join();
    }
};