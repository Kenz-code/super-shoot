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

#include "Render.h"
#include "Player.h"
#include "Enemy.h"


Enemy::Enemy(EnemyType Type, double Y, double Speed)
	:x(1280), y(Y), speed(Speed), type(Type)
{
	if (type == REDSIREN or type == GREENSIREN) {
		CollisionBox = { x+ 20,y+ 20,45 };
	}
}

void Enemy::move() {
	x -= speed * deltaTime;
	if (type == REDSIREN or type == GREENSIREN) {
		CollisionBox = { x + 20,y + 20,45 };
	}
}
