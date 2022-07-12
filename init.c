//cambios de la megumin 
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
//crea variables globales
//tamaño de ventana
#define XSIZE 600
#define YSIZE 400
//tiempo de espera en milisegundos
#define MS 10
//tamaño y velocidad del misil
#define MISIL_LEN 5
#define MISIL_VEL 5



//las funciones estan definidas al final 

//En resumen, carga una textura y la guarda en un puntero(?
SDL_Texture* LoadTexture(const char* filepath, SDL_Renderer* renderTarget){
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(filepath);
	texture = SDL_CreateTextureFromSurface(renderTarget, surface);
	SDL_FreeSurface(surface);

	return texture;
}

//Función que mueve la nave de forma circunferencial
void NaveAvanzaIzqDer(Uint32 *dash, SDL_Rect *ship, int *vx, double *radio, double *delta);

//funcion que hace que la nave dispare
void NaveDispara(Uint32 *time, SDL_Rect *ship, SDL_Rect p_bullet[], int balas[10][3],SDL_Renderer *renderer, SDL_Texture *texture);

//Spawnea aliens
void AlienSpawn(SDL_Rect *ship, SDL_Rect *aliens, double alieninfo[100][6], Uint32 *spawntime, int radio);

int main(int argc, char* argv[]){
	    int gameOver;
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "Jugar" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Instrucciones" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Salir" },
        {3,3,"Hola"}
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 0,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   255, 255,   255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Space Invaders", /* .title */
        "Selecciona una opción", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };


    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return 1;
    }
    if (buttonid == -1) {
        SDL_Log("no selection");
    } else if(buttonid == 0){
        gameOver = 0;
        printf("%d\n",gameOver);
    } else if(buttonid == 1){        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Space Invaders","Instrucciones",NULL);
    } else if(buttonid == 2){
        return 0;
    } else if(buttonid == 3){
    	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"HOLA","hola",NULL);
    	return 0;
    }


    else {
        SDL_Log("selection was %s", buttons[buttonid].text);
    }
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
	//crea una variable entera para controlar si sigue dentro del ciclo del juego o no 
	
	const unsigned char *keys;//crea una constante char sin signo para identificar la tecla que se pulsa
	
	SDL_Window *window;//crea un puntero de tipo ventana con nombre window
	SDL_Renderer *renderer;//crea un puntero de tipo Render con nombre renderer
	SDL_Event event;//crea una variable de tipo evento
	
	/*
	Creamos un puntero que crea la ventana del juego, el nombre de la ventana es Space Invader, XSIZE es el ancho, definido como 600. YSIXE es el alto, definido como 600
	los dos primeros parametros son para establecer en que posicion se abre la venta, y el ultimo muestra la ventana
	renderer renderiza la imagen en la ventana
	*/		
	
	window= SDL_CreateWindow("Space Invaders",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,XSIZE,YSIZE,SDL_WINDOW_SHOWN);
	
	//Initialize SDL_mixer
    if( Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	//le asigna 0 a la variable gameOver (0 es Falso)
	
	keys=SDL_GetKeyboardState(NULL);//se le asigna a keys la funcion GetKeyboardState, que detecta que tecla fue pulsada, la tecla presionada se guarda en key
	
	//ACA ASIGNAMOS TEXTURAS Y COLISIONES
	SDL_Texture* ship_texture = LoadTexture("Resources/Sprites/Nave_new.png", renderer);
	SDL_Texture* b_Texture = LoadTexture("./Resources/Sprites/Bullet_01.png", renderer);
	SDL_Texture* al_Texture = LoadTexture("Resources/Sprites/Alien1_1.png", renderer);
	SDL_Rect ship;

	//Punteros para música
	Mix_Music *tema = NULL;

	//Para efectos de sonido
	Mix_Chunk *gScratch = NULL;
	Mix_Chunk *gHigh = NULL;
	Mix_Chunk *gMedium = NULL;
	Mix_Chunk *gLow = NULL;

	tema = Mix_LoadMUS("Resources/Audio/Music/Long_live_the_new_fresh.wav");

	//Variables para el movimiento de la nave
	float ox, oy;
	double radio = 200;
	ox = (XSIZE - ship.w)/2 + radio;
	oy = (YSIZE - ship.h)/2 + radio;
	double delta = asin(0);

	//Posiciona el sprite de la nave
	ship.x = (int)(radio*sin(delta) + XSIZE/2);
	ship.y = (int)(radio*cos(delta) + YSIZE/2);

	//Balas del jugador, velocidad en x e y
	int balas[10][3];
	SDL_Rect p_bullet[10];

	//Aliens y su info
	//[x][0] indica inicialización
	//[x][1] indica el tipo de movimiento (1= recto, 2= espiral)
	//[x][2] variable ángulo
	//[x][3] variable radio
	//[x][4] Velocidad inicial x
	//[x][5] Velocidad inicial y
	double alieninfo[100][6]; for(int i=0; i<100; i++){alieninfo[i][0] = 0;}
	SDL_Rect aliens[100];

	//comienza un contador
	srand(SDL_GetTicks());
	Uint32 dashTime = SDL_GetTicks();
	Uint32 bulletTime = SDL_GetTicks();
	Uint32 spawnTime = SDL_GetTicks();
	


	while(gameOver == 0){//se niega gameOver para que se considere verdadera, mientras sea "verdadera" tb se puede usar la condicion gameOver==0
		
		//RenderClear limpia la ventana con un color X
		SDL_RenderClear(renderer);
		if(SDL_PollEvent(&event)){ //si se detecta la pulsacion de una tecla
			typeEvent=event.type; //ve que tipo de tecla se pulso
			if(typeEvent== SDL_QUIT){ //si se presiono la x que cierra la ventana
				gameOver=1; //termina el juego
			}
			else if(typeEvent==SDL_MOUSEBUTTONDOWN){ //si se detecta la pulsacion del mouse
				if(SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)){ //si es el clicl izq
					NaveDispara(&bulletTime, &ship, p_bullet, balas, renderer, b_Texture); //la nave dispara
				}
				
			}
			else if(typeEvent == SDL_KEYDOWN){ //si se detecta la pulsacion de una tecla
				if(keys[SDL_SCANCODE_ESCAPE]){	//guarda el tipo de tecla en keys
					gameOver=1; //si la tecla es ESC el juego termina
				}else if(keys[SDL_SCANCODE_LEFT]){ //si es la flecha izquierda, la nave se mueve hacia la izquierda
					int vx = -1; //la velocidad de la nave en el eje x se vuelve negativa
					NaveAvanzaIzqDer(&dashTime, &ship, &vx, &radio, &delta); //la nave se mueve
				}else if(keys[SDL_SCANCODE_RIGHT]){ //movimiento hacia la derecha
					int vx = 1; //la velocidad de la nave se hace positiva
					NaveAvanzaIzqDer(&dashTime, &ship, &vx, &radio, &delta); //la nave se mueve
				}else if(keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP]){ //si se presiona el espacio
					NaveDispara(&bulletTime, &ship, p_bullet, balas, renderer, b_Texture); //la nave dispara
				}

				//COMANDOS DE DESARROLLO
				else if(keys[SDL_SCANCODE_UP]){ //Empieza musica
					//Play the music
        			Mix_PlayMusic(tema, -1 );
				}
			}		
		}
		//Aparece un alien cada x milisegundos
		AlienSpawn(&ship, aliens, alieninfo, &spawnTime, radio);

		//Pide info de texturas
		SDL_QueryTexture(ship_texture, NULL, NULL, &ship.w, &ship.h);

		//Hace cosas con las balas del jugador
		for (int i = 0; i < 10; i++)
		{
			if (balas[i][0] == 0){
				p_bullet[i].x = XSIZE;
				p_bullet[i].y = YSIZE;
			}
			if (balas[i][0] == 1)
			{
				p_bullet[i].x += balas[i][1]/10;
				p_bullet[i].y += balas[i][2]/10;
				SDL_QueryTexture(b_Texture, NULL, NULL, &p_bullet[i].w, &p_bullet[i].h);
				SDL_RenderCopy(renderer, b_Texture, NULL, &p_bullet[i]);
			}
			if (p_bullet[i].x < 0 || p_bullet[i].y < 0 || p_bullet[i].x > XSIZE || p_bullet[i].y > YSIZE){
				balas[i][0] = 0;
			}
		}

		//Hace cosas con los aliens
		for (int i = 0; i < 100; i++)
		{
			if (alieninfo[i][0] == 0){
				aliens[i].x = 0;
				aliens[i].y = 0;
			}
			if (alieninfo[i][0] == 1)
			{
				for(int u=0; u<10; u++){
					if(SDL_HasIntersection(&p_bullet[u], &aliens[i]) == SDL_TRUE){
						alieninfo[i][0] = 0;
						balas[u][0] = 0;
					}
				}
				if(SDL_HasIntersection(&ship, &aliens[i]) == SDL_TRUE){
					alieninfo[i][0] = 0;
				}

				if(alieninfo[i][1] == 1){
					aliens[i].x += alieninfo[i][4];
					aliens[i].y += alieninfo[i][5];
				}
				if(alieninfo[i][1] == 2){
					alieninfo[i][3] += (0.1);
					alieninfo[i][2] += 1*M_PI/120;
					aliens[i].x = (int)(alieninfo[i][3]*sin(alieninfo[i][2]) + (XSIZE - aliens->w)/2);
					aliens[i].y = (int)(alieninfo[i][3]*cos(alieninfo[i][2]) + (YSIZE - aliens->h)/2);
				}
				SDL_QueryTexture(al_Texture, NULL, NULL, &aliens[i].w, &aliens[i].h);
				SDL_RenderCopy(renderer, al_Texture, NULL, &aliens[i]);
			}
			if (aliens[i].x < 0 || aliens[i].y < 0 || aliens[i].x > XSIZE || aliens[i].y > YSIZE){
				alieninfo[i][0] = 0;
			}
		}

		SDL_RenderCopy(renderer, ship_texture, NULL, &ship);
		//presenta la imagen, creo
		SDL_RenderPresent(renderer);
		SDL_Delay(MS);	
	}
	
	//finaliza todo
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	SDL_Quit();
	
	return 0;
}

