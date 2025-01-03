#pragma once
#include "Process.h"
#include <memory>

class ProcessManager {
private:
    std::unique_ptr<Process> processHandler;

public:
    ProcessManager();
    ~ProcessManager();

    void run();  // 현재의 main 로직을 여기로 이동
    void showMenu();
};
