#pragma once

class Particle {
public:
	Particle(int x, int y);
	~Particle();
	bool move();
private:
	int friction;
	int velX, velY;
	int X, Y;
	int negitive;
};
