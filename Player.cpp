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

#include "Render.h"
#include "Player.h"

Player::Player(int width, int height) {
	x = 0.0, y = 0.0;
	goleft = false, goright = false, goup= false, godown=false;
	Width = width, Height = height;
	CollisionBox = { x,y,13};
	player_up = { 0,0,40,40 };
	player_right = { 40,0,40,40 };
	player_down = { 80,0,40,40 };
	player_left = { 120,0,40,40 };
	current_clip = player_up;
}
Player::~Player() {
	float x = NULL, y = NULL;
	int Width = NULL, Height = NULL;
	SDL_Rect CollisionBox = { NULL,NULL,NULL,NULL };
}
void Player::move(SDL_Event e) {
	if (goup) {
		y -= 0.35 * deltaTime;
		CollisionBox = { x + 20,y + Width - 13,12 };
	}
	if (godown) {
		y += 0.35 * deltaTime;
		CollisionBox = { x + 20,y + 13,12 };
	}
	if (goright) {
		x += 0.35 * deltaTime;
		CollisionBox = { x + 13,y + 20,12 };
	}
	if (goleft) {
		x -= 0.35 * deltaTime;
		CollisionBox = { x + Width - 13,y + 20,12 };
	}
	if (x-CollisionBox.r < 0) {
		x = 0+ CollisionBox.r;
	}
	if (y -CollisionBox.r < 0) {
		y = 0+ CollisionBox.r;
	}
	if (x + CollisionBox.r > 1280) {
		x = 1280 - CollisionBox.r;
	}
	if (y+- CollisionBox.r > 720) {
		y = 720 - CollisionBox.r;
	}
}
void Player::keydown(SDL_Event e, SDL_Texture* bullet) {
	switch (e.key.keysym.sym) {
	case SDLK_d:
		goright = true;
		current_clip = player_right;
		break;
	case SDLK_a:
		goleft = true;
		current_clip = player_left;
		break;
	case SDLK_w:
		goup = true;
		current_clip = player_up;
		break;
	case SDLK_s:
		godown = true;
		current_clip = player_down;
		break;
	}
}

void Player::keyup(SDL_Event e) {
	switch (e.key.keysym.sym) {
	case SDLK_d:
		goright = false;
		break;
	case SDLK_a:
		goleft = false;
		break;
	case SDLK_w:
		goup = false;
		break;
	case SDLK_s:
		godown = false;
		break;
	}
}
float Player::getxpos() {
	return x;
}
float Player::getypos() {
	return y;
}
int Player::getwidth() {
	return Width;
}
int Player::getheight() {
	return Width;
}