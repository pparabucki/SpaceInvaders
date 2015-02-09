// Project:	Space Invaders
// File: 	theBullet.cpp
// Date: 	February 2015
// Author: 	Petar Parabucki
// Contents: 
//			Class: theBullet  

#include "theBullet.h"

theBullet::theBullet(int x,int y)
{
	x_position = x + SPRITE_WIDTH/2-5;
	y_position = y ;
	posBullet.x= x + SPRITE_WIDTH/2;
	posBullet.y= y ;
	posBullet.w = 11;
	posBullet.h = 22;
	this->isActive=true;
}

void theBullet::bullet_movement()
{
	posBullet.y -= 2;

	if(posBullet.y < 0)
	{
		posBullet.y = -50;

	}
}
