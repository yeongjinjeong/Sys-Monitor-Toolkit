#include "Network.h"

using namespace boost::asio;
using namespace boost::asio::ip;

void NetworkManager::NetworkTools::findUsedPorts() {
    try {
        std::set<int> used_ports;
        io_context context;
        tcp::acceptor acceptor(context);

        std::cout << "Currently Used Ports on Local System:\n";
        for (int port = 1; port <= 65535; ++port) {
            boost::system::error_code ec;
            acceptor.open(tcp::v4(), ec);
            if (ec) continue;

            acceptor.bind(tcp::endpoint(tcp::v4(), port), ec);
            if (ec == boost::asio::error::address_in_use) {
                used_ports.insert(port);
            }
            acceptor.close();
        }

        for (const auto& port : used_ports) {
            std::cout << "Used Port: " << port << "\n";
        }
    } catch (std::exception& e) {
        std::cerr << "Error in findUsedPorts: " << e.what() << "\n";
    }
}
