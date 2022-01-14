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

#include "Particle.h"
#include "Render.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;
Window window;
Player player1(80,80);

bool thatButton = false;
bool gameOverRunning = false;
int prevPlayer1Score = -1;
int bgTreX = 0;

TTF_Font* font50 = TTF_OpenFont("res/ARCADECLASSIC.TTF", 50);

SDL_Texture* player1Tre = window.loadFromFilee("res/Player_spritesheet.png");
SDL_Texture* EnemyTre = window.loadFromFilee("res/Enemy.png");
SDL_Texture* bulletTre = window.loadFromFilee("res/Bullet.png");
SDL_Texture* blankTre = window.loadFromFilee("res/blank.png");
SDL_Texture* titleTre = window.loadFromFilee("res/title.png");
SDL_Texture* bgTre = window.loadFromFilee("res/bg.png");
SDL_Texture* bossBarInTre = window.loadFromFilee("res/bossBarIn.png");
SDL_Texture* bossBarOutTre = window.loadFromFilee("res/bossBarOut.png");
SDL_Texture* bossDieTre = window.loadFromFilee("res/bossDie.png");

Mix_Chunk* mainbgSfx3 = Mix_LoadWAV("res/lava theme.wav");
Mix_Chunk* mainbgSfx2 = Mix_LoadWAV("res/j.wav");
Mix_Chunk* mainbgSfx1 = Mix_LoadWAV("res/1.wav");
Mix_Chunk* titleSfx = Mix_LoadWAV("res/title.wav");
Mix_Chunk* titleclickSfx = Mix_LoadWAV("res/titlee.wav");
Mix_Chunk* enemyDieSfx = Mix_LoadWAV("res/dieenemy.wav");
Mix_Chunk* playerShootSfx = Mix_LoadWAV("res/shoot.wav");
Mix_Chunk* gameOverSfx = Mix_LoadWAV("res/game over.wav");
Mix_Chunk* winSfx = Mix_LoadWAV("res/win.wav");
Mix_Chunk* bossDieSfx = Mix_LoadWAV("res/bossDie.wav");


SDL_Color BLACK = { 0,0,0 };
SDL_Color WHITE = { 255,255,255 };

SDL_Rect BossSirenTre = { 120, 0, 40 ,40 };
SDL_Rect BrownSirenTre = { 80, 0, 40 ,40 };
SDL_Rect RedSirenTre = { 0,0,40,40 };
SDL_Rect GreenSirenTre = { 40,0,40,40 };
SDL_Rect enemyExplosion1Tre = { 0, 40, 40, 40 };
SDL_Rect enemyExplosion2Tre = { 0, 80 , 40 ,40 };
SDL_Rect enemyExplosion3Tre = { 40,80, 40 ,40 };
SDL_Rect enemyExplosion4Tre = { 80,80,40,40 };
SDL_Rect enemyExplosion5Tre = { 120,80,40,40 };
SDL_Rect enemyExplosion6Tre = { 160,80,40,40 };

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
	Mix_FreeChunk(mainbgSfx1);
	Mix_FreeChunk(titleSfx);
	Mix_FreeChunk(titleclickSfx);
	Mix_FreeChunk(enemyDieSfx);
	Mix_FreeChunk(playerShootSfx);
	Mix_FreeChunk(gameOverSfx);
	TTF_CloseFont(font50);
	Mix_FreeChunk(mainbgSfx2);
	Mix_FreeChunk(mainbgSfx3);
	Mix_FreeChunk(winSfx);
	Mix_FreeChunk(bossDieSfx);
}

