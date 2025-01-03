#pragma once 
#include <sys/types.h>
#include <vector>
#include <string>
#include <chrono>

// 프로세스의 리소스 사용량을 모니터링하는 클래스
class ProcessResource {
private:
    // 개별 프로세스의 정보를 저장하는 구조체
    struct ProcessInfo {
        pid_t pid;                                    // 프로세스 ID
        std::string name;                            // 프로세스 이름
        double cpuUsage;                             // CPU 사용률
        std::size_t memoryUsage;                     // 메모리 사용량
        std::chrono::system_clock::time_point lastUpdateTime;  // 마지막 업데이트 시간
    };

    std::vector<ProcessInfo> processes;              // 프로세스 정보 목록
    std::chrono::system_clock::time_point lastUpdateTime;  // 전체 업데이트 시간

public:
    ProcessResource();
    void updateProcessList();    // 프로세스 목록 갱신
    void displayProcessInfo();   // 프로세스 정보 화면 출력

private:
    // CPU 사용률 계산
    double calculateCPUUsage(pid_t pid);
    // 메모리 사용량 조회
    std::size_t getMemoryUsage(pid_t pid);
    // 프로세스 이름 조회
    std::string getProcessName(pid_t pid);
    // proc 파일시스템에서 프로세스 정보 읽기
    void readProcStats(ProcessInfo& info);
};
