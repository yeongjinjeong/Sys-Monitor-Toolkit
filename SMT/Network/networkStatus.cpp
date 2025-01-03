#include "Network.h"

void NetworkManager::NetworkTools::monitorWiFi() {
    try {
        #ifdef _WIN32
        std::cout << "Scanning for available Wi-Fi networks (Windows)...\n";
        int result = system("netsh wlan show networks");
        if (result != 0) {
            std::cerr << "Failed to scan Wi-Fi networks. Ensure 'netsh' is accessible.\n";
        }
        #elif __linux__
        std::cout << "Scanning for available Wi-Fi networks (Linux)...\n";
        int result = system("nmcli dev wifi list");
        if (result != 0) {
            std::cerr << "Failed to scan Wi-Fi networks. Ensure 'nmcli' is installed and accessible.\n";
        }
        #else
        std::cerr << "Wi-Fi scanning is not supported on this platform.\n";
        #endif
    } catch (std::exception& e) {
        std::cerr << "Error in monitorWiFi: " << e.what() << "\n";
    }
}
