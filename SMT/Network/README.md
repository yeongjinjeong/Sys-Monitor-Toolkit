## 네트워크 스캐닝 및 모니터링 도구 - NetWork
    - 로컬 네트워크 장치 검색 - NetWork_Search
    - 포트 스캔 및 서비스 식별 - NetWork_Port
    - 네트워크 상태 모니터링 - NetWork_Status
    - +@(libpcap 라이브러리 활용한 패킷 캡처)

---

## **Network Module Overview**  
`NetworkManager` 모듈은 C++ 기반 네트워크 관리 시스템으로, 로컬 네트워크 디바이스 검색, 사용 중인 포트 확인, Wi-Fi 네트워크 모니터링 기능을 제공합니다. **Boost.Asio**, **ifaddrs.h**, **arpa/inet.h** 등의 라이브러리를 사용해 네트워크 정보를 탐색하고 실시간으로 출력합니다.  
이 모듈은 플랫폼별 명령어와 라이브러리를 사용해 **Windows**, **Linux** 환경을 모두 지원합니다.

---

## **구성 요소 및 주요 기능**

### **1. NetworkManager 클래스**
`NetworkManager` 클래스는 사용자 입력을 처리하고 네트워크 관련 기능을 실행합니다.  
- **`void run()`**: 사용자 메뉴를 출력하고 기능을 선택하도록 안내합니다.  
- **`int getUserChoice()`**: 사용자의 입력을 받고 유효성을 검사하여 선택지를 반환합니다.  

### **2. NetworkTools 클래스 (내부 클래스)**
`NetworkTools` 클래스는 네트워크 작업을 수행하는 세 가지 주요 메서드를 제공합니다.  

#### **① `void searchDevices()` – 로컬 네트워크 장치 검색**
- 시스템의 모든 네트워크 인터페이스 정보를 확인합니다.  
- **IP 주소, 넷마스크, 브로드캐스트 주소**를 출력합니다.  
- **Linux 시스템의 `ifaddrs` 구조체**를 사용해 네트워크 인터페이스 정보를 가져옵니다.  

#### **② `void findUsedPorts()` – 사용 중인 포트 확인**
- **Boost.Asio**를 사용해 **1번부터 65535번 포트**까지 검사합니다.  
- 사용 중인 포트를 모두 출력합니다.  
- 에러 처리 및 예외 상황(포트 점유 실패 등)을 고려해 안정성을 높였습니다.  

#### **③ `void monitorWiFi()` – Wi-Fi 네트워크 모니터링**
- **Windows (`netsh wlan show networks`)**: 주변 Wi-Fi 네트워크를 스캔해 정보를 출력합니다.  
- **Linux (`nmcli dev wifi list`)**: 사용 가능한 Wi-Fi 네트워크 정보를 조회합니다.  
- 플랫폼에 따라 지원하지 않는 경우 에러 메시지를 출력합니다.  

---

## **의존성 및 라이브러리**
- **Boost.Asio**: 네트워크 I/O 및 포트 스캔 처리  
- **ifaddrs.h**, **arpa/inet.h**: 네트워크 인터페이스 정보 탐색  
- **net/if.h**: 네트워크 인터페이스 플래그 확인  

---

## **컴파일 및 실행 방법**

### **1. 의존성 설치**
- **Boost 라이브러리** 설치 필요 (Linux와 Windows에서 지원)  
- **Linux**: `sudo apt-get install libboost-all-dev`  
- **Windows**: Boost 라이브러리를 설치 후 Visual Studio에 통합  

### **2. 컴파일 명령어 (Linux 예시)**
```bash
g++ -o network_manager Network.cpp networkPort.cpp networkSearch.cpp networkStatus.cpp -lboost_system
```

### **3. 실행**
```bash
./network_manager
```

---

## **사용 예시**
```bash
Select an option:
1. Local Network Device Search (NetWork_Search)
2. Port Scan and Service Identification (NetWork_Port)
3. Wi-Fi Network Monitoring (NetWork_Status)
0. Exit
```

- **1 입력**: 로컬 네트워크 장치 정보 확인  
- **2 입력**: 사용 중인 포트 리스트 출력  
- **3 입력**: Wi-Fi 네트워크 목록 출력  

---

## **확장 가능 기능**  
- **네트워크 속도 테스트** 추가  
- **네트워크 인터페이스 상태 변경** (활성화/비활성화)  
- **Wi-Fi 신호 강도 분석 및 알림 기능**  

---

## **에러 처리**
- 사용자 입력 검증 및 잘못된 입력 방지  
- 네트워크 장치 검색 실패 시 예외 처리  

---

