#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

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

    // Create a Renderer
    SDL_Renderer* rend = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC
    );

    //Load Images
    SDL_Surface* surface = IMG_Load("Resources/Sprites/Nave_new.png"); 

    //Create textures from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);

    // Clear the window
    SDL_RenderClear(rend);

    SDL_RenderCopy(rend, texture, NULL, NULL);
    SDL_RenderPresent(rend);

    SDL_Delay(5000);
    // Close and destroy the window
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
