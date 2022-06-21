//cambios de la megumin 
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <math.h>
//crea variables globales
//tamaño de ventana
#define XSIZE 1280
#define YSIZE 720
//tiempo de espera en milisegundos
#define MS 10
//tamaño y velocidad de la nave
#define NAVE_LEN 20
#define NAVE_VEL 10
//tamaño y velocidad del misil
#define MISIL_LEN 5
#define MISIL_VEL 5
//se definen 2 estructuras
//la estructura de tipo Nave con nombre Nave
typedef struct Nave Nave;
//la estructura de tipo Misil con nombre Misil
typedef struct Misil Misil;
//definimos que la figura de la nave esta compuesta por 3 puntos,
//x1,y1,..., son las coordenadas de cada punto de la nave, la nave es un triangulo
//por lo tanto, cada punto es un vertice
//ademas vx y vy es la velocidad de la nave en el eje x y en el eje y respectivamente
struct Nave{
	int x1,y1;
	int x2,y2;
	int x3,y3;
	int vx,vy;
	Misil *misiles; //a la nave se le enlaza una lista de misiles para que pueda disparar	
};
//se crea la estructura del misil, los misiles son lineas, la posicion x1 y x2 definiran
//el grosor del misil, y1,y2 el largo del misil
struct Misil{
	int x1,y1;
	int x2,y2;
	int vx,vy;
	Misil *siguiente;//se le enlaza el misil siguiente
};
//las funciones estan definidas al final 

//En resumen, carga una textura y la guarda en un puntero(?
SDL_Texture* LoadTexture(const char* filepath, SDL_Renderer* renderTarget){
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(filepath);
	texture = SDL_CreateTextureFromSurface(renderTarget, surface);
	SDL_FreeSurface(surface);

	return texture;
}
//esta funcion elimina los misiles que salgan de la pantalla que estan ocupando memoria dinamica
//recibe los misiles
void MisilBorraObsoletos(Misil *misiles);

void NaveAvanzaArrAba(Nave *nave, SDL_Rect *ship, double *radio, double *delta);

void NaveAvanzaIzqDer(Nave *nave, SDL_Rect *ship, int *vx, double *radio, double *delta);
//esta funcion hace que los misiles avancen, anvanza por el eje y
void MisilAvanza(Misil *misil);

//funcion que pinta el borde de la nave (dibuja el triangulo)
void NavePinta(Nave *nave, SDL_Renderer *renderer);

//funcion que hace que la nave dispare
void NaveDispara(Nave *nave);

int main(int argc, char* argv[]){
	//si SDL no se inicia lanza una ventana de error
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",SDL_GetError(),NULL);
	}
	//en caso contrario crea una ventana con nombre "Space Invaders"
	//y lanza el mensaje GAME READY
	else{		
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Space Invaders","GAME READY",NULL);
	}
	
	int typeEvent;//crea una variable entera typeEvent para almacenar la entrada de tecla
	int gameOver;//crea una variable entera para controlar si sigue dentro del ciclo del juego o no 
	
	const unsigned char *keys;//crea una constante char sin signo para identificar la tecla que se pulsa
	
	SDL_Window *window;//crea un puntero de tipo ventana con nombre window
	SDL_Renderer *renderer;//crea un puntero de tipo Render con nombre renderer
	SDL_Event event;//crea una variable de tipo evento
	
	//**
	//aqui le damos las dimensiones a la nave, el ultimo parametro NULL es que no tiene misiles
	Nave nave ={XSIZE/2,YSIZE/2,XSIZE/2-NAVE_LEN,YSIZE/2+NAVE_LEN,XSIZE/2+NAVE_LEN,YSIZE/2+NAVE_LEN,NAVE_VEL,NAVE_VEL,NULL};
