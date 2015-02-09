#include "theBullet.h"
#include "theEnemy.h"
#include "thePlayer.h"

#include <stdlib.h>
#include <cstdio>

using namespace std;

//screen and sprtie dimensions
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 1000;
const int SPRITE_WIDTH = 90;
const int SPRITE_HEIGHT = 90;
const int WIDTH = 900;
int NUM_OF_BULLETS=0;
const int MAX_BULLETS = 5;
int CURR_NUM_BULLETS = 1;
const int MAX_ENEMIES = 24;
int NUM_OF_ENEMIES=MAX_ENEMIES;
const int ENEMIES_ROW = 3;
const int ENEMIES_COL = 8;
const int BULLET_OFFSET = 50;	//  50 px before second bullet
const int ENEMIES_OFFSET_X = 75;	// 100 px distance between next enemies
const int ENEMIES_OFFSET_Y = 75;	// 100 px distance between next enemies
short int LAST_LEFT=7;
short int LAST_RIGHT=0;
int GAME_OVER=550;					// if greater than 550px END
short int enemySpeed=1;


SDL_Window* Window = NULL;			//the window rendering to
SDL_Surface* ScreenSurface = NULL;	//surface contained by window
SDL_Surface* Background = NULL;
SDL_Surface* Player = NULL;
SDL_Surface* Enemy = NULL;
SDL_Surface* Bullet = NULL;
SDL_Surface* newBullet = NULL;
SDL_Surface* Cmap = NULL;

SDL_Rect posPlayer, posEnemy[MAX_ENEMIES], posBullet, posnewBullet;

const Uint8* keystate = SDL_GetKeyboardState(NULL);
SDL_Event event;

theBullet* listOfBullets[MAX_BULLETS]={0};
bool listOfActiveBullets[MAX_BULLETS]={0};
theEnemy*  listOfEnemies[ENEMIES_ROW][ENEMIES_COL]={0};
bool listOfActiveEnemies[ENEMIES_ROW][ENEMIES_COL]={0};

int score=0;
bool reset=true;

bool initialise()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_EVERYTHING |  SDL_INIT_AUDIO) !=0)
	{
		cout<<"SDL_Init Error."<<SDL_GetError()<<endl;
		success = false;
	}
	else
	{
		//create the window for game
		Window = SDL_CreateWindow("Space invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (Window == NULL)
		{
			cout<<"Window Error"<<SDL_GetError()<<endl;
			success = false;
		}
		else
		{
			//get window surface
			ScreenSurface = SDL_GetWindowSurface(Window);
		}
	}
	
	return success;
}

bool LoadingMedia()
{
	bool success = true;

	Player = SDL_LoadBMP("spaceship.bmp");
	if (Player == NULL)
	{
		cout<<"Error in loading player."<<SDL_GetError()<<endl;
		success = false;
	}
	
	Enemy = SDL_LoadBMP("enemy.bmp");
	if (Enemy == NULL)
	{
		cout<<"Error in loading enemy."<<SDL_GetError()<<endl;
		success = false;
	}

	Bullet = SDL_LoadBMP("bullet.bmp");
	if (Bullet == NULL)
	{
		cout<<"Error in loading bullet."<<SDL_GetError()<<endl;
		success = false;
	}

	Cmap = SDL_LoadBMP("cmap.bmp");
	if (Cmap == NULL)
	{
		cout<<"Error in loading Cmap."<<SDL_GetError()<<endl;
		success = false;
	}
	
	return success;
}
	
void closedown()
{
	SDL_FreeSurface(Background);
	Background = NULL;
	SDL_FreeSurface(Player);
	Player = NULL;
	SDL_FreeSurface(Enemy);
	Enemy = NULL;

	SDL_DestroyWindow(Window);
	Window = NULL;

	SDL_Quit();
}

int draw_char(char c, int x, int y) {

	SDL_Rect src;
	SDL_Rect dest;
	int i,j;
	char *map[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			"abcdefghijklmnopqrstuvwxyz",
			"!@#$%^&*()_+{}|:\"<>?,.;'-=",
			"0123456789"};

	src.x = 0;
	src.y = 0;
	src.w = 20;
	src.h = 20;
	
	dest.x = x;
	dest.y = y;
	dest.w = 20;
	dest.h = 20;

	for (i = 0; i < 4; i++) {
	
		for(j = 0; j < strlen(map[i]); j++) {
			
			if (c == map[i][j]) {
			
				SDL_BlitSurface(Cmap, &src, ScreenSurface, &dest);
				SDL_SetColorKey(Cmap, SDL_TRUE, SDL_MapRGB(Cmap->format, 255, 0, 255));
				return 0;
			}

			src.x += 20;
		}
	
		src.y += 20;//move down one line on the image file
		src.x = 0; //reset to start of line
	}

	return 0;
}

