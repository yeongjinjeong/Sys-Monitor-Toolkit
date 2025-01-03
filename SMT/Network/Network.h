#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <iostream>
#include <set>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ip/address.hpp>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <net/if.h> 
#include <stdexcept> 

class NetworkManager {
private:
    class NetworkTools {
    public:
        void searchDevices();  // 로컬 네트워크 장치 검색
        void findUsedPorts();  // 사용 중인 포트 스캔
        void monitorWiFi();    // Wi-Fi 네트워크 모니터링
    };

    int getUserChoice();  // 사용자 입력을 받는 함수

public:
    void run();  // 네트워크 관리 실행
};

#endif // NETWORK_MANAGER_H
