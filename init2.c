/*
Falta:
- leer score, guardar score , modificar score (en resumen)
	-Hacer el top, comparar el puntaje obtenido en el top y modificarlo
-agregar nuevas mecanicas
	-caja "bonus"
		- te nerfea
		-o te da bonus
int
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <limits.h>
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

SDL_Texture* LoadFromRenderedText(const char* textureText, TTF_Font *gFont, SDL_Renderer* renderer, SDL_Color textColor){
	//Lo mismo que la función anterior pero con texto
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
	if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
	SDL_Texture* mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return mTexture;
}
//Pa pasar de entero a caracter
char *my_itoa(int num, char *str)
{
        sprintf(str, "%d", num);
        return str;
}
void Top(FILE *archivo, int puntaje,char *puntos);
//Función que mueve la nave de forma circunferencial
void NaveAvanzaIzqDer(Uint32 *dash, SDL_Rect *ship, int *vx, double *radio, double *delta);
//funcion que hace que la nave dispare
void NaveDispara(Uint32 *time, SDL_Rect *ship, SDL_Rect p_bullet[], int balas[10][3],SDL_Renderer *renderer, SDL_Texture *texture);
//Spawnea aliens
void AlienSpawn(SDL_Rect *ship, SDL_Rect *aliens, double alieninfo[100][6], Uint32 *spawntime);

void extraerPuntajes(FILE *archivo , char *puntajes){
	archivo = fopen("score.txt" , "r");
	int i = 0;
	while(feof(archivo) == 0){
		puntajes[i] = fgetc(archivo);
		i++;
	}
}

int mainMenu(int menu){

	char fraseMainMenu[] = "Selecciona una opción \n";
	char Instrucciones[] = "Mueveté con las flechas izquierda y derecha, silencia la música con la flecha hacia arriba y dispara con la barra espaciadora\n\n";
	char Records[10000]; // Records[5]; pos1: puntaje1 , pos2: \n , pos3: puntaje2 , pos4: \n , pos5: puntaje 3
	FILE *score;
	extraerPuntajes(score , Records);
		while(menu == INT_MIN){
	    	const SDL_MessageBoxButtonData buttons[] = {
           	 	{        0, 0, "Jugar" },
            	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Instrucciones" },
            	{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2,"Récords"  },
            	{3,3,"Salir"}
        	};
        	const SDL_MessageBoxColorScheme colorScheme = {
            	{

                	{ 0,   0,   0 },

                	{   255, 255,   255 },

                	{ 255, 255,   255 },

                	{   0,   0, 0 },

                	{ 255,   0, 255 }
            	}
        	};
        	const SDL_MessageBoxData messageboxdata = {
            	SDL_MESSAGEBOX_INFORMATION,
            	NULL,
            	"Space Invaders",
            	fraseMainMenu,
            	SDL_arraysize(buttons),
            	buttons,
           	 	&colorScheme
        	};
        	int buttonid;
        	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
            	SDL_Log("error displaying message box");
            	return 1;
        	}
        	if (buttonid == -1) {
            	SDL_Log("no selection");
        	} else if(buttonid == 0){ //pa jugar
            	menu = 0;
            	break;
        	} else if(buttonid == 1){ //Instrucciones
    			menu = 2;
    			while(menu == 2){
    				const SDL_MessageBoxButtonData botones[] = {
        	    		//{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,0,"Instrucciones"},
        	    		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Volver al menú"},

               		};
               		const SDL_MessageBoxColorScheme colorBotones = {
               			{
               				{0,0,0}, //Fondo de MesaggeBox
               		    	{255,255,255}, //Color del texto del mensaje
               		    	{255,255,0},
               		    	{0,0,0},
               		    	{255,0,255}
               			}

               		};
               		const SDL_MessageBoxData datosBotones = {
               			SDL_MESSAGEBOX_INFORMATION,
               			NULL,
               			"Instrucciones",
               			Instrucciones,
               			SDL_arraysize(botones),
               			botones,
               			&colorBotones
              		};
               		int idBoton;
               		if(SDL_ShowMessageBox(&datosBotones , &idBoton) < 0){
               			printf("error\n");
               			return 1;
               		}
               		if(idBoton == -1) SDL_Log("No seleccion");

               		else if(idBoton == 1){
           			menu = INT_MIN;
              		}
        		}
        	} else if(buttonid == 2){ //Récords
    			menu = 10;
    			while(menu == 10){
    				const SDL_MessageBoxButtonData botones1[] = {
    			       	//{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,0,"Hola"},
    			       	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Volver al menú"},

    			    };
    			    const SDL_MessageBoxColorScheme colorBotones1 = {
    			    	{
    			    				{0,0,0}, //Fondo de MesaggeBox
    			    	  		{255,255,255}, //Color del texto del mensaje
    			            {255,255,0},
    			            {0,0,0},
    			            {255,0,255}
    			        }
    			    };
    			    const SDL_MessageBoxData datosBotones1 = {
    			        SDL_MESSAGEBOX_INFORMATION,
    			        NULL,
    			        "Instrucciones",
    			        Records,
    			        SDL_arraysize(botones1),
    			        botones1,
    			        &colorBotones1
    			    };
    			    int idBoton1;
    			    if(SDL_ShowMessageBox(&datosBotones1 , &idBoton1) < 0){
    			    	printf("error\n");
    			        return 1;
    			    }
    			    if(idBoton1 == -1) SDL_Log("No seleccion");

    			    else if(idBoton1 == 1){

    			    menu = INT_MIN;
    			    }
    			}
        	}
        	else if(buttonid == 3){ //Salir
        		menu = -1;
        		SDL_Quit;
        	}
        	else {
            	SDL_Log("selection was %s", buttons[buttonid].text);
        	}
    	}
    	return menu;
}

int reset(int vidas, int puntaje,int nivel){
	if(nivel >= 0 && vidas == 0 && puntaje >= 0){
		nivel = 0;
		vidas = 3;
		puntaje = 0;
		return nivel,vidas,puntaje;
	}
}

int menuMuerte(int gameOver){
	while(gameOver == INT_MAX){
		const SDL_MessageBoxButtonData botones[] = {
        	{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,0,"Volver a Jugar"},
        	{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,1,"Volver al menú principal"},
        	{2,2,"Salir"},
        };
        const SDL_MessageBoxColorScheme colorBotones = {
        	{
        		{0,0,0}, //Fondo de MesaggeBox
        		{255,255,255}, //Color del texto del mensaje
        		{255,255,0},
        		{0,0,0},
        		{255,0,255}
        	}
        };
        const SDL_MessageBoxData datosBotones = {
        	SDL_MESSAGEBOX_INFORMATION,
        	NULL,
        	"Has Perdido",
        	"Has perdido, selecciona un botón",
        	SDL_arraysize(botones),
        	botones,
        	&colorBotones
        };
        int idBoton;
        if(SDL_ShowMessageBox(&datosBotones , &idBoton) < 0){
        	printf("error\n");
        	return 1;
        }
        switch(idBoton){
        	case -1:
        		SDL_Log("No seleccion");
        		break;
        	case 0: //volver a jugar
        		gameOver = CHAR_BIT;
        		break;
        	case 1: //volver al menú principal
        		gameOver = INT_MIN;
        		break;
        	case 2: //salir
						gameOver = CHAR_MIN;
						SDL_Quit();
        		break;
        }
    }
    return gameOver;
}

int main(int argc, char* argv[]){
	//Carga el archivo del score
	FILE *score;
	char scores[10000];
	//VolerMenu:
	int menu = INT_MAX;
	//INICIO;
	int gameOver = INT_MIN;
	gameOver = mainMenu(gameOver);


	if(gameOver == 3){
		Mix_Quit();
		SDL_Quit();
		return 0;
	}

	//if(gameOver == INT_MIN) gameOver = mainMenu(gameOver);


	TTF_Init();
	//si SDL no se inicia lanza una ventana de error
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",SDL_GetError(),NULL);
	}
	//en caso contrario crea una ventana con nombre "Space Invaders"
	//y lanza el mensaje GAME READY
	/*
	else{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Space Invaders","GAME READY",NULL);
	}
	*/
	int typeEvent;//crea una variable entera typeEvent para almacenar la entrada de tecla
	//crea una variable entera para controlar si sigue dentro del ciclo del juego o no

	const unsigned char *keys;//crea una constante char sin signo para identificar la tecla que se pulsa

	SDL_Window *window;//crea un puntero de tipo ventana con nombre window
	SDL_Renderer *renderer;//crea un puntero de tipo Render con nombre renderer
	SDL_Event event;//crea una variable de tipo evento
	TTF_Font *gFont; //La fuente a ser utilizada

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



	keys=SDL_GetKeyboardState(NULL);//se le asigna a keys la funcion GetKeyboardState, que detecta que tecla fue pulsada, la tecla presionada se guarda en key

	//ACA ASIGNAMOS TEXTURAS Y COLISIONES
	SDL_Texture* ship_texture = LoadTexture("Resources/Sprites/Nave_new.png", renderer);
	SDL_Texture* b_Texture = LoadTexture("Resources/Sprites/Bullet_01.png", renderer);
	SDL_Texture* al_Texture = LoadTexture("Resources/Sprites/Alien1_1.png", renderer);

	gFont = TTF_OpenFont("Resources/Fonts/Minecraft.ttf", 28);
	SDL_Rect texto;
	SDL_Rect texto2;
	SDL_Rect GamerOver;
	SDL_Color color = {255, 255, 255};
	SDL_Texture* txt_Texture = LoadFromRenderedText("Hola", gFont, renderer, color);

	SDL_Rect ship;

	//Punteros para música
	Mix_Music *tema = NULL;
	Mix_Music *tema2 = NULL;
	//Para efectos de sonido
	Mix_Chunk *gScratch = NULL;
	Mix_Chunk *gHigh = NULL;
	Mix_Chunk *gMedium = NULL;
	Mix_Chunk *gLow = NULL;
	Mix_Chunk *sonido;
	Mix_Chunk *explosion;

	tema2 = Mix_LoadMUS("Resources/Audio/Music/space.mp3");
	sonido = Mix_LoadWAV("Resources/Audio/disparoNave.wav");
	tema = Mix_LoadMUS("Resources/Audio/Music/megalovania.mp3");

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
	//[x][1] indica el tipo de movimiento (1 = recto, 2 = espiral)
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
	int vida = 3;
	int puntaje = 0;
	char puntos[10000];
	char manzana[50];
	int initSpawner = 0;
	//Mix_PlayMusic(tema,-1);
	Inicio:
	do{//se niega gameOver para que se considere verdadera, mientras sea "verdadera" tb se puede usar la condicion gameOver==0

		//LOOP PA DISEÑO BONITO
		for(int i=0; i<20; i++){
			if(alieninfo[i][0] != -1 && initSpawner < 20){
				alieninfo[i][0] = 1;
				alieninfo[i][1] = 2;
				alieninfo[i][2] = i*M_PI/5;
				if(i < 10){alieninfo[i][3] = 25;}
				else{alieninfo[i][3] = 50;}

				initSpawner += 1;
			}
		}

		//Texturas a actualizar
		SDL_Texture* txt_Texture = LoadFromRenderedText(my_itoa(puntaje, puntos), gFont, renderer, color);
		SDL_Texture* vidas = LoadFromRenderedText(my_itoa(vida,manzana),gFont,renderer,color);
		SDL_Texture* go = LoadFromRenderedText(NULL, gFont,renderer,color);
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
				}else if(keys[SDL_SCANCODE_SPACE]){ //si se presiona el espacio
					//Mix_PlayChannel(-1,sonido,0);
					NaveDispara(&bulletTime, &ship, p_bullet, balas, renderer, b_Texture); //la nave dispara

				}

				//COMANDOS DE DESARROLLO
				else if(keys[SDL_SCANCODE_UP]){ //Empieza musica
					//Play the music
        			//Mix_PlayMusic(tema, -1 );
        			Mix_PlayMusic(tema2,-1);
				}
			}
		}
		//Aparece un alien cada x milisegundos
		AlienSpawn(&ship, aliens, alieninfo, &spawnTime);

		//Posiciona el sprite de la nave
		texto.x = 0;
		texto.y = 0;
		texto2.x = 100;
		texto2.y = 0;
		GamerOver.x = XSIZE/2;
		GamerOver.y = YSIZE/2;

		//Pide info de texturas
		SDL_QueryTexture(ship_texture, NULL, NULL, &ship.w, &ship.h);
		SDL_QueryTexture(txt_Texture, NULL, NULL, &texto.w, &texto.h);
		SDL_QueryTexture(go, NULL, NULL, &GamerOver.w, &GamerOver.h);
		SDL_QueryTexture(vidas,NULL,NULL,&texto2.w, &texto2.h );
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
				aliens[i].x = XSIZE;
				aliens[i].y = YSIZE;
			}
			if (alieninfo[i][0] == 1)
			{
				for(int u=0; u<10; u++){
					if(SDL_HasIntersection(&p_bullet[u], &aliens[i]) == SDL_TRUE
				     && aliens[i].x > 1 && aliens[i].y > 1){
						alieninfo[i][0] = -1;
						balas[u][0] = 0;
						puntaje += 100;	//Colision alien-bala
					}
				}
				if(SDL_HasIntersection(&ship, &aliens[i]) == SDL_TRUE
			     && aliens[i].x > 1 && aliens[i].y > 1){
					alieninfo[i][0] = -1;
					vida --;	//Colision nave-alien
				}
				if(vida == 0){

					Mix_PlayChannel(-1,explosion,0);
					gameOver = INT_MAX;
					gameOver = menuMuerte(gameOver);

					if(gameOver == CHAR_BIT){ //volver a jugar
						//Reinicia los aliens
						for(int i=0; i<100; i++){
							alieninfo[i][0] = 0;
						}
						initSpawner = 0;
						vida = 3;
						puntaje = 0;
						gameOver = 0;
					}else if(gameOver == INT_MIN){ //volver al menu principal
						vida = 3;
						puntaje = 0;
						gameOver = mainMenu(gameOver);
					}else{ //salir
						break;
					}

				}

				if(alieninfo[i][1] == 1){
					aliens[i].x += alieninfo[i][4];
					aliens[i].y += alieninfo[i][5];
				}
				if(alieninfo[i][1] == 2){
					alieninfo[i][3] += (0.05);
					alieninfo[i][2] += 1*M_PI/180;
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

		int cuentaAliens = 0; //aliens muertos
		for(int i=0; i<100; i++){
			if (alieninfo[i][0] == -1) {
				cuentaAliens += 1;
			}
			if (cuentaAliens == 100) {
				//INICIAR NIVEL NUEVO
				initSpawner = 0;
				for(int u=0; u<100; u++){
					alieninfo[u][0] = 0;
				}
			}
		}

		SDL_RenderCopy(renderer, ship_texture, NULL, &ship);
		SDL_RenderCopy(renderer, txt_Texture, NULL, &texto);
		SDL_RenderCopy(renderer, vidas, NULL,&texto2);
		//SDL_RenderCopy(renderer,go,NULL,&GamerOver);   //no se imprime ;-;
		//presenta la imagen, creo
		SDL_RenderPresent(renderer);
		SDL_Delay(MS);


	}while(!gameOver);

	printf("\ngameOver %d\n, vida %d \n, puntaje %d\n ", gameOver,vida,puntaje);
	//finaliza todo

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	SDL_Quit();
	Top(score,puntaje,puntos);
	fclose(score);

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

	if((keys[SDL_SCANCODE_SPACE]) && (tiempo_actual - *dash) > 500){ //si se presiona el espacio y han pasado x segundos
		*delta += *vx*M_PI/5;
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
		if(balas[i][0] == 0 && (tiempo_actual - *time) > 250){
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

void AlienSpawn(SDL_Rect *ship, SDL_Rect *aliens, double alieninfo[100][6], Uint32 *spawntime){

	Uint32 tiempo_actual = SDL_GetTicks();
	int tipo = rand() % 101;

	for (int i = 0; i < 100; i++)
	{
		//1 significa inicializado. 0 es una bala disponible
		if(alieninfo[i][0] == 0 && tiempo_actual - *spawntime > 150){
			aliens[i].x = XSIZE/2;
			aliens[i].y = YSIZE/2 - 1;
			alieninfo[i][0] = 1;
			if(tipo <= 10){ //Probabilidad de [i][1] = 1
				alieninfo[i][1] = 1;
				alieninfo[i][4] = (ship->x - (XSIZE + ship->w)/2)/20;
				alieninfo[i][5] = (ship->y - (YSIZE + ship->h)/2)/20;
			}
			if(tipo > 45){ //Probabilidad de [i][1] = 2
				alieninfo[i][1] = 2;
				alieninfo[i][2] = 0;
				alieninfo[i][3] = 1;
			}

			*spawntime = tiempo_actual;
			break;
		}
	}
}
void Top(FILE *archivo, int puntaje,char *puntos){
	char guardado1[10000];
	char guardado2[10000];
	char guardado3[10000];
	archivo = fopen("score.txt","r");
	fgets(guardado1,10000,archivo);
	fgets(guardado2,10000,archivo);
	fgets(guardado3,10000,archivo);
	fclose(archivo);
	archivo = fopen("score.txt","w");
	if(puntaje>atoi(guardado1))
	{
		fputs(my_itoa(puntaje,puntos),archivo);
		fputs("\n",archivo);
		fputs(guardado2,archivo);
		fputs(guardado3,archivo);
		printf("Se remplazo 1\n");

	}else if(puntaje>atoi(guardado2) && puntaje<atoi(guardado1))
	{
		fputs(guardado1,archivo);
		fputs(my_itoa(puntaje,puntos),archivo);
		fputs("\n",archivo);
		fputs(guardado3,archivo);
		printf("Se remplazo 2\n");

	}else if(puntaje>atoi(guardado3) && puntaje<atoi(guardado2))
	{
		fputs(guardado1,archivo);
		fputs(guardado2,archivo);
		fputs(my_itoa(puntaje,puntos),archivo);
		printf("Se remplazo 3\n");

	}
	if(puntaje<atoi(guardado3))
	{
		fputs(guardado1,archivo);
		fputs(guardado2,archivo);
		fputs(guardado3,archivo);
	}
	fclose(archivo);
}