void draw_string(char s[], int x, int y) {

	int i;

	for (i = 0; i < strlen(s); i++) {
	
		draw_char(s[i],x,y);
		x += 20;
	}
}

void draw_hud() {
	
	SDL_Rect r;
	
	r.x = 0;
	r.y = 0;
	r.w = SCREEN_WIDTH - 800;
	r.h = 100;

	char score_label[] = "Score";
	draw_string(score_label, WIDTH, 0);
	
	char score_num[10];
	itoa(score,score_num,10);
	draw_string(score_num, WIDTH, 20);
	
	char bull_label[] = "Bullets:";
	draw_string(bull_label, 0, 0);

	char bullet_num[10];
	itoa(CURR_NUM_BULLETS,bullet_num,10);
	draw_string(bullet_num, 0, 20);

}

void resetGame(){
	int num=0;	// position of all enemies
	for(int i=0;i<ENEMIES_ROW;i++){
		for(int j=0;j<ENEMIES_COL;j++){

			listOfEnemies[i][j] = new theEnemy(SCREEN_WIDTH-(j+1)*ENEMIES_OFFSET_X,(ENEMIES_OFFSET_Y)*i+45,num);
			listOfEnemies[i][j]->move = enemySpeed;
			listOfActiveEnemies[i][j] = true; 
			num++;
		}
	}
	enemySpeed++;
}


