#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);       

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Space Invaders",                  
        SDL_WINDOWPOS_UNDEFINED,           
        SDL_WINDOWPOS_UNDEFINED,           
        640,                               
        480,                               
        SDL_WINDOW_OPENGL
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}