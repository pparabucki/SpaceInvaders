// Project:	Space Invaders
// File: 	thePlayer.h
// Date: 	February 2015
// Author: 	Petar Parabucki
// Contents: 
//			Class: thePlayer 

#include "thePlayer.h"

thePlayer::thePlayer()
{
	posPlayer.x = SCREEN_WIDTH/2 - SPRITE_WIDTH/2 ;
	posPlayer.y = SCREEN_HEIGHT;

	posPlayer.w = 20;
	posPlayer.h = 30;

}

void thePlayer::player_movement()
{
	if(keystate[SDL_SCANCODE_LEFT])
		{
			posPlayer.x -= 2;
		}
	if(keystate[SDL_SCANCODE_RIGHT])
		{
			posPlayer.x += 2;
		}
	if(keystate[SDL_SCANCODE_UP])
		{
			//do nothing
		}
	if(keystate[SDL_SCANCODE_DOWN])
		{
			//do nothing
		}
	if (event.type == SDL_MOUSEMOTION)
        {
		if (event.motion.xrel < 0)
			posPlayer.x -= 2;
        else if (event.motion.xrel > 0)
            posPlayer.x += 2;
		}
	if ((posPlayer.x + SPRITE_WIDTH) > SCREEN_WIDTH)
		{
			posPlayer.x = (SCREEN_WIDTH - SPRITE_WIDTH);
		}
	if ((posPlayer.y + SPRITE_HEIGHT) > SCREEN_HEIGHT)
		{
			posPlayer.y = (SCREEN_HEIGHT - SPRITE_HEIGHT);
		}

}

void thePlayer::show_player()
{
	SDL_BlitSurface(Player, NULL, ScreenSurface, &posPlayer);
	SDL_SetColorKey(Player, SDL_TRUE, SDL_MapRGB(Player->format, 255, 255, 255));

}