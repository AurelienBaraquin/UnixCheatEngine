//  ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░░▒▓██████▓▒░▒▓████████▓▒░      ░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓████████▓▒░ 
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
// ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
// ░▒▓█▓▒░      ░▒▓████████▓▒░▒▓██████▓▒░ ░▒▓████████▓▒░ ░▒▓█▓▒░          ░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░   
// ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
//  ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░ 

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include "Tracer.hpp"

namespace CheatEngine::Memory {

class MemoryRegion {
public:
    uintptr_t startAddr;
    uintptr_t endAddr;
    std::string permissions;
    std::string pathname;

    MemoryRegion(uintptr_t start, uintptr_t end, const std::string& perms, const std::string& path)
        : startAddr(start), endAddr(end), permissions(perms), pathname(path) {}
};

class MemoryParser {
public:
    /**
     * @brief MemoryParser class constructor.
     * 
     * This constructor initializes a MemoryParser object with the given process ID (pid).
     * It also loads the memory regions of the process using the loadMemoryRegions() function.
     * 
     * @param pid The process ID of the target process.
     */
    explicit MemoryParser(pid_t pid) : pid(pid) {
        loadMemoryRegions();
    }

    /**
     * @brief Clear the memory regions by clearing the existing regions and loading new memory regions.
     */
    void clear() {
        regions.clear();
        data.clear();
        loadMemoryRegions();
    }

    // update all value in data by reading from memory
    template<typename T>
    void refresh() {
        Tracer tracer(pid);
        for (const auto addr : data) {
            data[addr.first] = tracer.read<T>(addr.first);
        }
    }

    template<typename T>
    void read() {
        filterRegions("r");
        Tracer tracer(pid);
        for (const auto& region : regions) {
            std::map<long, T> regionData = readRegion<T>(region, tracer);
            data.insert(regionData.begin(), regionData.end());
        }
    }

    template<typename T>
    void filterEqual(T value) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val == value) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterNotEqual(T value) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val != value) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterGreater(T value) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val > value) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterLess(T value) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val < value) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterGreaterEqual(T value) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val >= value) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterLessEqual(T value) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val <= value) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterRange(T lower, T upper) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val >= lower && val <= upper) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

    template<typename T>
    void filterNotRange(T lower, T upper) {
        std::map<long, T> filteredData;
        for (const auto& [addr, val] : data) {
            if (val < lower || val > upper) {
                filteredData[addr] = val;
            }
        }
        data.clear();
        data = filteredData;
    }

private:
    pid_t pid;
    std::vector<MemoryRegion> regions;
    std::map<long, std::vector<unsigned char>> data;

    /**
     * @brief Returns the vector of MemoryRegion objects.
     *
     * This function returns a constant reference to the vector of MemoryRegion objects.
     * The MemoryRegion class represents a memory region in the system.
     *
     * @return A constant reference to the vector of MemoryRegion objects.
     */
    const std::vector<MemoryRegion>& getRegions() const {
        return regions;
    }

    /**
     * Filters the memory regions based on the specified criteria.
     * 
     * @param type The type of memory region to filter.
     * @param pathname The pathname to filter the memory regions by. Default is an empty string.
     * @return A vector of MemoryRegion objects that match the specified criteria.
     */
    void filterRegions(std::string permissions, const std::string& pathname = "") {
        std::vector<MemoryRegion> filteredRegions;
        for (const auto& region : regions) {
            if ((permissions.empty() || region.permissions.find(permissions) != std::string::npos) &&
                (pathname.empty() || region.pathname.find(pathname) != std::string::npos)) {
                filteredRegions.push_back(region);
            }
        }
        regions.clear();
        regions = filteredRegions;
    }

    template<typename T>
    /**
     * @brief A map container that stores key-value pairs with keys of type long and values of type T.
     */
    std::map<long, T> readRegion(MemoryRegion region, Tracer tracer) {
        std::map<long, T> data;
        try {
            tracer.attach();
            for (long i = region.startAddr; i < region.endAddr; i += sizeof(T)) {
                T value = tracer.read<T>(i);
                data[i] = value;
            }
            tracer.detach();
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        return data;
    }

    void loadMemoryRegions() {
        std::ifstream mapsFile("/proc/" + std::to_string(pid) + "/maps");
        std::string line;

        while (std::getline(mapsFile, line)) {
            std::istringstream iss(line);
            std::string addr, perms, offset, dev, inode, path;
            iss >> addr >> perms >> offset >> dev >> inode;
            std::getline(iss, path);

            size_t dashPos = addr.find('-');
            std::string startStr = addr.substr(0, dashPos);
            std::string endStr = addr.substr(dashPos + 1);

            uintptr_t start = std::stoull(startStr, nullptr, 16);
            uintptr_t end = std::stoull(endStr, nullptr, 16);
            path = trim(path);

            regions.emplace_back(start, end, perms, path);
        }
    }

    std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};

}
