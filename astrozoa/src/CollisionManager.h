#pragma once
#include "AsteroidEmitter.h"
#include "Player.h"
#include "Sprite.h"
#include <memory>

struct TimedEmitter {
	std::unique_ptr<AsteroidEmitter> emitter;
	uint64_t deleteTime;
};

class CollisionManager {
public:
	Player* player = nullptr;
	std::vector<AsteroidEmitter*>* aemitters = nullptr;
	std::vector<Emitter*>* emitters = nullptr;
	std::vector<TimedEmitter>* temp_aemitters = nullptr;

	void update();

	void split(Asteroid & a);

	bool * asteroidHitSound = nullptr;
	bool * playerHitSound = nullptr;

	bool gameOverTriggered = false;
	std::function<void()> onGameOver;
	bool triggerGameOverNextFrame = false;

private:
	bool circleCircleCollision(	const glm::vec3 & c1, float r1,
								const glm::vec3 & c2, float r2);
	bool pointLineCircleCollision(
		const glm::vec3 & circlePos, float circleRadius,
		const glm::vec3 & pointPos, const glm::vec3 & pointVelocity,
		float dt, float tolerance = 2.0f);
};
