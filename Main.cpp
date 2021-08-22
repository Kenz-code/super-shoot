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
#include "Bullet.h"

using namespace std;
Window window;
Player player1(80,80);

SDL_Texture* player1Tre = window.loadFromFilee("res/Player_spritesheet.png");
SDL_Texture* bulletTre = window.loadFromFilee("res/Bullet.png");
SDL_Texture* blankTre = window.loadFromFilee("res/blank.png");
SDL_Texture* titleTre = window.loadFromFilee("res/title.png");
SDL_Texture* bgTre = window.loadFromFilee("res/bg.png");

Mix_Chunk* mainbgSfx = Mix_LoadWAV("res/1.wav");
Mix_Chunk* titleSfx = Mix_LoadWAV("res/title.wav");

SDL_Color BLACK = { 0,0,0 };
SDL_Color WHITE = { 255,255,255 };

void close() {
	SDL_DestroyTexture(player1Tre);
	SDL_DestroyTexture(bulletTre);
	SDL_DestroyTexture(blankTre);
	SDL_DestroyTexture(titleTre);
	SDL_DestroyTexture(bgTre);
	Mix_FreeChunk(mainbgSfx);
	Mix_FreeChunk(titleSfx);
}

void title() {
	bool running = true;
	SDL_Event e;
	Mix_PlayChannel(0, titleSfx, -1);
	while (running) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = false;
				Mix_HaltChannel(0);
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						running = false;
						Mix_HaltChannel(0);
				}
			}
		}
		window.clear(&WHITE);
		window.render(titleTre, 0, 0, 1280, 720, NULL);
		window.update();

	}
}



int main(int argc, char* argv[]) {
	title();
	bool running = true;
	SDL_Event e;
	SDL_Rect current_clip1;
	std::vector<Bullet> bullets1;
	Bullet b(1, 1, 1275, 715, bulletTre, 20);
	bullets1.push_back(b);
	Mix_PlayChannel(-1, mainbgSfx, -1);
	while (running) {
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
		window.render(player1Tre, player1.getxpos(), player1.getypos(), player1.getwidth(), player1.getheight(), &current_clip1);
		window.update();
	}
	close();


	return 0;
}