#pragma(once)

#include <SDL.h>
#include <SDL_audio.h>
#include <stdio.h> //use for things like printf, same as cout
#include <iostream>
#include <string>
#include <time.h>
#include<windows.h>
#include <stdlib.h>
#include <cstdio>


extern const int SCREEN_HEIGHT;
extern const int SCREEN_WIDTH;
extern const int SPRITE_WIDTH;
extern const int SPRITE_HEIGHT;

extern int NUM_OF_BULLETS;
extern const int MAX_BULLETS;
extern const int MAX_ENEMIES;
extern const int BULLET_OFFSET; // 50 px before second bullet
extern const int ENEMIES_OFFSET;

extern SDL_Window* Window;//the window rendering to
extern SDL_Surface* ScreenSurface;//surface contained by window
extern SDL_Surface* Background;
extern SDL_Surface* Player;
extern SDL_Surface* Enemy;
extern SDL_Surface* Bullet;
extern SDL_Surface* newBullet;

extern SDL_Rect posPlayer, posEnemy[], posBullet, posnewBullet;

extern const Uint8* keystate;
extern SDL_Event event;