// Project:	Space Invaders
// File: 	theBullet.h
// Date: 	February 2015
// Author: 	Petar Parabucki
// Contents: 
//			Class: theBullet 

#pragma once

#include "externs.h"

class theBullet
{
	public:
	theBullet(int,int);

	bool isActive;
	int x_position;
	int y_position;

	void bullet_movement();
	void add_new_bullet();

};