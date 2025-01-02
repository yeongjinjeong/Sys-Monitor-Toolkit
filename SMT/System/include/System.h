#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <sys/statvfs.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <unistd.h>

class System{
public:
    float getCPUUsage();
    void getMemoryUsage();
    void getDiskUsage(const std::string& path = "/");
    void run(){
        std::cout << "System Monitor (CLI)" << std::endl;
        while (true) {
            std::cout << "CPU Usage: " << getCPUUsage() << "%\n";
            getMemoryUsage();
            getDiskUsage("/");
            sleep(1);        
        }
    }
}