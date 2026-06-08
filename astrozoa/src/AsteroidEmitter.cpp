#include "AsteroidEmitter.h"

void AsteroidList::add(Asteroid s) {
	asteroids.push_back(s);
}

void AsteroidList::remove(int i) {
	asteroids.erase(asteroids.begin() + i);
}

void AsteroidList::clear() {
	asteroids.clear();
}

void AsteroidList::update() {
	float dt = ofGetLastFrameTime();

	if (asteroids.size() == 0) return;
	vector<Asteroid>::iterator s = asteroids.begin();
	vector<Asteroid>::iterator tmp;

	while (s != asteroids.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			tmp = asteroids.erase(s);
			s = tmp;
		} else
			s++;
	}

	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i].update(dt);
	}
}

void AsteroidList::draw() {
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i].draw();
	}
}

AsteroidEmitter::AsteroidEmitter() {
	sys = new AsteroidList();
	teleportInterval = 2000 + ofRandom(0, 2000);
	nextTeleportTime = ofGetElapsedTimeMillis() + teleportInterval;
	acceleration = glm::vec3(0, 0, 0);
	lifespan = 30000;
}

void AsteroidEmitter::update() {
	float time = ofGetElapsedTimeMillis();
	if (time > nextTeleportTime) {
		teleportToEdge();
		spawnSprite();
		nextTeleportTime = time + teleportInterval + ofRandom(0, 2000);
	}

	if (sys->asteroids.size() == 0) return;
	vector<Asteroid>::iterator s = sys->asteroids.begin();
	vector<Asteroid>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sys->asteroids.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			tmp = sys->asteroids.erase(s);
			s = tmp;
		} else
			s++;
	}

	sys->update();
}

void AsteroidEmitter::draw() {
	sys->draw();
}

void AsteroidEmitter::spawnSprite() {
	float radius = ofRandom(10, 50);
	Asteroid asteroid(radius);
	asteroid.lifespan = lifespan;
	asteroid.birthtime = ofGetElapsedTimeMillis();
	asteroid.position = position;

	glm::vec3 toCenter = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0) - position;
	toCenter = glm::normalize(toCenter);
	float angleVariance = glm::radians(ofRandom(-20, 20));
	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angleVariance, glm::vec3(0, 0, 1));
	glm::vec3 variedDir = glm::vec3(rot * glm::vec4(toCenter, 0));
	float forceMagnitude = ofRandom(3000, 6000);
	glm::vec3 launchForce = variedDir * forceMagnitude;
	asteroid.applyForce(launchForce);
	asteroid.angularVelocity = ofRandom(-0.2,0.2);

	sys->add(asteroid);
}

void AsteroidEmitter::applyForces(Asteroid & asteroid) {
	acceleration = glm::vec3(0, 0, 0); 

	glm::vec3 toCenter = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0) - asteroid.position;
	float dist = glm::length(toCenter);
	if (dist > 0.01f)
		acceleration += glm::normalize(toCenter) * 50.0f / (dist * 0.05f);
}

void AsteroidEmitter::teleportToEdge() {
	int side = ofRandom(4); // 0=top, 1=right, 2=bottom, 3=left
	float w = ofGetWidth();
	float h = ofGetHeight();

	switch (side) {
	case 0:
		position = glm::vec3(ofRandom(0, w), 0, 0);
		break; // top
	case 1:
		position = glm::vec3(w, ofRandom(0, h), 0);
		break; // right
	case 2:
		position = glm::vec3(ofRandom(0, w), h, 0);
		break; // bottom
	case 3:
		position = glm::vec3(0, ofRandom(0, h), 0);
		break; // left
	}
}
