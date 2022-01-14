#pragma once

#include <iostream>
#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include <Windows.h>
#include <fstream>
#include <Commdlg.h>
#include <thread>
#include <stdlib.h>
#include <time.h>

#include "Particle.h"

Particle::Particle(int x, int y) :friction(0.18), X(x), Y(y) {
	srand(time(NULL));
	velX = rand() % 15 + 1;
	srand(time(NULL));
	velX = rand() % 15 + 1;

	srand(time(NULL));
	negitive = rand() % 2 + 1;
	if (negitive == 1) {
		int Xminus = velX * 2;
		velX -= Xminus;
	}
	srand(time(NULL));
	negitive = rand() % 2 + 1;
	if (negitive == 2) {
		int Xminus = velX * 2;
		velY -= Xminus;
	}
}

Particle::~Particle() {
	friction = NULL;
	velX = NULL;
	velY = NULL;
}
bool Particle::move() {
	X += velX;
	velX* friction;
	Y += velY;
	velY* friction;
	if (velX < 1 and velY < 1) {
		return false;
	}
	return true;
}