#pragma once

class Enemy {
public:
	Enemy(EnemyType Type = REDSIREN, double Y = 0, double Speed = 1);

	void move();

	Circle getCollisionBox() { return CollisionBox; }
	float getxpos() { return x; };
	float getypos() { return y; };
	EnemyType gettype() { return type; };
private:
	Circle CollisionBox;

	double x, y, speed;
	EnemyType type;
};