/*
Creamos un puntero que crea la ventana del juego, el nombre de la ventana es Space Invader, XSIZE es el ancho, definido como 600. YSIXE es el alto, definido como 600
los dos primeros parametros son para establecer en que posicion se abre la venta, y el ultimo muestra la ventana
renderer renderiza la imagen en la ventana
*/		
	
	window= SDL_CreateWindow("Space Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,XSIZE,YSIZE,SDL_WINDOW_SHOWN);
	
	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	
	gameOver=0;//le asigna 0 a la variable gameOver (0 es Falso)
	
	keys=SDL_GetKeyboardState(NULL);//se le asigna a keys la funcion GetKeyboardState, que detecta que tecla fue pulsada, la tecla presionada se guarda en key
	
	SDL_Texture* ship_texture = LoadTexture("Resources/Sprites/Nave_new.png", renderer);
	SDL_Rect ship;

	//Variables para el movimiento de la nave
	float ox, oy;
	double radio = 100;
	ox = (XSIZE - ship.w)/2 + radio;
	oy = (YSIZE - ship.h)/2 + radio;
	double delta = asin(0);

	//pide la posición inicial de la nave (cambiar xsize)

	float x_pos = XSIZE;
	//Posiciona el sprite de la nave
	ship.x = (int)(radio*sin(delta) + XSIZE/2);
	ship.y = (int)(radio*cos(delta) + YSIZE/2);



	while(!gameOver){//se niega gameOver para que se considere verdadera, mientras sea "verdadera" tb se puede usar la condicion gameOver==0
		if(SDL_PollEvent(&event)){ //si se detecta la pulsacion de una tecla
			typeEvent=event.type; //ve que tipo de tecla se pulso
			if(typeEvent== SDL_QUIT){ //si se presiono la x que cierra la ventana
				gameOver=1; //termina el juego
			}else if(typeEvent==SDL_MOUSEBUTTONDOWN){ //si se detecta la pulsacion del mouse
				if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){ //si es el clicl izq
					NaveDispara(&nave); //la nave dispara
				}
				
			}else if(typeEvent == SDL_KEYDOWN){ //si se detecta la pulsacion de una tecla
				if(keys[SDL_SCANCODE_ESCAPE]){	//guarda el tipo de tecla en keys
					gameOver=1; //si la tecla es ESC el juego termina
				}else if(keys[SDL_SCANCODE_LEFT]){ //si es la flecha izquierda, la nave se mueve hacia la izquierda
					int vx = -1; //la velocidad de la nave en el eje x se vuelve negativa
					NaveAvanzaIzqDer(&nave, &ship, &vx, &radio, &delta); //la nave se mueve
				}else if(keys[SDL_SCANCODE_RIGHT]){ //movimiento hacia la derecha
					int vx = 1; //la velocidad de la nave se hace positiva
					NaveAvanzaIzqDer(&nave, &ship, &vx, &radio, &delta); //la nave se mueve
				}else if(keys[SDL_SCANCODE_SPACE]){ //si se presiona el espacio
					NaveDispara(&nave); //la nave dispara
				}else if(keys[SDL_SCANCODE_UP]){ //la nave sube
					nave.vy=-abs(nave.vy); //para que la nave suba, la velocidad en el eje y debe ser negativa, ni idea pq
					NaveAvanzaArrAba(&nave, &ship, &radio, &delta); //la nave sube
				}else if(keys[SDL_SCANCODE_DOWN]){ //la nave baja
					nave.vy=+abs(nave.vy); //para que baje la aceleracion debe ser positiva
					NaveAvanzaArrAba(&nave, &ship, &radio, &delta); //la nave baja
				}
			}		
		}
		MisilBorraObsoletos(nave.misiles); // luego de cada movimiento, se borran los misiles que no aparezcan en pantalla
		//pantalla


		//Pide info de texturas
		SDL_QueryTexture(ship_texture, NULL, NULL, &ship.w, &ship.h);

		SDL_SetRenderDrawColor(renderer,0,0,0,0); //pinta la pantalla negra
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, ship_texture, NULL, &ship);
		//pinta la nave de blanco
		SDL_SetRenderDrawColor(renderer,255,255,255,0);
		NavePinta(&nave,renderer);
		//presenta la imagen, creo
		SDL_RenderPresent(renderer);
		SDL_Delay(MS);		
	}
	
	//finaliza todo
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}


