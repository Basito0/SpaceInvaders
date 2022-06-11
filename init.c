//cambios de la megumin 
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

//crea variables locales para el tamaño de la ventana y el tiempo de espera
#define XSIZE 600
#define YSIZE 600
#define MS 10

//define dos estructuras, para la nave y el misisl
typedef struct Nave Nave;
typedef struct Misil Misil;
//a la nave le agrega 
struct Nave{ //la nave es un triangulo,cada punto es un vertice
	int x1,y1;
	int x2,y2;
	int x3,y3; 
	int vx,vy; //velocidad en el eje X E Y
	Misil *misiles;
};

struct Misil{ //el misil es una linea, por eso son dos puntos, y vx y vy son las velocidades en los ejes
	int x1,y1;
	int x2,y2;
	int vx,vy;
};

int main(int argc, char* argv[]){
	
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){ //si SDL no se inicia lanza una ventana de error
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",SDL_GetError(),NULL); //crea una ventana de error con el mensaje "Error"
	}else{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Space Invaders","GAME READY",NULL); //en caso contrario lanza una ventana con nombre "Space Invaders", la ventana lanza el mensaje "GAME READY"
	}
	
	int typeEvent; //crea una variable entera typeEvent para almacenar la entrada de tecla
	int gameOver; //crea una variable entera para controlar si sigue dentro del ciclo del juego o no 
	
	const unsigned char *keys; //crea una constante char sin signo para identificar la tecla que se pulsa
	
	SDL_Window *window;  //crea un puntero de tipo ventana con nombre window
	SDL_Renderer *renderer;	//crea un puntero de tipo Render con nombre renderer
	SDL_Event event; //crea una variable de tipo evento
	
	
/*
Creamos un puntero que crea la ventana del juego, el nombre de la ventana es Space Invader, XSIZE es el ancho, definido como 600. YSIXE es el alto, definido como 600
los dos primeros parametros son para establecer en que posicion se abre la venta, y el ultimo muestra la ventana

renderer renderiza la imagen en la ventana
*/	
	window= SDL_CreateWindow("Space Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,XSIZE,YSIZE,SDL_WINDOW_SHOWN); 
	
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	
	gameOver=0; //le asigna 0 a la variable gameOver (0 es Falso)
	
	keys=SDL_GetKeyboardState(NULL); //se le asigna a keys la funcion GetKeyboardState, que detecta que tecla fue pulsada, la tecla presionada se guarda en key
	
	while(!gameOver){ //se niega gameOver para que se considere verdadera, mientras sea "verdadera" tb se puede usar la condicion gameOver==0
		if(SDL_PollEvent(&event)){//si detecta la pulsacion de una tecla
			typeEvent=event.type;//guarda la tecla que se pulso en typeEvent
			if(typeEvent== SDL_QUIT){//si apretan la 'x' en la ventana, la ventana se cierra
				gameOver=1;//gameOver es 1, el ciclo termina, y entonces el ciclo no continua
			}else if(typeEvent==SDL_MOUSEBUTTONDOWN){//si typeEvent detecta una pulsacion de la tecla del mouse
				if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){	//operacion bit a bit, toma el estado del mouse y si se pulsa el mouse izquierdo, entonces el programa se cierra
					//gameOver=1;//sale del ciclo
				}
				
			}else if(typeEvent == SDL_KEYDOWN){//si typeEvent es la pulsacion de una tecla
				if(keys[SDL_SCANCODE_ESCAPE]){//si se apreta escape (ESC), la ventana se cierra 
					gameOver=1;//el ciclo termina
				}else if(keys[SDL_SCANCODE_LEFT]){//si se apreta la flecha hacia la izquiera
				
				}else if(keys[SDL_SCANCODE_RIGHT]){ //si se apreta la flecha hacia la derecha
				
				}else if(keys[SDL_SCANCODE_SPACE]){ //si se apreta la barra espaciadora
				
				}
			}		
		}
		
		//pantalla: para mostar algo
		SDL_SetRenderDrawColor(renderer,0,0,0,0); //pinta la pantalla negra
		
		//renderiza la ventana, creo
		SDL_RenderClear(renderer); 
		SDL_RenderPresent(renderer);
		
		//crea un delay entre cada frame de imagen, como la ventana es negra, no se va a notar
		SDL_Delay(MS);//MS es una variable global, SDL_Delay mide el tiempo en milisegundos
		
		
	}
	
	
	//termina todo
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}
//EXPLOOOOOOOSION
