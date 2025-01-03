#include "Network.h"

using namespace boost::asio;

void NetworkManager::NetworkTools::searchDevices() {
    try {
        struct ifaddrs* ifAddrStruct = nullptr;
        struct ifaddrs* ifa = nullptr;

        // Get network interfaces
        if (getifaddrs(&ifAddrStruct) == -1) {
            throw std::runtime_error("Failed to get network interfaces");
        }

        std::cout << "=========================\n";
        std::cout << "Local Network Devices:\n";
        std::cout << "=========================\n";

        for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa->ifa_addr == nullptr) continue;

            // Check if the interface is UP and supports IPv4
            if ((ifa->ifa_flags & IFF_UP) && ifa->ifa_addr->sa_family == AF_INET) {
                char ip[INET_ADDRSTRLEN];
                char netmask[INET_ADDRSTRLEN];
                char broadcast[INET_ADDRSTRLEN];

                // Get IPv4 address
                inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, ip, INET_ADDRSTRLEN);

                // Get netmask
                if (ifa->ifa_netmask) {
                    inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_netmask)->sin_addr, netmask, INET_ADDRSTRLEN);
                } else {
                    strcpy(netmask, "Unknown");
                }

                // Get broadcast address
                if (ifa->ifa_flags & IFF_BROADCAST && ifa->ifa_broadaddr) {
                    inet_ntop(AF_INET, &((struct sockaddr_in*)ifa->ifa_broadaddr)->sin_addr, broadcast, INET_ADDRSTRLEN);
                } else {
                    strcpy(broadcast, "N/A");
                }

                // Print network interface details
                std::cout << "Interface: " << ifa->ifa_name << "\n";
                std::cout << "  IP Address: " << ip << "\n";
                std::cout << "  Netmask: " << netmask << "\n";
                std::cout << "  Broadcast: " << broadcast << "\n";
                std::cout << "-------------------------\n";
            }
        }

        // Free the interface list
        if (ifAddrStruct != nullptr) {
            freeifaddrs(ifAddrStruct);
        }
    } catch (std::exception& e) {
        std::cerr << "Error in searchDevices: " << e.what() << "\n";
    }
}
