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
	CollisionBox = { x + 10,y + 16,24,10 };
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
		xp += 0.8 * deltaTime;
	}
	if (Angle == 180) {
		xp -= 0.8 * deltaTime;
	}
	if (Angle == 90) {
		yp += 0.8 * deltaTime;
	}
	if (Angle == 270) {
		yp -= 0.8 * deltaTime;
	}
	CollisionBox = { xp + 10,yp + 16,24,10 };

}
