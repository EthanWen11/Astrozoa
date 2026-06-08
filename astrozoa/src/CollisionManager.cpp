#include "CollisionManager.h"
#include "AsteroidEmitter.h"
#include "ofApp.h"

void CollisionManager::update() {
	if (!aemitters || !emitters || !temp_aemitters) return;

	// Loop through each asteroid in each emitter
	for (auto aemitter : *aemitters) {
		for (int i = static_cast<int>(aemitter->sys->asteroids.size()) - 1; i >= 0; --i) {
			Asteroid & a = aemitter->sys->asteroids[i];

			// Asteroid <--> Player
			if (circleCircleCollision(a.position, a.radius, player->position, player->radius)) {
				aemitter->sys->remove(i);
				player->health -= 1;
				if (playerHitSound) *playerHitSound = true;
				if (player->health <= 0 && !gameOverTriggered) {
					triggerGameOverNextFrame = true;
					gameOverTriggered = true;
				}
				continue;
			}

			// Asteroid <--> Sprite
			for (auto emitter : *emitters) {
				for (int j = static_cast<int>(emitter->sys->sprites.size()) - 1; j >= 0; --j) {
					Sprite & s = emitter->sys->sprites[j];
					float dt = ofGetLastFrameTime();
					if (pointLineCircleCollision(a.position, a.radius, s.position, s.velocity, dt, 2.0f)) {
						emitter->sys->remove(j);
						split(a);
						aemitter->sys->remove(i);
						player->score += 100;
						if (asteroidHitSound) *asteroidHitSound = true;
						goto nextAsteroid; 
					}
				}
			}

		nextAsteroid:;
		}
	}

	// Update temporary AsteroidEmitters from split
	uint64_t now = ofGetElapsedTimeMillis();
	for (auto & t : *temp_aemitters) {
		if (t.emitter) t.emitter->update();
	}

	// Cleanup temporary AsteroidEmitters
	for (int i = static_cast<int>(temp_aemitters->size()) - 1; i >= 0; --i) {
		if (now >= (*temp_aemitters)[i].deleteTime) {
			temp_aemitters->erase(temp_aemitters->begin() + i);
		}
	}
}

// Helper methods for collision detection
bool CollisionManager::circleCircleCollision(const glm::vec3 & c1, float r1,
	const glm::vec3 & c2, float r2) {
	return glm::distance(c1, c2) <= r1 + r2;
}

bool CollisionManager::pointLineCircleCollision(
	const glm::vec3 & circlePos, float circleRadius,
	const glm::vec3 & pointPos, const glm::vec3 & pointVelocity,
	float dt, float tolerance) {
	
	glm::vec3 segStart = pointPos;
	glm::vec3 segEnd = pointPos + pointVelocity * dt;
	glm::vec3 seg = segEnd - segStart;
	glm::vec3 toCircle = circlePos - segStart;

	float segLenSq = glm::dot(seg, seg);
	if (segLenSq < 0.0001f) {
		return glm::distance(segStart, circlePos) <= circleRadius + tolerance;
	}

	float t = glm::dot(toCircle, seg) / segLenSq;
	t = glm::clamp(t, 0.0f, 1.0f);
	glm::vec3 closestPoint = segStart + t * seg;

	return glm::distance(closestPoint, circlePos) <= circleRadius + tolerance;
}

// Helper method to split an Asteroid into four smaller random fragments
void CollisionManager::split(Asteroid & a) {
	auto aemitter = std::make_unique<AsteroidEmitter>();
	aemitter->position = a.position;
	aemitter->lifespan = 1000;
	aemitter->drawable = false;

	for (int i = 0; i < 4; i++) {
		float baseAngle = i * PI / 2.0;
		float offset = glm::radians(ofRandom(-30, 30));
		glm::vec3 dir = glm::vec3(cos(baseAngle + offset), sin(baseAngle + offset), 0);

		Asteroid suba(a.radius * ofRandom(0.25f, 0.5f));
		suba.position = a.position;
		float forceMagnitude = ofRandom(1200, 1700);
		suba.applyForce(dir * forceMagnitude);
		suba.lifespan = 1000;
		suba.birthtime = ofGetElapsedTimeMillis();

		aemitter->sys->add(suba);
	}

	aemitter->start();

	TimedEmitter t;
	t.emitter = std::move(aemitter);
	t.deleteTime = ofGetElapsedTimeMillis() + 3000;

	temp_aemitters->push_back(std::move(t));
	ofLogNotice() << "Created temp emitter at " << a.position << " deleteTime=" << t.deleteTime;
}
