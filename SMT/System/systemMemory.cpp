#include "System.h"

void System::getMemoryUsage() {
    std::ifstream memInfo("/proc/meminfo");
    std::string line;
    long totalMem = 0, freeMem = 0, availableMem = 0;

    while (std::getline(memInfo, line)) {
        std::istringstream ss(line);
        std::string key;
        long value;
        std::string unit;
        ss >> key >> value >> unit;

        if (key == "MemTotal:") totalMem = value;
        else if (key == "MemFree:") freeMem = value;
        else if (key == "MemAvailable:") availableMem = value;
    }

    std::cout << "Memory Usage: " << (totalMem - availableMem) / 1024 << " MB / "
              << totalMem / 1024 << " MB" << std::endl;
}