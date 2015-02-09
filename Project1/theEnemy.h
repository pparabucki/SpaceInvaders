// Project:	Space Invaders
// File: 	theEnemy.h
// Date: 	February 2015
// Author: 	Petar Parabucki
// Contents: 
//			Class: theEnemy 

#pragma once

#include "externs.h"

class theEnemy
{
	
public:
	friend int main(int, char**);
	friend void resetGame();
	
	theEnemy(int x,int y,int i);

	void enemy_movement();
	void show_enemy();

private:
	short int id;
	short int move;
	
	int offset;

	bool toLeft;	
	bool speedup;
};