//para moverse de izquierda a derecha, dependera de la velocidad en el eje x
//si la velocidad en el eje x es negativa se mueve hacia la izquierda
//en caso contrario se mueve hacia la derecha
void NaveAvanzaIzqDer(Uint32 *dash, SDL_Rect *ship, int *vx, double *radio, double *delta){

	Uint32 tiempo_actual = SDL_GetTicks();
	const unsigned char *keys;
	keys=SDL_GetKeyboardState(NULL);

	*delta += *vx*M_PI/45;

	if(keys[SDL_SCANCODE_SPACE] && (tiempo_actual - *dash) > 500){ //si se presiona el espacio y han pasado x segundos
		*delta += *vx*M_PI/10;
		*dash = tiempo_actual;
	}
	ship->x = (int)(*radio*sin(*delta) + (XSIZE - ship->w)/2);
	ship->y = (int)(*radio*cos(*delta) + (YSIZE - ship->h)/2);
}


void NaveDispara(Uint32 *time, SDL_Rect *ship, SDL_Rect *p_bullet, int balas[10][3], SDL_Renderer *renderer, SDL_Texture *texture){

	Uint32 tiempo_actual = SDL_GetTicks();
	for (int i = 0; i < 10; i++)
	{
		//1 significa inicializado. 0 es una bala disponible
		if(balas[i][0] == 0 && (tiempo_actual - *time) > 500){
			balas[i][0] = 1;
			p_bullet[i].x = ship->x;
			p_bullet[i].y = ship->y;

			//Calcula la velocidad de la bala
			balas[i][1] = -1*(p_bullet[i].x - (XSIZE/2));
			balas[i][2] = -1*(p_bullet[i].y - (YSIZE/2));

			//Dibuja la bala en pantalla
			SDL_QueryTexture(texture, NULL, NULL, &p_bullet[i].w, &p_bullet[i].h);
			SDL_RenderCopy(renderer, texture, NULL, &p_bullet[i]);

			//reinicia time
			*time = tiempo_actual;
			break;
		}
	}
}

void AlienSpawn(SDL_Rect *ship, SDL_Rect *aliens, double alieninfo[100][6], Uint32 *spawntime, int radio){

	Uint32 tiempo_actual = SDL_GetTicks();
	int tipo = rand() % 101;

	for (int i = 0; i < 20; i++)
	{
		//1 significa inicializado. 0 es una bala disponible
		if(alieninfo[i][0] == 0 && tiempo_actual - *spawntime > 100){
			alieninfo[i][0] = 1;
			aliens[i].x = XSIZE/2;
			aliens[i].y = YSIZE/2 - 1;
			if(tipo <= 50){
				alieninfo[i][1] = 1;
				if((ship->x - (XSIZE + ship->w)/2)/20 > radio && (ship->y - (YSIZE + ship->h)/2)/20 > radio - 50){
					alieninfo[i][4] = (ship->x - (XSIZE + ship->w)/2)/20;
					alieninfo[i][5] = (ship->y - (YSIZE + ship->h)/2)/20;
				}
			}
			if(tipo > 50){				
				alieninfo[i][1] = 2;
				alieninfo[i][2] = 0;
				alieninfo[i][3] = 1;
				
			}

			*spawntime = tiempo_actual;
			break;
		}
	}
}
//EXPLOOOOOSION
