//cambios de la megumin 
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define XSIZE 600
#define YSIZE 600
#define MS 10

typedef struct Nave Nave;
typedef struct Misil Misil;

struct Nave{
	int x1,y1;
	int x2,y2;
	int x3,y3;
	int vx,vy;
	Misil *misiles;
};

struct Misil{
	int x1,y1;
	int x1,y2;
	int vx,vy;
};

int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",SDL_GetError(),NULL);
	}else{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Space Invaders","GAME READY",NULL);
	}
	
	int typeEvent;
	int gameOver;
	
	const unsigned char *keys;
	
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	
	window= SDL_CreateWindow("Space Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,XSIZE,YSIZE,SDL_WINDOW_SHOWN);
	
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	
	gameOver=0;
	
	keys=SDL_GetKeyboardState(NULL);
	
	while(!gameOver){
		if(SDL_PollEvent(&event)){
			typeEvent=event.type;
			if(typeEvent== SDL_QUIT){
				gameOver=1;
			}else if(typeEvent==SDL_MOUSEBUTTONDOWN){
				if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){
					//gameOver=1;
				}
				
			}else if(typeEvent == SDL_KEYDOWN){
				if(keys[SDL_SCANCODE_ESCAPE]){
					gameOver=1;
				}else if(keys[SDL_SCANCODE_LEFT]){
				
				}else if(keys[SDL_SCANCODE_RIGHT]){
				
				}else if(keys[SDL_SCANCODE_SPACE]){
				
				}
			}		
		}
		//pantalla
		SDL_SetRenderDrawColor(renderer,0,0,0,0); //pinta la pantalla negra
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		SDL_Delay(MS);
		
		
	}
	
	
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}
//EXPLOOOOOOOSION
