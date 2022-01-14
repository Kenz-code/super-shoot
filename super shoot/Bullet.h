#pragma once
class Bullet {
public:
	Bullet(int width, int height, int x, int y, SDL_Texture* texture, int angle);
	~Bullet();

	void move();

	int getxpos() { return xp; };
	int getypos() { return yp; };
	int getwidth() { return Width; };
	int getheight() { return Height; };
	int getangle() { return Angle; };
	SDL_Texture* gettexture() { return Texture; };
	SDL_Rect getCBox() { return CollisionBox; };
private:
	SDL_Rect CollisionBox;
	int Width, Height;
	int xp, yp;
	int Angle;
	SDL_Texture* Texture;

};