#pragma once

class Player {
public:
	int score;
	Player(int width, int height);
	~Player();

	void move(SDL_Event e);
	void keydown(SDL_Event e, SDL_Texture* bullet);
	void keyup(SDL_Event e);
	float getxpos(), getypos();
	int getwidth(), getheight();
	SDL_Rect getCurrentClip() { return current_clip; };
	Circle getCBox() { return CollisionBox; };
private:
	double x, y;
	bool goleft, goright, goup, godown;
	int Width, Height;
	Circle CollisionBox;

	//current clip being rendered
	SDL_Rect current_clip;
	//clips
	SDL_Rect player_up;
	SDL_Rect player_down;
	SDL_Rect player_left;
	SDL_Rect player_right;
};
