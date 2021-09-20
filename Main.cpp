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
#include "Bullet.h"
#include "Enemy.h"

using namespace std;
Window window;
Player player1(80,80);

bool thatButton = false;

SDL_Texture* player1Tre = window.loadFromFilee("res/Player_spritesheet.png");
SDL_Texture* EnemyTre = window.loadFromFilee("res/Enemy.png");
SDL_Texture* bulletTre = window.loadFromFilee("res/Bullet.png");
SDL_Texture* blankTre = window.loadFromFilee("res/blank.png");
SDL_Texture* titleTre = window.loadFromFilee("res/title.png");
SDL_Texture* bgTre = window.loadFromFilee("res/bg.png");

Mix_Chunk* mainbgSfx = Mix_LoadWAV("res/1.wav");
Mix_Chunk* titleSfx = Mix_LoadWAV("res/title.wav");
Mix_Chunk* titleclickSfx = Mix_LoadWAV("res/titlee.wav");

SDL_Color BLACK = { 0,0,0 };
SDL_Color WHITE = { 255,255,255 };

SDL_Rect RedSirenTre = { 0,0,40,40 };

std::vector<Enemy> enemies;
std::vector<bool> enemyCS(100);

double distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}

bool checkCollisionCC(Circle a, Circle b)
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	//If the distance between the centers of the circles is less than the sum of their radii
	if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
	{
		//The circles have collided
		return true;
	}

	//If not
	return false;
}

bool checkCollisionCR(Circle a, SDL_Rect b)
{
	//Closest point on collision box
	int cX, cY;

	//Find closest x offset
	if (a.x < b.x)
	{
		cX = b.x;
	}
	else if (a.x > b.x + b.w)
	{
		cX = b.x + b.w;
	}
	else
	{
		cX = a.x;
	}
	//Find closest y offset
	if (a.y < b.y)
	{
		cY = b.y;
	}
	else if (a.y > b.y + b.h)
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = a.y;
	}

	//If the closest point is inside the circle
	if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r)
	{
		//This box and the circle have collided
		return true;
	}

	//If the shapes have not collided
	return false;
}

void close() {
	SDL_DestroyTexture(player1Tre);
	SDL_DestroyTexture(bulletTre);
	SDL_DestroyTexture(blankTre);
	SDL_DestroyTexture(titleTre);
	SDL_DestroyTexture(bgTre);
	SDL_DestroyTexture(EnemyTre);
	Mix_FreeChunk(mainbgSfx);
	Mix_FreeChunk(titleSfx);
	Mix_FreeChunk(titleclickSfx);
}

void spawnEnemies(Uint32 time) {
	int yo = time / 1000;
	if (yo == 1 and enemyCS.at(0) != true) {
		Enemy e(REDSIREN, 340, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(),true);
	}
	if (yo == 2 and enemyCS.at(1) != true) {
		Enemy e(REDSIREN, 500, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 3 and enemyCS.at(2) != true) {
		Enemy e(REDSIREN, 220, 1);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
}

Uint32 title() {
	bool running = true;
	SDL_Event e;
	Mix_Volume(0, 95);
	Mix_PlayChannel(0, titleSfx, -1);
	while (running) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
				Mix_HaltChannel(0);
				thatButton = true;
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						running = false;
						Mix_HaltChannel(0);
						Mix_PlayChannel(0, titleclickSfx, 0);
						SDL_Delay(500);
						Mix_HaltChannel(0);
				}
			}
		}
		window.clear(&WHITE);
		window.render(titleTre, 0, 0, 1280, 720, NULL);
		window.update();

	}
	return SDL_GetTicks();
}

void gameOver() {
	//game over stuff
}



int main(int argc, char* argv[]) {
	Uint32 titleTime = title();
	bool running = true;
	if (thatButton == true) {
		running = false;
	}
	SDL_Event e;
	SDL_Rect current_clip1;
	std::vector<Bullet> bullets1;
	Bullet b(1, 1, 1275, 715, bulletTre, 20);
	bullets1.push_back(b);
	Uint32 startTime = SDL_GetTicks();
	Mix_PlayChannel(0, mainbgSfx, -1);
	while (running) {
		//delta time
		window.updateDelta();
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				player1.keydown(e, bulletTre);
				switch (e.key.keysym.sym) { 
				case SDLK_SPACE:
					if (player1.getCurrentClip().x == 0 and player1.getCurrentClip().y == 0) {
						Bullet b(40, 40, player1.getxpos() + 20, player1.getypos(), bulletTre, 270);
						bullets1.push_back(b);
					}
					else if (player1.getCurrentClip().x == 40 and player1.getCurrentClip().y == 0) {
						Bullet b(40, 40, player1.getxpos() + 40, player1.getypos() + 20, bulletTre, 0);
						bullets1.push_back(b);
					}
					else if (player1.getCurrentClip().x == 80 and player1.getCurrentClip().y == 0) {
						Bullet b(40, 40, player1.getxpos() + 20, player1.getypos() + 40, bulletTre, 90);
						bullets1.push_back(b);
					}
					else if (player1.getCurrentClip().x == 120 and player1.getCurrentClip().y == 0) {
						Bullet b(40, 40, player1.getxpos(), player1.getypos() + 20, bulletTre, 180);
						bullets1.push_back(b);
					}
				}
			}
			else if (e.type == SDL_KEYUP) {
				player1.keyup(e);
			}
		}
		
		player1.move(e);
		current_clip1 = player1.getCurrentClip();
		window.clear(&WHITE);
		window.render(bgTre, 0, 0, 1280, 720, NULL);
		for (int i = 0; i < bullets1.size(); i++) {
			if (i != bullets1.size() - 1) {
				bullets1.at(i + 1).move();
			}
			else {
				bullets1.at(i).move();
			}
			if (bullets1.at(i).getxpos() < 0 or bullets1.at(i).getxpos() > 1280) {
				bullets1.erase(bullets1.begin() + i);
			}
			else if (bullets1.at(i).getypos() < 0 or bullets1.at(i).getypos() > 720) {
				bullets1.erase(bullets1.begin() + i);
			}
			else { window.renderEx(bullets1.at(i).gettexture(), bullets1.at(i).getxpos(), bullets1.at(i).getypos(), bullets1.at(i).getwidth(), bullets1.at(i).getheight(), NULL, bullets1.at(i).getangle()); }

		}
		startTime = SDL_GetTicks();
		spawnEnemies(startTime-titleTime);
		for (int i = 0; i != enemies.size(); i++) {
			enemies.at(i).move();
			for (int j = 0; j != bullets1.size(); j++) {
				if (checkCollisionCR(enemies.at(i).getCollisionBox(), bullets1.at(j).getCBox())) {
					if (enemies.size() == 1) {
						Enemy e(REDSIREN, 739, .5);
						enemies.push_back(e);
						enemies.erase(enemies.begin() + i);
					}
					else {
						enemies.erase(enemies.begin() + i);
					}
				}
				if (checkCollisionCC(enemies.at(i).getCollisionBox(), player1.getCBox())) {
					running = false;

				}
			}
			if (enemies.at(i).getxpos() < 0 or enemies.at(i).getxpos() > 1290) {
				if (enemies.size() != 1) {
					enemies.erase(enemies.begin() + i);
				}
			}
			else { 
				if (enemies.at(i).gettype() == REDSIREN) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &RedSirenTre,270);
				} 
			}

		}
		window.render(player1Tre, player1.getxpos(), player1.getypos(), player1.getwidth(), player1.getheight(), &current_clip1);
		window.update();
	}
	close();


	return 0;
}