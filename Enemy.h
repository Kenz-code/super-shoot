#pragma once

class Enemy {
public:
	Enemy(EnemyType Type = REDSIREN, double Y = 0, double Speed = 1, int HEALTH = -1);

	void move();
	void hit();
	void changeExplosionState(int i);
	int getExplosionState();
	void add1ExplosionState();

	Circle getCollisionBox() { return CollisionBox; }
	float getxpos() { return x; };
	float getypos() { return y; };
	EnemyType gettype() { return type; };
	int gethealth() { return health; };
	void changehealth(int i) { health = i; };
	std::vector<Particle> particles;
private:
	Circle CollisionBox;
	double x, y, speed;
	EnemyType type;
	int health;
	int explosionState;
};
