#include "Process.hpp"

std::vector<std::string> getProcessList() {
    std::vector<std::string> processList;
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("ps -A", "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        processList.push_back(buffer.data());
    }
    return processList;
}

std::vector<std::string> ProcessList = getProcessList();
