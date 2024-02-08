#include "Tracer.hpp"
#include <raylib-cpp.hpp>
#include <iostream>

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

    raylib::Window w(800, 450, "Hello, Raylib!");

    while (!w.ShouldClose()) {
        w.BeginDrawing();
        w.ClearBackground();
        raylib::DrawText("Congrats! You created your first window!", 190, 200, 20, (Color)RAYWHITE);
        w.EndDrawing();
    }

    return 0;
}
