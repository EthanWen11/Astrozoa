#include "Asteroid.h"

Asteroid::Asteroid(float radius) {
	makeAsteroid(radius);
	radius = radius;
	mass = 1.0;
	damping = 0.9999f;
	angularDrag = 0.9999f;
}

void Asteroid::draw() {
	ofPushMatrix();
	ofMultMatrix(getTransform());

	ofNoFill();
	ofSetColor(ofColor::white);
	ofBeginShape();
	for (auto& p : verts) {
		ofVertex(p.x, p.y);
	}
	ofEndShape(OF_CLOSE);

	ofPopMatrix();
}

void Asteroid::update(float dt) {
	// handle Euler Integrator physics movement
	integrate();

	Sprite::wrapAroundScreen();
}

// Helper method to make random Asteroid vertices using the method described in class
void Asteroid::makeAsteroid(float r) {
	verts.clear();
	int numPoints = 10;
	float angleStep = TWO_PI / numPoints;

	for (int i = 0; i < numPoints; i++) {
		float angle = i * angleStep + ofRandom(-angleStep * 0.25, angleStep * 0.25);
		ofVec3f dir = ofVec3f(cos(angle), sin(angle));
		float t = ofRandom(0.85f * r, 1.15f * r);
		ofVec3f point = position + (dir * t);
		verts.push_back(point);
	}
}