void spawnEnemies(Uint32 time) {
	int yo = time / 1000;
	if (yo == 1 and enemyCS.at(0) != true) {
		Enemy e(REDSIREN, 300, .25);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(),true);
	}
	if (yo == 2 and enemyCS.at(1) != true) {
		Enemy e(REDSIREN, 550, .35);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 3 and enemyCS.at(2) != true) {
		Enemy e(GREENSIREN, 120, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 4 and enemyCS.at(3) != true) {
		Enemy e(REDSIREN, 10, .35);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}if (yo == 4 and enemyCS.at(4) != true) {
		Enemy e(GREENSIREN, 600, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 4 and enemyCS.at(5) != true) {
		Enemy e(GREENSIREN, 350, .4);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 6 and enemyCS.at(6) != true) {
		Enemy e(REDSIREN, 200, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 6 and enemyCS.at(7) != true) {
		Enemy e(REDSIREN, 450, .35);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 7 and enemyCS.at(8) != true) {
		Enemy e(GREENSIREN, 600, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 8 and enemyCS.at(9) != true) {
		Enemy e(REDSIREN, 300, .28);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 8 and enemyCS.at(10) != true) {
		Enemy e(GREENSIREN, 150, .55);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 8 and enemyCS.at(11) != true) {
		Enemy e(GREENSIREN, 650, .40);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 9 and enemyCS.at(12) != true) {
		Enemy e(REDSIREN, 500, .37);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 9 and enemyCS.at(13) != true) {
		Enemy e(GREENSIREN, 1, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 20 and enemyCS.at(14) != true) {
		Enemy e(BROWNSIREN, 320, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 22 and enemyCS.at(15) != true) {
		Enemy e(GREENSIREN, 505, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}if (yo == 22 and enemyCS.at(16) != true) {
		Enemy e(GREENSIREN, 200, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 22 and enemyCS.at(17) != true) {
		Enemy e(BROWNSIREN, 340, .75);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 24 and enemyCS.at(18) != true) {
		Enemy e(BROWNSIREN, 1, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 24 and enemyCS.at(19) != true) {
		Enemy e(BROWNSIREN, 200, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 24 and enemyCS.at(20) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 24 and enemyCS.at(21) != true) {
		Enemy e(BROWNSIREN, 600, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 25 and enemyCS.at(22) != true) {
		Enemy e(REDSIREN, 560, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 25 and enemyCS.at(23) != true) {
		Enemy e(GREENSIREN, 50, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}if (yo == 25 and enemyCS.at(24) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 26 and enemyCS.at(25) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 26 and enemyCS.at(26) != true) {
		Enemy e(BROWNSIREN, 10, .8);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 26 and enemyCS.at(27) != true) {
		Enemy e(REDSIREN, 420, .35);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 26 and enemyCS.at(28) != true) {
		Enemy e(BROWNSIREN, 500, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 26 and enemyCS.at(29) != true) {
		Enemy e(BROWNSIREN, 170, .75);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 26 and enemyCS.at(30) != true) {
		Enemy e(REDSIREN, 260, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 27 and enemyCS.at(31) != true) {
		Enemy e(BROWNSIREN, 1, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 27 and enemyCS.at(32) != true) {
		Enemy e(BROWNSIREN, 200, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 27 and enemyCS.at(33) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 27 and enemyCS.at(34) != true) {
		Enemy e(BROWNSIREN, 600, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 28 and enemyCS.at(35) != true) {
		Enemy e(REDSIREN, 1, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 28 and enemyCS.at(36) != true) {
		Enemy e(BROWNSIREN, 200, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 28 and enemyCS.at(37) != true) {
		Enemy e(GREENSIREN, 400, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 28 and enemyCS.at(38) != true) {
		Enemy e(BROWNSIREN, 600, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 29 and enemyCS.at(39) != true) {
		Enemy e(REDSIREN, 500, .35);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 29 and enemyCS.at(40) != true) {
		Enemy e(GREENSIREN, 260, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 40 and enemyCS.at(41) != true) {
		Enemy e(BOSIREN, 220, .1, 1);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 41 and enemyCS.at(42) != true) {
		Enemy e(BROWNSIREN, 300, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(43) != true) {
		Enemy e(REDSIREN, 1, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(44) != true) {
		Enemy e(BROWNSIREN, 200, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(45) != true) {
		Enemy e(GREENSIREN, 400, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(46) != true) {
		Enemy e(BROWNSIREN, 600, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(47) != true) {
		Enemy e(REDSIREN, 220, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(48) != true) {
		Enemy e(BROWNSIREN, 280, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(49) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(50) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 42 and enemyCS.at(51) != true) {
		Enemy e(GREENSIREN, 460, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 43 and enemyCS.at(52) != true) {
		Enemy e(REDSIREN, 220, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 43 and enemyCS.at(53) != true) {
		Enemy e(BROWNSIREN, 280, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 43 and enemyCS.at(54) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 43 and enemyCS.at(55) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 43 and enemyCS.at(56) != true) {
		Enemy e(GREENSIREN, 460, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(57) != true) {
		Enemy e(GREENSIREN, 365, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(58) != true) {
		Enemy e(GREENSIREN, 140, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(59) != true) {
		Enemy e(GREENSIREN, 400, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(60) != true) {
		Enemy e(GREENSIREN, 570, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(61) != true) {
		Enemy e(REDSIREN, 220, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(62) != true) {
		Enemy e(BROWNSIREN, 280, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(63) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(64) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 44 and enemyCS.at(65) != true) {
		Enemy e(GREENSIREN, 460, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(66) != true) {
		Enemy e(REDSIREN, 10, .2);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(67) != true) {
		Enemy e(REDSIREN, 600, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(68) != true) {
		Enemy e(REDSIREN, 320, .2);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(69) != true) {
		Enemy e(REDSIREN, 460, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(70) != true) {
		Enemy e(REDSIREN, 220, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(71) != true) {
		Enemy e(BROWNSIREN, 280, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(72) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(73) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 45 and enemyCS.at(73) != true) {
		Enemy e(GREENSIREN, 460, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(74) != true) {
		Enemy e(GREENSIREN, 365, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(75) != true) {
		Enemy e(GREENSIREN, 140, .5);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(76) != true) {
		Enemy e(GREENSIREN, 400, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(77) != true) {
		Enemy e(GREENSIREN, 570, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(78) != true) {
		Enemy e(REDSIREN, 220, .3);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(79) != true) {
		Enemy e(BROWNSIREN, 280, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(80) != true) {
		Enemy e(GREENSIREN, 340, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(81) != true) {
		Enemy e(BROWNSIREN, 400, .7);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
	if (yo == 47 and enemyCS.at(82) != true) {
		Enemy e(GREENSIREN, 460, .45);
		enemies.push_back(e);
		enemyCS.insert(enemyCS.begin(), true);
	}
}

Uint32 title() {
	bool keyDown = false;
	bool running = true;
	SDL_Event e;
	Mix_Volume(0, 40);
	Mix_Volume(1, 120);
	Mix_Volume(2, 40);
	Mix_PlayChannel(-1, titleSfx, -1);
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
						if (keyDown == false) {
							keyDown = true;
							running = false;
							Mix_HaltChannel(0);
							Mix_PlayChannel(0, titleclickSfx, 0);
							SDL_Delay(500);
							Mix_HaltChannel(0);
						}
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
	SDL_Event e;
	Mix_PlayChannelTimed(0, gameOverSfx, 0, 1000);
	while (gameOverRunning) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				gameOverRunning = false;
			}
		}
		window.clear(&WHITE);
		window.render(bgTre, bgTreX, 0, 3840, 720, 0);
		window.renderText(500, 320, "GAME OVER", font50, WHITE);
		window.update();
	}
}

void cutScene(int cutSceneNum) {
	int starting = 1280 * cutSceneNum;
	Mix_FadeOutChannel(0, 2000);
	for (int i = 0; i != -1280; i -= 4) {
		bgTreX = i + starting - 1;
		window.clear(&WHITE);
		window.render(bgTre, bgTreX, 0, 3840, 720, NULL);
		window.update();
	}
}

void bossBar(int health) {
	window.render(bossBarInTre, 160, 10, 960 * health, 72, NULL);
}

void winScreen(int x, int y) {
	Mix_PlayChannel(0, winSfx, 0);
	SDL_Event e;
	double j = 0;
	window.clear(&WHITE);
	window.render(bgTre, bgTreX, 0, 3840, 720, NULL);
	double i;
	for (i = 320; i != 460; i += 0.5) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				i = 320;
			}
		}
		j += 0.25;
		window.render(EnemyTre, x - j, y - j, i, i, &BossSirenTre);
		window.update();
	
	}
	window.clear(&WHITE);
	window.render(bgTre, bgTreX, 0, 3840, 720, NULL);
	Mix_PlayChannel(0, bossDieSfx, 0);
	for (int l = 0; l != 6; l++) {
		SDL_Delay(36);
		window.clear(&WHITE);
		window.render(bgTre, bgTreX, 0, 3840, 720, NULL);
		if (l == 0) {
			window.render(EnemyTre, x - j, y - j, i, i, &enemyExplosion1Tre);
		}
		if (l == 1) {
			window.render(EnemyTre, x - j, y  - j, i, i, &enemyExplosion2Tre);
		}
		if (l == 2) {
			window.render(EnemyTre, x - j, y  - j, i, i, &enemyExplosion3Tre);
		}
		if (l == 3) {
			window.render(EnemyTre, x- j, y  - j, i, i, &enemyExplosion4Tre);
		}
		if (l == 4) {
			window.render(EnemyTre, x - j, y  - j, i, i, &enemyExplosion5Tre);
		}
		if (l == 5) {
			window.render(EnemyTre, x - j, y  - j, i, i, &enemyExplosion6Tre);
		}
		window.update();
	}
	window.clear(&WHITE);
	window.render(bgTre, bgTreX, 0, 3840, 720, NULL);
	window.update();
	SDL_Delay(500);

}




int main(int argc, char* argv[]) {
	Uint32 titleTime = title();
	bool running = true;
	if (thatButton == true) {
		running = false;
	}
	bool win = false;
	int winX, winY;
	SDL_Event e;
	string omg;
	SDL_Rect current_clip1;
	std::vector<Bullet> bullets1;
	Bullet b(1, 1, 1275, 715, bulletTre, 20);
	bullets1.push_back(b);
	Uint32 startTime = SDL_GetTicks();
	Mix_PlayChannel(0, mainbgSfx1, -1);
	SDL_Surface *surf;
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
					//shoot
					Mix_PlayChannel(1, playerShootSfx, 0);
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
		window.render(bgTre, bgTreX, 0, 3840, 720, NULL);
		//bullets, move and render
		for (int i = 0; i < bullets1.size(); i++) {
			if (i != bullets1.size() - 1) {
				bullets1.at(0).move();
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
			else {
				window.renderEx(bullets1.at(i).gettexture(), bullets1.at(i).getxpos(), bullets1.at(i).getypos(), bullets1.at(i).getwidth(), bullets1.at(i).getheight(), NULL, bullets1.at(i).getangle());
			}

		}
		startTime = SDL_GetTicks();
		//cutscenes
		int smallTime = startTime - titleTime;
		if (smallTime/ 1000 == 13) {
			cutScene(0);
			Mix_Volume(0, 30);
			Mix_FadeInChannel(0, mainbgSfx2, -1, 1000);
		}
		if (smallTime / 1000 == 33) {
			cutScene(-1);
			Mix_Volume(0, 40);
			Mix_FadeInChannel(0, mainbgSfx3, -1, 1000);
		}
		//spawn new enemies
		spawnEnemies(smallTime);
		//enemy collisions, score and animations
		for (int i = 0; i != enemies.size(); i++) {
			enemies.at(i).move();
			for (int j = 0; j != bullets1.size(); j++) {
				if (checkCollisionCR(enemies.at(i).getCollisionBox(), bullets1.at(j).getCBox())) {
					Bullet b(0, 0, 1, 1, bulletTre,180);
					bullets1.push_back(b);
					bullets1.erase(bullets1.begin() + j);
					if (enemies.at(i).getExplosionState() <= 0) {
						if (enemies.at(i).gettype() == REDSIREN) {
							player1.score += 1;
							Enemy e(REDSIREN, 1000, .5);
							enemies.push_back(e);
							enemies.at(i).changeExplosionState(0);
						}
						else if (enemies.at(i).gettype() == GREENSIREN) {
							player1.score += 2;
							Enemy e(REDSIREN, 1000, .5);
							enemies.push_back(e);
							enemies.at(i).changeExplosionState(0);
						}
						else if (enemies.at(i).gettype() == BROWNSIREN) {
							player1.score += 3;
							Enemy e(REDSIREN, 1000, .5);
							enemies.push_back(e);
							enemies.at(i).changeExplosionState(0);
						}
						else if (enemies.at(i).gettype() == BOSIREN) {
							enemies.at(i).hit();
							bossBar(enemies.at(i).gethealth());
							if (enemies.at(i).gethealth() == 0) {
								winX = enemies.at(i).getxpos();
								winY = enemies.at(i).getypos();
								win = true;
								running = false;
							}
						}
					}
				}
				
			}
			if (player1.score != prevPlayer1Score) {
				Mix_PlayChannel(2, enemyDieSfx, 0);
				prevPlayer1Score = player1.score;
			}
			if (enemies.at(i).getExplosionState() <= 0) {
				if (checkCollisionCC(enemies.at(i).getCollisionBox(), player1.getCBox())) {
					gameOverRunning = true;
					running = false;
				}
			}
			if (enemies.at(i).getxpos() < 0 or enemies.at(i).getxpos() > 1290) {
				Enemy e(REDSIREN, 1000, .99999);
				enemies.push_back(e);
				enemies.erase(enemies.begin() + i);
			}
			else { 
				if (enemies.at(i).getExplosionState() == 0) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &enemyExplosion1Tre, 270);
					enemies.at(i).add1ExplosionState();
				}
				else if (enemies.at(i).getExplosionState() == 1) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &enemyExplosion2Tre, 270);
					enemies.at(i).add1ExplosionState();
				}
				else if (enemies.at(i).getExplosionState() == 2) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &enemyExplosion3Tre, 270);
					enemies.at(i).add1ExplosionState();
				}
				else if (enemies.at(i).getExplosionState() == 3) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &enemyExplosion4Tre, 270);
					enemies.at(i).add1ExplosionState();
				}
				else if (enemies.at(i).getExplosionState() == 4) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &enemyExplosion5Tre, 270);
					enemies.at(i).add1ExplosionState();
				}
				else if (enemies.at(i).getExplosionState() == 5) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &enemyExplosion6Tre, 270);
					enemies.erase(enemies.begin() + i);
				}
				else if (enemies.at(i).gettype() == REDSIREN) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &RedSirenTre,270);
				} 
				else if (enemies.at(i).gettype() == BROWNSIREN) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &BrownSirenTre,270);
				} 
				else if (enemies.at(i).gettype() == GREENSIREN) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 80, 80, &GreenSirenTre, 270);
				}
				else if (enemies.at(i).gettype() == BOSIREN) {
					window.renderEx(EnemyTre, enemies.at(i).getxpos(), enemies.at(i).getypos(), 240, 240, &BossSirenTre, 270);
				}
				if (enemies.at(i).gethealth() >= 0) {
					bossBar(enemies.at(i).gethealth());
				}
			}

		}
		//render score
		window.renderText(10, 10, "Score", font50, WHITE);
		if (player1.score == -1) {
			omg = to_string(player1.score + 1);
		}
		else {
			omg = to_string(player1.score);
		}
		window.renderText(200, 10, omg.c_str(), font50, WHITE);
		//render playera
		window.render(player1Tre, player1.getxpos(), player1.getypos(), player1.getwidth(), player1.getheight(), &current_clip1);
		window.update();
	}
	if (win == true) {
		winScreen(winX, winY);
	}
	//if gameover quit game
	if (gameOverRunning) {
		gameOver();
	}
	close();


	return 0;
}