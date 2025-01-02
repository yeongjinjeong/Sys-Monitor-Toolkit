#include "system.h"

system::void getDiskUsage(const std::string& path) {
    struct statvfs diskInfo;    
    if (statvfs(path.c_str(), &diskInfo) == 0) {
        unsigned long long totalSpace = diskInfo.f_blocks * diskInfo.f_frsize;
        unsigned long long freeSpace = diskInfo.f_bfree * diskInfo.f_frsize;
        unsigned long long usedSpace = totalSpace - freeSpace;

        std::cout << "Disk Usage (" << path << "): " 
                  << std::fixed << std::setprecision(2)
                  << usedSpace / (1024.0 * 1024 * 1024) << " GB / "
                  << totalSpace / (1024.0 * 1024 * 1024) << " GB" << std::endl;
    } else {
        std::cerr << "Error retrieving disk usage for path: " << path << std::endl;
    }
}