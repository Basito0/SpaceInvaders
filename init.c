#include <stdio.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){

    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);       

    // Crea una ventana 
    window = SDL_CreateWindow(
        "Space Invaders",                  
        SDL_WINDOWPOS_UNDEFINED,           
        SDL_WINDOWPOS_UNDEFINED,           
        640,                               
        480,                               
        SDL_WINDOW_OPENGL
    );

    // Verifica si la ventana se creó correctamente
    if (window == NULL) {
        // Imprime el error correspondiente
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Cierra la ventana
    SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