int main(int argc, char** argv)
{
	bool quit = false;
	bool hit = false;
	thePlayer myPlayer;
		
	theBullet* lastBullet = new theBullet(0,SCREEN_HEIGHT-150);	// for more bullets not to colide
	
	if (!initialise())
	{
		cout<<"Failed to initialise"<<SDL_GetError()<<endl;
	}
	else
	{
		if (!LoadingMedia())
		{
			cout<<"Error loading media"<<SDL_GetError()<<endl;
		}
	}
	
	
	draw_hud();	
	SDL_FillRect(ScreenSurface, NULL, 0x000000);
	draw_hud();

	//GAME LOOP
	while (quit == false)
	{
		if(reset){ resetGame(); reset=false;}

		SDL_BlitSurface(Background, NULL, ScreenSurface, NULL);
		myPlayer.show_player();
		myPlayer.player_movement();

		

		while (SDL_PollEvent(&event))
		{
			if( event.type == SDL_QUIT )
            {
                quit = true;
				break;
            }
		
			if((keystate[SDL_SCANCODE_SPACE] || event.type == SDL_MOUSEBUTTONDOWN ) && NUM_OF_BULLETS < MAX_BULLETS)
			{
			
				for (int i=0; i<CURR_NUM_BULLETS; i++)
				{
					//cout<<"listOf..["<<i<<"]="<<listOfActiveBullets[i]<<endl;
					if ( listOfActiveBullets[i] == false 
						&& posPlayer.y - lastBullet->y_position > BULLET_OFFSET
 						)
					{
						listOfBullets[i] = new theBullet(posPlayer.x,posPlayer.y);
						
						listOfBullets[i]->isActive = true;
						listOfActiveBullets[i] = true;
						
						lastBullet = listOfBullets[i];

						NUM_OF_BULLETS++;
						break;
					}
				}

			
			}
		
		
		}// end while (SDL_PollEvent(&event))

		//update game objects
		for (int i=0; i<CURR_NUM_BULLETS; i++){

			if( listOfActiveBullets[i] == true){
				if (listOfBullets[i]->isActive == true){				
					listOfBullets[i]->y_position -= 2;
			
					if (listOfBullets[i]->y_position < -posBullet.h)
					{
						listOfBullets[i]->isActive = false;
						listOfActiveBullets[i] = false;
						//free(listOfBullets[i]);
						score-=5;
						SDL_FillRect(ScreenSurface, NULL, 0x000000); // refresh image of bullet
						draw_hud();	
						NUM_OF_BULLETS--;
					}else if(listOfBullets[i]->y_position <= posEnemy[22].y+posEnemy[16].h){	// not entering untill close to last enemy
					
						for(int j=2;j>=0;j--){
							for(int k=0;k<8;k++){
								if(listOfActiveEnemies[j][k]){
									if(listOfBullets[i]->x_position - posEnemy[j*8+k].x < posEnemy[j*8+k].w && listOfBullets[i]->x_position - posEnemy[j*8+k].x > 0 ){
										if(listOfBullets[i]->y_position - posEnemy[j*8+k].y < posEnemy[j*8+k].h && listOfBullets[i]->y_position - posEnemy[j*8+k].y > 0 ){ 
											//cout<<"POGODAK"<<endl;
											
											listOfBullets[i]->isActive = false;
											listOfActiveBullets[i] = false;
											listOfBullets[i]->y_position -= 2;
											NUM_OF_BULLETS--;
					
											listOfActiveEnemies[j][k]=false;
											listOfEnemies[j][k]->toLeft = true;
								
											
											
											if(j==2) score+=10;
											else if(j==1) score+=20;
											else if(j==0) score+=30;
											if(NUM_OF_ENEMIES>0)
											NUM_OF_ENEMIES--;
											else {
											break;
											}
	  										SDL_FillRect(ScreenSurface, NULL, 0x000000); // refresh image of bullet
											draw_hud();	
											break;
										}
									}
								}
							}
						}

					}
				}
			}
		}
		
		for (int i=0; i<CURR_NUM_BULLETS; i++)
		{
			if( listOfActiveBullets[i] == true){
				if (listOfBullets[i]->isActive == true)
				{
				
					SDL_Rect dstrect;
					dstrect.x = listOfBullets[i]->x_position;
					dstrect.y = listOfBullets[i]->y_position;
					dstrect.w = Bullet->w;
					dstrect.h = Bullet->h;
					SDL_BlitSurface(Bullet, NULL, ScreenSurface, &dstrect);
				
				
				}
			}
		}

		for (int i=0; i<CURR_NUM_BULLETS; i++)
		{
			if( listOfActiveBullets[i] == true){
				if (listOfBullets[i]->isActive == true)
				{
					SDL_Rect dstrect;
					dstrect.x = listOfBullets[i]->x_position;
					dstrect.y = listOfBullets[i]->y_position;
					dstrect.w = Bullet->w;
					dstrect.h = Bullet->h;
					SDL_BlitSurface(Bullet, NULL, ScreenSurface, &dstrect);
			
				}
			}
		}

		myPlayer.show_player();

		for(int i=0;i<ENEMIES_ROW;i++){
			for(int j=0;j<ENEMIES_COL;j++){
				if(listOfActiveEnemies[i][j]){
				listOfEnemies[i][j]->show_enemy();
				}
			}			
		}

		myPlayer.player_movement();
		
		if(listOfActiveEnemies[2][LAST_LEFT]==false) 
			if(listOfActiveEnemies[1][LAST_LEFT]==false) 
				if(listOfActiveEnemies[0][LAST_LEFT]==false){ 
					if(LAST_LEFT>0)
					LAST_LEFT--;
					//cout<<"LAST_LEFT "<<LAST_LEFT<<endl;
				}

		if(listOfActiveEnemies[2][LAST_RIGHT]==false) 
			if(listOfActiveEnemies[1][LAST_RIGHT]==false) 
				if(listOfActiveEnemies[0][LAST_RIGHT]==false) {
					if(LAST_RIGHT<7)
					LAST_RIGHT++;
					//cout<<"LAST_RIGHT "<<LAST_RIGHT<<endl;
				}

		for(int i=0;i<ENEMIES_ROW;i++){
			for(int j=0;j<ENEMIES_COL;j++){
			
				if(listOfEnemies[i][j]->toLeft){
					if( posEnemy[LAST_LEFT].x >= 1){
						posEnemy[i*8+j].x-= listOfEnemies[i][j]->move;
					}else if (posEnemy[i*8+j].y%15 || posEnemy[i*8+j].y <= listOfEnemies[i][j]->offset) {
						posEnemy[i*8+j].y+=1;
					}else if(posEnemy[LAST_LEFT].x <= 1){	
						listOfEnemies[i][j]->toLeft=false;
						listOfEnemies[i][j]->offset+=15;
					}
				}else if(!listOfEnemies[i][j]->toLeft){
					if( posEnemy[LAST_RIGHT].x + listOfEnemies[i][j]->move <= SCREEN_WIDTH-50){
						posEnemy[i*8+j].x+=listOfEnemies[i][j]->move;
					}else if (posEnemy[i*8+j].y%15 || posEnemy[i*8+j].y <= listOfEnemies[i][j]->offset) {
						posEnemy[i*8+j].y+=1;
					}else if(posEnemy[LAST_RIGHT].x <= SCREEN_WIDTH-49){ 
						listOfEnemies[i][j]->toLeft=true;
						listOfEnemies[i][j]->offset+=15;
					}
				}

			}
		}
	
		//delay--;
		//if(delay==0){
		SDL_Delay(CURR_NUM_BULLETS);
		//delay=20;
		//}
		
		
		if(NUM_OF_ENEMIES==0) { 
				reset=true;
				for(int i=0;i<ENEMIES_ROW;i++){
					for(int j=0;j<ENEMIES_COL;j++){

						free(listOfEnemies[i][j]);
					}
				}
				NUM_OF_ENEMIES=24;
				if(CURR_NUM_BULLETS<MAX_BULLETS)
				CURR_NUM_BULLETS++;
				LAST_LEFT=7;
				LAST_RIGHT=0;
		}
		
		
		SDL_UpdateWindowSurface(Window); //updates screen
	}

	closedown();

	return 0;

}