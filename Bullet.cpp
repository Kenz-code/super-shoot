#pragma 
#include <iostream>
#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Bullet.h"
#include "Render.h"

Bullet::Bullet(int width, int height, int x, int y, SDL_Texture* texture, int angle) {
	Width = width;
	Height = height;
	xp = x;
	yp = y;
	Texture = texture;
	Angle = angle;

}

Bullet::~Bullet() {
	Width = 0;
	Height = 0;
	xp = 0;
	yp = 0;
	Texture = NULL;
	Angle = 0;
}

void Bullet::move() {
	if (Angle == 0) {
		xp += 8;
	}
	if (Angle == 180) {
		xp -= 8;
	}
	if (Angle == 90) {
		yp += 8;
	}
	if (Angle == 270) {
		yp -= 8;
	}

}
