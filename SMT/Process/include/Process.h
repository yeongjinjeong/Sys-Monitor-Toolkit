#pragma once
#include <memory>
#include <string>

// 전방 선언 - 헤더 파일 순환 참조 방지
class ProcessResource;
class ProcessPriority;
class ProcessDaemon;

// 프로세스 관리의 메인 클래스
// 리소스 모니터링, 우선순위 조정, 데몬 관리 기능을 총괄
class Process {
private:
    // 각 기능별 객체를 unique_ptr로 관리하여 메모리 누수 방지
    std::unique_ptr<ProcessResource> resourceMonitor;    // 리소스 모니터링 담당
    std::unique_ptr<ProcessPriority> priorityManager;    // 우선순위 관리 담당
    std::unique_ptr<ProcessDaemon> daemonManager;        // 데몬 프로세스 관리 담당

public:
    Process();    // 생성자 - 각 관리 객체 초기화
    ~Process();   // 소멸자

    // 시스템의 프로세스 리소스(CPU, 메모리) 사용량 모니터링
    void monitorResources();

    // 특정 프로세스의 우선순위 조정
    // @param pid: 대상 프로세스 ID
    // @param priority: 설정할 우선순위 값(-20 ~ 19)
    // @return: 우선순위 조정 성공 여부
    bool adjustPriority(pid_t pid, int priority);

    // 시스템의 데몬 프로세스 관리
    void manageDaemons();
};
