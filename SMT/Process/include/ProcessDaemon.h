#pragma once
#include <string>
#include <vector>
#include <sys/types.h>

// 데몬 프로세스를 관리하는 클래스
class ProcessDaemon {
public:
    // 데몬 프로세스 정보를 저장하는 구조체
    struct DaemonInfo {
        std::string name;    // 데몬 이름
        pid_t pid;          // 프로세스 ID
        bool isRunning;     // 실행 상태
    };

    ProcessDaemon();

    // 실행 중인 데몬 프로세스 목록 조회
    std::vector<DaemonInfo> listDaemons();
    // 특정 데몬 프로세스 중지
    bool stopDaemon(const std::string& name);
    // 데몬 프로세스 실행 상태 확인
    bool isDaemonRunning(const std::string& name);

private:
    // 데몬 프로세스의 PID 찾기
    pid_t findDaemonPid(const std::string& name);
    // 프로세스가 데몬인지 확인
    bool isProcessDaemon(pid_t pid);
    // 데몬 프로세스의 이름 가져오기
    std::string getDaemonName(pid_t pid);
};
