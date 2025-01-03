// 시스템 정보 수집을 위한 헤더 파일들
#include "ProcessResource.h"
#include <fstream>       // 파일 입출력을 위한 헤더
#include <sstream>       // 문자열 스트림 처리를 위한 헤더
#include <iostream>      // 표준 입출력을 위한 헤더
#include <iomanip>       // 출력 형식 지정을 위한 헤더
#include <dirent.h>      // 디렉토리 탐색을 위한 헤더
#include <unistd.h>      // POSIX 시스템 콜을 위한 헤더
#include <sys/stat.h>    // 파일 상태 확인을 위한 헤더
#include <algorithm>     // 알고리즘 함수들을 위한 헤더
#include <cstring>       // C 문자열 처리를 위한 헤더
#include <map>           // 키-값 쌍 저장을 위한 헤더

// CPU 데이터를 저장하기 위한 구조체
struct CPUData {
    unsigned long user;        // 사용자 모드에서의 CPU 시간
    unsigned long nice;        // nice 우선순위의 사용자 모드 CPU 시간
    unsigned long system;      // 시스템 모드에서의 CPU 시간
    unsigned long idle;        // 유휴 상태 시간
    unsigned long iowait;      // I/O 대기 시간
    unsigned long irq;         // 하드웨어 인터럽트 처리 시간
    unsigned long softirq;     // 소프트웨어 인터럽트 처리 시간
    unsigned long steal;       // 가상화 환경에서 다른 CPU에 뺏긴 시간
    unsigned long guest;       // 가상 CPU에서 실행된 시간
    unsigned long guest_nice;  // nice 우선순위의 가상 CPU 실행 시간
};

// 이전 CPU 사용량 데이터를 저장하는 정적 변수들
static std::map<pid_t, CPUData>prevCPUData;
static CPUData prevTotalCPUData;

ProcessResource::ProcessResource() {
    updateProcessList();  // 초기 프로세스 목록 갱신
}

// CPU 사용률 계산 함수
double ProcessResource::calculateCPUUsage(pid_t pid) {
    // /proc/{pid}/stat 파일에서 프로세스의 CPU 정보 읽기
    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    
    if (!statFile.is_open()) {
        return 0.0;
    }

    // 파일에서 한 줄을 읽어 토큰으로 분리
    std::string line;
    std::getline(statFile, line);
    std::istringstream iss(line);

    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if(tokens.size() < 22) {
        return 0.0;
    }

    // CPU 시간 관련 값들 추출
    unsigned long utime = std::stoul(tokens[13]);    // 사용자 모드 시간
    unsigned long stime = std::stoul(tokens[14]);    // 시스템 모드 시간
    unsigned long cutime = std::stoul(tokens[15]);   // 자식 프로세스의 사용자 모드 시간
    unsigned long cstime = std::stoul(tokens[16]);   // 자식 프로세스의 시스템 모드 시간

    // 전체 시스템 CPU 정보 읽기
    std::ifstream procStatFile("/proc/stat");
    std::string cpuLine;
    std::getline(procStatFile, cpuLine);
    std::istringstream cpuIss(cpuLine);

    CPUData currentTotalCPU;
    std::string cpu;
    cpuIss >> cpu 
           >> currentTotalCPU.user
           >> currentTotalCPU.nice
           >> currentTotalCPU.system
           >> currentTotalCPU.idle
           >> currentTotalCPU.iowait
           >> currentTotalCPU.irq
           >> currentTotalCPU.softirq
           >> currentTotalCPU.steal
           >> currentTotalCPU.guest
           >> currentTotalCPU.guest_nice;

    // 프로세스의 총 CPU 시간 계산
    unsigned long totalProcessTime = utime + stime + cutime + cstime;

    // CPU 사용률 계산
    if(prevCPUData.find(pid) != prevCPUData.end()) {
        unsigned long prevProcessTotal = prevCPUData[pid].user + prevCPUData[pid].system;
        
        // 전체 CPU 시간 계산
        unsigned long totalCPUTime = (currentTotalCPU.user + currentTotalCPU.nice + 
                                    currentTotalCPU.system + currentTotalCPU.idle + 
                                    currentTotalCPU.iowait + currentTotalCPU.irq + 
                                    currentTotalCPU.softirq + currentTotalCPU.steal);
        
        unsigned long prevTotalCPUTime = (prevTotalCPUData.user + prevTotalCPUData.nice + 
                                        prevTotalCPUData.system + prevTotalCPUData.idle + 
                                        prevTotalCPUData.iowait + prevTotalCPUData.irq + 
                                        prevTotalCPUData.softirq + prevTotalCPUData.steal);

        // CPU 사용률 계산 및 유효성 검사
        unsigned long timeDiff = totalCPUTime - prevTotalCPUTime;
        if (timeDiff > 0) {
            double cpuUsage = 100.0 * (totalProcessTime - prevProcessTotal) / timeDiff;
            if (cpuUsage >= 0 && cpuUsage <= 100) {
                // 현재 데이터 저장
                CPUData currentProcessCPU;
                currentProcessCPU.user = utime;
                currentProcessCPU.system = stime;
                prevCPUData[pid] = currentProcessCPU;
                prevTotalCPUData = currentTotalCPU;
                return cpuUsage;
            }
        }
    }

    // 첫 실행 또는 계산 실패 시 초기 데이터 저장
    CPUData initialProcessCPU;
    initialProcessCPU.user = utime;
    initialProcessCPU.system = stime;
    prevCPUData[pid] = initialProcessCPU;
    prevTotalCPUData = currentTotalCPU;

    return 0.0;
}

