#include "system.h"

system::float getCPUUsage() {
    std::ifstream procStat("/proc/stat");
    std::string line;
    std::getline(procStat, line);
    std::istringstream ss(line);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long totalIdle = idle + iowait;
    long totalNonIdle = user + nice + system + irq + softirq + steal;
    long total = totalIdle + totalNonIdle;

    static long prevTotal = 0, prevIdle = 0;
    float cpuPercentage = (total - prevTotal == 0) ? 0.0 : 
                            (1.0 * (total - prevTotal - (totalIdle - prevIdle)) / (total - prevTotal)) * 100;

    prevTotal = total;
    prevIdle = totalIdle;

    return cpuPercentage;     
}