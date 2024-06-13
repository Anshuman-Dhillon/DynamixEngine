//Developed by Anshuman Dhillon


#include <SDL.h>
#include <iostream>
#include "PhysicsEngine.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    
    SDL_Window* window = SDL_CreateWindow("2D Physics Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Fixed delta time (60 FPS)
    const float FIXED_DELTA_TIME = 0.016f;
    
    PhysicsEngine engine;

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Add some objects to the physics engine
    engine.addObject(200, 200, 10.0f, 100, 100);
    engine.addObject(400, 300, 15.0f, 150, 150);
    engine.addObject(50, 50, 5.0f, 50, 50);
    engine.addObject(500, 100, 7.5f, 75, 75);
    engine.addObject(600, 100, 7.5f, 800, 30);

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    SDL_RaiseWindow(window);

    // Main loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update the physics engine with fixed delta time
        engine.update(FIXED_DELTA_TIME, windowHeight, windowWidth);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Render all objects
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        for (const auto& obj : engine.getObjects()) {
            SDL_Rect fillRect = { static_cast<int>(obj.x), static_cast<int>(obj.y), obj.width, obj.height };
            SDL_RenderFillRect(renderer, &fillRect);
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}