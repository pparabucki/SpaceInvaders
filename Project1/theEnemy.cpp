// Project:	Space Invaders
// File: 	theEnemy.cpp
// Date: 	February 2015
// Author: 	Petar Parabucki
// Contents: 
//			Class: theEnemy  

#include "theEnemy.h"


theEnemy::theEnemy(int x,int y, int i)
{
	srand (time(NULL));
	id=i;
	posEnemy[id].x = x;
	posEnemy[id].y = y;
	posEnemy[id].w = 45;
	posEnemy[id].h = 80;
	toLeft=true;
	offset=y;
	move=1;
	speedup=false;
	//std::cout<<"posEnemy["<<id<<"].x = "<<posEnemy[id].x<<std::endl;
	//std::cout<<"posEnemy["<<id<<"].y = "<<posEnemy[id].y<<std::endl;
	//std::cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;

}

void theEnemy::enemy_movement()
{
	
	if(toLeft){
		if( posEnemy[id].x >= 1){
			posEnemy[id].x-=move;
		}else if (posEnemy[id].y%25 || posEnemy[id].y <= offset) {
			posEnemy[id].y+=1;
		}else if(posEnemy[id].x <= 1){	
			toLeft=false;
			offset+=25;
		}
	}else if(!toLeft){
		if( posEnemy[id].x <= SCREEN_WIDTH-50){
			posEnemy[id].x+=move;
		}else if (posEnemy[id].y%25 || posEnemy[id].y <= offset) {
			posEnemy[id].y+=1;
		}else if(posEnemy[id].x <= SCREEN_WIDTH-49){ 
			toLeft=true;
			offset+=25;
		}
	}
	
	// Error here 
	//if(offset%250==0 && offset!=0 &&  posEnemy.x == SCREEN_WIDTH-49 ) 
		//move+=1;
}

void theEnemy::show_enemy()
{
	SDL_BlitSurface(Enemy, NULL, ScreenSurface, &posEnemy[id]);
	SDL_SetColorKey(Enemy, SDL_TRUE, SDL_MapRGB(Player->format, 255, 255, 255));
}