// 프로세스 우선순위 관리를 위한 헤더 파일들
#include "ProcessPriority.h"
#include <sys/resource.h>   // 프로세스 리소스 제어를 위한 헤더 (setpriority, getpriority 함수 포함)
#include <unistd.h>         // POSIX 시스템 콜을 위한 헤더 (geteuid 함수 포함)

ProcessPriority::ProcessPriority() {}

// 프로세스 우선순위 설정 함수
// @param pid: 대상 프로세스 ID
// @param priority: 설정할 우선순위 값 (-20 ~ 19)
// 반환값: 우선순위 설정 성공 여부
bool ProcessPriority::setPriority(pid_t pid, int priority) {
    // 권한 확인 및 우선순위 값 유효성 검사
    if (!checkPermissions() || !isValidPriority(priority)) {
        return false;
    }

    // setpriority 시스템 콜을 사용하여 우선순위 설정
    // PRIO_PROCESS: 프로세스 우선순위 변경 모드
    // 반환값이 0이면 성공, -1이면 실패
    return (setpriority(PRIO_PROCESS, pid, priority) == 0);
}

// 현재 프로세스 우선순위 조회 함수
// @param pid: 대상 프로세스 ID
// 반환값: 프로세스의 현재 우선순위 (-20 ~ 19), 실패 시 -999 반환
int ProcessPriority::getPriority(pid_t pid) {
    errno = 0;  // 에러 상태 초기화
    // getpriority 시스템 콜을 사용하여 우선순위 조회
    int priority = getpriority(PRIO_PROCESS, pid);
    
    // getpriority는 실패 시 -1을 반환하고 errno를 설정
    // 정상적인 우선순위 값도 -1일 수 있으므로 errno로 구분
    if (errno != 0) {
        return -999;  // 에러 발생 시 특수 값 반환
    }
    return priority;
}

// 우선순위 값을 문자열로 변환하는 함수
// @param priority: 우선순위 값 (-20 ~ 19)
// 반환값: 우선순위 레벨을 나타내는 문자열
std::string ProcessPriority::getPriorityLevel(int priority) {
    // 우선순위 값에 따른 레벨 문자열 반환
    if (priority < -15) return "Very High";
    if (priority < -10) return "High";
    if (priority < 0) return "Above Normal";
    if (priority == 0) return "Normal";
    if (priority < 10) return "Below Normal";
    return "Low";
}

// 우선순위 값 유효성 검사 함수
// @param priority: 검사할 우선순위 값
// 반환값: 유효한 우선순위 값인지 여부
bool ProcessPriority::isValidPriority(int priority) {
    // 리눅스 시스템의 유효한 우선순위 범위: -20 ~ 19
    return priority >= -20 && priority <= 19;
}

// 우선순위 변경 권한 확인 함수
// 반환값: 권한 있음 여부
bool ProcessPriority::checkPermissions() {
    // geteuid(): 실효 사용자 ID 반환
    // 루트 사용자(ID: 0)만 다른 프로세스의 우선순위를 변경할 수 있음
    return (geteuid() == 0);
}
