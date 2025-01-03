// 데몬 프로세스 관리를 위한 헤더 파일들
#include "ProcessDaemon.h"
#include <fstream>      // 파일 입출력을 위한 헤더
#include <sstream>      // 문자열 스트림 처리를 위한 헤더
#include <iostream>     // 표준 입출력을 위한 헤더
#include <dirent.h>     // 디렉토리 탐색을 위한 헤더
#include <signal.h>     // 시그널 처리를 위한 헤더
#include <unistd.h>     // POSIX 시스템 콜을 위한 헤더
#include <sys/stat.h>   // 파일 상태 확인을 위한 헤더
#include <algorithm>    // 알고리즘 함수들을 위한 헤더

ProcessDaemon::ProcessDaemon() {}

// 실행 중인 데몬 프로세스 목록 조회 함수
std::vector<ProcessDaemon::DaemonInfo> ProcessDaemon::listDaemons() {
    std::vector<DaemonInfo> daemons;
    // /proc 디렉토리 열기
    DIR* dir = opendir("/proc");
    if (!dir) {
        return daemons;
    }

    // /proc 디렉토리 순회
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {  // 디렉토리인 경우만 처리
            std::string dirname = entry->d_name;
            // PID 디렉토리만 처리 (숫자로만 된 디렉토리)
            if (std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
                pid_t pid = std::stoi(dirname);
                // 데몬 프로세스인 경우 목록에 추가
                if (isProcessDaemon(pid)) {
                    DaemonInfo daemon;
                    daemon.pid = pid;
                    daemon.name = getDaemonName(pid);
                    daemon.isRunning = true;
                    daemons.push_back(daemon);
                }
            }
        }
    }
    closedir(dir);
    return daemons;
}

// 데몬 프로세스 중지 함수
bool ProcessDaemon::stopDaemon(const std::string& name) {
    pid_t pid = findDaemonPid(name);
    if (pid > 0) {
        // SIGTERM 시그널을 프로세스에 전송
        if (kill(pid, SIGTERM) == 0) {
            // 프로세스가 종료될 때까지 잠시 대기
            sleep(1);
            return true;
        }
    }
    return false;
}

// 데몬 프로세스 실행 상태 확인 함수
bool ProcessDaemon::isDaemonRunning(const std::string& name) {
    return findDaemonPid(name) > 0;
}

// 데몬 프로세스의 PID 찾기 함수
pid_t ProcessDaemon::findDaemonPid(const std::string& name) {
    auto daemons = listDaemons();
    // 데몬 목록에서 이름으로 검색
    for (const auto& daemon : daemons) {
        if (daemon.name == name) {
            return daemon.pid;
        }
    }
    return -1;  // 찾지 못한 경우
}

// 프로세스가 데몬인지 확인하는 함수
bool ProcessDaemon::isProcessDaemon(pid_t pid) {
    // /proc/{pid}/stat 파일에서 프로세스 정보 읽기
    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    if (!statFile.is_open()) {
        return false;
    }

    // stat 파일 파싱
    std::string line;
    std::getline(statFile, line);
    std::istringstream iss(line);
    
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() < 4) {
        return false;
    }

    // PPID가 1인 경우 데몬으로 간주
    // (init/systemd가 부모 프로세스인 경우)
    return std::stoi(tokens[3]) == 1;
}

// 데몬 프로세스의 이름 가져오기 함수
std::string ProcessDaemon::getDaemonName(pid_t pid) {
    // /proc/{pid}/cmdline 파일에서 프로세스 이름 읽기
    std::string cmdlinePath = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream cmdlineFile(cmdlinePath);
    std::string name;
    
    if (cmdlineFile.is_open()) {
        // cmdline 파일에서 프로세스 이름 읽기
        std::getline(cmdlineFile, name, '\0');
        // 경로에서 실제 프로세스 이름만 추출
        std::size_t pos = name.find_last_of('/');
        if (pos != std::string::npos) {
            name = name.substr(pos + 1);
        }
    }
    
    return name.empty() ? "unknown" : name;
}