void NavePinta(Nave *nave, SDL_Renderer *renderer){
	//pinta misiles
	Misil *ixMisil=nave->misiles;
	while(ixMisil!=NULL){
		MisilAvanza(ixMisil);
		SDL_RenderDrawLine(renderer,ixMisil->x1,ixMisil->y1,ixMisil->x2,ixMisil->y2);
		ixMisil=ixMisil->siguiente;
	}
	//pinta nave
	SDL_Point points[4]={
		{nave->x1,nave->y1},
		{nave->x2,nave->y2},
		{nave->x3,nave->y3},
	};
	//para pintar la nave en vez de recorrer 3 puntos recorre 4, para dibujar el borde completo del triangulo
	points[3]=points[0];
	SDL_RenderDrawLines(renderer,points,4);//pinta el triangulo
	

}

//para moverse de izquierda a derecha, dependera de la velocidad en el eje x
//si la velocidad en el eje x es negativa se mueve hacia la izquierda
//en caso contrario se mueve hacia la derecha
void NaveAvanzaIzqDer(Nave *nave, SDL_Rect *ship, int *vx, double *radio, double *delta){
	*delta += *vx*M_PI/30;
	ship->x = (int)(*radio*sin(*delta) + XSIZE/2);
	ship->y = (int)(*radio*cos(*delta) + YSIZE/2);
	printf("%d\n %d\n", ship->x, ship->y);


	nave->x1+=nave->vx;
	nave->x2+=nave->vx;
	nave->x3+=nave->vx;
}
//ocurre lo mismo que en el movimiento horizontal, solo que si la velocidad es negativa
//la nave sube, y si es positiva la nave baja
void NaveAvanzaArrAba(Nave *nave, SDL_Rect *ship, double *radio, double *delta){
	ship->y += nave->vy;
	nave->y1+=nave->vy;
	nave->y2+=nave->vy;
	nave->y3+=nave->vy;
}
//en resumen, en esta parte se crea un arreglo de misiles con memoria dinamica
//el primer if verifica si la nave tiene misiles ir a (** mas arriba), la configure para que parta sin misiles
//luego, en el else, si no tiene misiles, los crea y los guarda en la nave
void NaveDispara(Nave *nave){
	if(nave->misiles==NULL){
		nave->misiles=(Misil *)SDL_malloc(sizeof(Misil));
		nave->misiles->x1=nave->x1;
		nave->misiles->y1=nave->y1;
		nave->misiles->x2=nave->x1;
		nave->misiles->y2=nave->y1 -MISIL_LEN;
		nave->misiles->vx=0;
		nave->misiles->vy=-MISIL_VEL;
		nave->misiles->siguiente=NULL;
	}else{
		Misil *ixMisil =nave->misiles;
		while(ixMisil->siguiente!=NULL){
			ixMisil=ixMisil->siguiente;	
		}
		ixMisil->siguiente=(Misil *)SDL_malloc(sizeof(Misil));
		ixMisil=ixMisil->siguiente;
		ixMisil->x1=nave->x1;
		ixMisil->y1=nave->y1;
		ixMisil->x2=nave->x1;
		ixMisil->y2=nave->y1-MISIL_LEN;
		ixMisil->vx=0;
		ixMisil->vy=-MISIL_VEL;
		ixMisil->siguiente=NULL;
	}
}

//esto hace que los misiles se muevan hacia arriba, la velocidad siempre es negativa
//parten de la posicion de la nave
void MisilAvanza(Misil *misil){
	misil->x1+=misil->vx;
	misil->y1+=misil->vy;
	misil->x2+=misil->vx;
	misil->y2+=misil->vy;
}
//si no hay misiles no hace nada
//mientras hayan misiles, si estan fuera de la pantalla los borra
//en caso contrario no hace nada
//creo que era asi
void MisilBorraObsoletos(Misil *misiles){
	Misil * ixMisil=misiles;
	if(ixMisil==NULL){
		return;
	}
	Misil *auxMisil;
	while(ixMisil->siguiente!=NULL){
		if(ixMisil->siguiente->y1<0){
			auxMisil=ixMisil->siguiente->siguiente;
			SDL_free(ixMisil->siguiente);
			ixMisil->siguiente=auxMisil;
		}else{
			ixMisil=ixMisil->siguiente;
		}
	}
}
//EXPLOOOOOOOSION