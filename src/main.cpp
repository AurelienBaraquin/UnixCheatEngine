//  ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░░▒▓██████▓▒░▒▓████████▓▒░      ░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓████████▓▒░ 
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
// ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
// ░▒▓█▓▒░      ░▒▓████████▓▒░▒▓██████▓▒░ ░▒▓████████▓▒░ ░▒▓█▓▒░          ░▒▓██████▓▒░ ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒▒▓███▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░   
// ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
// ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        
//  ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░          ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░░▒▓██████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░ 

#include "Tracer.hpp"
#include "Process.hpp"
#include "MemoryParser.hpp"
#include <raylib-cpp.hpp>
#include <iostream>

// #define GRAPHICAL

int main(int ac, char **av) {
    // CheatEngine::Memory::Tracer tracer(std::atoi(av[1]));

    // if (tracer.attach()) {
    //     std::cout << "Attached to process" << std::endl;
    // } else {
    //     std::cout << "Failed to attach to process" << std::endl;
    // }

    // if (tracer.detach()) {
    //     std::cout << "Detached from process" << std::endl;
    // } else {
    //     std::cout << "Failed to detach from process" << std::endl;
    // }
    std::cout << "process:" << std::endl;
    for (auto process : CheatEngine::Memory::ProcessList) {
        std::cout << process.first << " " << process.second << std::endl;
    }
    CheatEngine::Memory::MemoryParser parser(48514);
    parser.filterRegions("r-xp");
    auto regions = parser.getRegions();
    for (auto &region : regions) {
        std::cout << "Start: " << std::hex << region.startAddr << " End: " << std::hex << region.endAddr << " Permissions: " << region.permissions << " Pathname: " << region.pathname << std::endl;
    }

#ifdef GRAPHICAL
    raylib::Window w(800, 450, "Hello, Raylib!");

    while (!w.ShouldClose()) {
        w.BeginDrawing();
        w.ClearBackground();
        raylib::DrawText("Congrats! You created your first window!", 190, 200, 20, (Color)RAYWHITE);
        w.EndDrawing();
    }
#endif

    return 0;
}
