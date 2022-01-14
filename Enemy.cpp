#pragma once

#include "Particle.h"
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


Enemy::Enemy(EnemyType Type, double Y, double Speed, int HEALTH)
	:x(1280), y(Y), speed(Speed), type(Type), health(HEALTH), explosionState(-1)
{
	if (type == REDSIREN or type == GREENSIREN or type == BROWNSIREN) {
		CollisionBox = { x+ 40,y+ 40,40 };
	}
	else if (type == BOSIREN) {
		CollisionBox = { x + 120, y + 120, 120 };
	}
}

void Enemy::move() {
	x -= speed * deltaTime;
	if (type == REDSIREN or type == GREENSIREN or type == BROWNSIREN) {
		CollisionBox = { x + 40,y + 40,40 };
	}
	else if (type == BOSIREN) {
		CollisionBox = { x + 120, y + 120, 120};
	}
}

void Enemy::hit() {
	health -= 1;
}
void Enemy::changeExplosionState(int i) {
	explosionState = i;
}
int Enemy::getExplosionState() {
	return explosionState;
}
void Enemy::add1ExplosionState() {
	explosionState += 1;
}
