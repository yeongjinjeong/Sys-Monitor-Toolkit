#include "Network.h"

using namespace boost::asio;
using namespace boost::asio::ip;

void NetworkManager::NetworkTools::searchDevices() {
    try {
        io_context context;
        udp::resolver resolver(context);
        udp::resolver::query query(udp::v4(), boost::asio::ip::host_name(), "");
        auto results = resolver.resolve(query);

        std::cout << "Local Network Devices:\n";
        for (const auto& endpoint : results) {
            std::cout << "IP Address: " << endpoint.endpoint().address().to_string() << "\n";
            std::cout << "DNS Name: " << boost::asio::ip::host_name() << "\n";
        }

        std::cout << "Additional Network Information:\n";
        auto host = boost::asio::ip::host_name();
        auto addr = resolver.resolve(udp::v4(), host, "");
        for (auto& entry : addr) {
            std::cout << "Resolved Address: " << entry.endpoint().address().to_string() << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Error in searchDevices: " << e.what() << "\n";
    }
}
