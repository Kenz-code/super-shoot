#pragma once

class Window {
public:
	Window();
	~Window();

	void render(SDL_Texture* Layer = NULL, int x = 0, int y = 0, int Width = 0, int Height = 0, SDL_Rect* clip = NULL);
	void renderEx(SDL_Texture* Layer = NULL, int x = 0, int y = 0, int Width = 0, int Height = 0, SDL_Rect* clip = NULL, double angle = 0.0);
	void renderText(int x = 0, int y = 0, const char* text = "", TTF_Font* font = NULL, SDL_Color textColor = { 0,0,0 });
	SDL_Texture* loadFromFilee(const char* path);
	void update();
	void clear(SDL_Color* color);
	void updateDelta();
	double returnDelta();
private:
	//delta time varibles
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

extern double deltaTime;
enum EnemyType { REDSIREN, GREENSIREN, BROWNSIREN, BOSIREN };
struct Circle
{
	double x, y;
	int r;
};