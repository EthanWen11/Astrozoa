#pragma once

#include "Emitter.h"
#include "Asteroid.h"

class AsteroidList {
public:
	void add(Asteroid);
	void remove(int);
	void update();
	void draw();
	void clear();
	vector<Asteroid> asteroids;
};

class AsteroidEmitter : public Emitter {
public:
	AsteroidList * sys;
	AsteroidEmitter();

	void update();
	void draw();
	void spawnSprite() override;
	void applyForces(Asteroid & asteroid);

	void teleportToEdge();

private:
	glm::vec3 acceleration;
	float nextTeleportTime;
	float teleportInterval;
};