// 프로세스의 메모리 사용량 조회 함수
std::size_t ProcessResource::getMemoryUsage(pid_t pid) {
    // /proc/{pid}/status 파일에서 메모리 정보 읽기
    std::string statusPath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);

    if(!statusFile.is_open()) {
        return 0;
    }

    // VmRSS 값 찾기 (실제 물리 메모리 사용량)
    std::string line;
    std::size_t vmRSS = 0;
    while (std::getline(statusFile, line)) {
        if(line.compare(0, 6, "VmRss:") == 0) {
            std::istringstream iss(line);
            std::string label;
            iss >> label >> vmRSS;
            break;
        }
    }

    return vmRSS;  // KB 단위 반환
}

// 프로세스 이름 조회 함수
std::string ProcessResource::getProcessName(pid_t pid) {
    // /proc/{pid}/comm 파일에서 프로세스 이름 읽기
    std::string path = "/proc/" + std::to_string(pid) + "/comm";
    std::ifstream comm_file(path);
    std::string name;

    if(comm_file.is_open()) {
        std::getline(comm_file, name);
        // 이름이 너무 길 경우 잘라내기
        if(name.length() > 15) {
            name = name.substr(0, 12) + "...";
        }
    } else {
        name = "[unknown]";
    }

    return name;
}

// 프로세스 정보 읽기 함수
void ProcessResource::readProcStats(ProcessInfo& info) {
    info.cpuUsage = calculateCPUUsage(info.pid);
    info.memoryUsage = getMemoryUsage(info.pid);
    info.lastUpdateTime = std::chrono::system_clock::now();
}

// 프로세스 목록 업데이트 함수
void ProcessResource::updateProcessList() {
    DIR* dir = opendir("/proc");
    if(!dir) {
        throw std::runtime_error("Cannot open /proc directory");
    }

    processes.clear();
    struct dirent* entry;

    // /proc 디렉토리 순회하며 프로세스 정보 수집
    while((entry = readdir(dir)) != nullptr) {
        if(entry->d_type == DT_DIR) {
            std::string dirname = entry->d_name;
            // PID 디렉토리만 처리 (숫자로만 된 디렉토리)
            if(std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
                pid_t pid = std::stoi(dirname);
                ProcessInfo info;
                info.pid = pid;
                info.name = getProcessName(pid);
                readProcStats(info);
                processes.push_back(info);
            }
        }
    }
    closedir(dir);

    // CPU 사용률 기준으로 내림차순 정렬
    std::sort(processes.begin(), processes.end(),
            [](const ProcessInfo& a, const ProcessInfo& b) {
                return a.cpuUsage > b.cpuUsage;
            });
}

// 프로세스 정보 화면 출력 함수
void ProcessResource::displayProcessInfo() {
    // 테이블 헤더 출력
    std::cout << std::left
              << std::setw(8) << "PID"
              << std::setw(20) << "NAME"
              << std::right
              << std::setw(10) << "CPU%"
              << std::setw(15) << "MEM(KB)" << "\n"
              << std::string(53, '-') << "\n";

    // 상위 15개 프로세스 정보 출력
    int count = 0;
    for(const auto& proc : processes) {
        if(count++ >= 15) break;

        std::cout << std::left << std::setw(8) << proc.pid
                  << std::setw(20) << proc.name
                  << std::right
                  << std::setw(9) << std::fixed << std::setprecision(1) << proc.cpuUsage << "%"
                  << std::setw(15) << proc.memoryUsage << "\n";
    }

    // 총계 계산 및 출력
    double totalCPU = 0.0;
    std::size_t totalMem = 0;
    for(const auto& proc : processes) {
        totalCPU += proc.cpuUsage;
        totalMem += proc.memoryUsage;
    }

    std::cout << "\nTotal Processes: " << processes.size()
              << "  CPU Usage: " << std::fixed << std::setprecision(1) << totalCPU << "%"
              << "  Memory Usage: " << (totalMem / 1024) << " MB\n";
}
