#pragma once
#include <sys/types.h>
#include <string>

// 프로세스의 우선순위를 관리하는 클래스
class ProcessPriority {
public:
    ProcessPriority();

    // 프로세스 우선순위 설정
    // @param pid: 대상 프로세스 ID
    // @param priority: 설정할 우선순위 값(-20 ~ 19)
    bool setPriority(pid_t pid, int priority);

    // 현재 우선순위 조회
    int getPriority(pid_t pid);

    // 우선순위 값을 문자열로 변환 (Very High, High 등)
    std::string getPriorityLevel(int priority);
    
private:
    // 우선순위 값이 유효한지 검사
    bool isValidPriority(int priority);
    // 우선순위 변경 권한 확인
    bool checkPermissions();
};
