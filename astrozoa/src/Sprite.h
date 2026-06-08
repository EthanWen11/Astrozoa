#pragma once

#include "Shape.h"



// Base class for a Sprite. Can be instantiated on it's own (deafult)
// or subclassed to create a custom sprite.  Includes Shape transformations.
// If the sprite has no image set on it, then a simple triangle will be drawn.
//

class Sprite : public Shape {
public:
	Sprite() {
		// default geometry (lightning bolt) if no image attached.
		//
		verts.push_back(glm::vec3(0, -20, 0));
		verts.push_back(glm::vec3(-5, 0, 0));
		verts.push_back(glm::vec3(5, 0, 0));
		verts.push_back(glm::vec3(0, 20, 0));
	}
	
	// some functions for highlighting when selected
	//
	virtual void draw() {
		if (bShowImage) {
			ofPushMatrix();
			ofSetColor(ofColor::white);
			ofMultMatrix(getTransform());
			spriteImage.draw(-spriteImage.getWidth() / 2, -spriteImage.getHeight() / 2.0);
			ofPopMatrix();
		}
		else
		{
			if (bHighlight) ofSetColor(ofColor::white);
			else ofSetColor(ofColor::yellow);
			ofPushMatrix();
			ofMultMatrix(getTransform());

			ofDrawLine(verts[0], verts[1]);
			ofDrawLine(verts[1], verts[2]);
			ofDrawLine(verts[2], verts[3]);

			ofPopMatrix();
		}
	}

	float age() {
		return (ofGetElapsedTimeMillis() - birthtime);
	}

	void setImage(ofImage img) {
		spriteImage = img;
		bShowImage = true;
		width = img.getWidth();
		height = img.getHeight();
	}

	virtual bool inside(const glm::vec3 p);
	virtual void update() {}

	bool insideTriangle(const glm::vec3 p);
	

	void setSelected(bool state) { bSelected = state; }
	void setHighlight(bool state) { bHighlight = state; }
	bool isSelected() { return bSelected; }
	bool isHighlight() { return bHighlight; }


	bool bHighlight = false;
	bool bSelected = false;
	bool bShowImage = false;

	glm::vec3 velocity = glm::vec3(0, 0, 0);
	glm::vec3 acceleration = glm::vec3(0, 0, 0);
	float angularVelocity = 0.0;
	float angularAcceleration = 0.0;
	float linearDrag = 0.95f;
	float angularDrag = 0.90f;
	float thrustPower = 400.0f;
	float turnPower = glm::radians(2.0f) * 200.0f;

	float birthtime = 0; // elapsed time in ms
	float lifespan = -1;  //  time in ms
	string name =  "UnammedSprite";
	float width = 40;
	float height = 40;

	ofImage spriteImage;

	// default verts for polyline shape if no image on sprite
	//
	vector<glm::vec3> verts;

	void wrapAroundScreen();

	float mass = 1.0f;
	glm::vec3 force = glm::vec3(0, 0, 0);
	float damping = 0.99f;

	void integrate() {
		if (ofGetFrameRate() <= 0) return;
		float dt = 1.0 / ofGetFrameRate();
		// (1) update position from velocity and time interval
		position += velocity * dt;
		rotation += angularVelocity * dt;
		// (2) update velocity (based on acceleration
		ofVec3f accel = acceleration;
		accel = 1.0 / mass * force;
		velocity += accel * dt;
		angularVelocity += angularAcceleration * dt;
		// (3) multiply final result by the damping factor to sim drag
		velocity = velocity * damping;
		angularVelocity *= angularDrag;
		// (4) zero out forces to reset them
		force = glm::vec3(0, 0, 0);
		angularAcceleration = 0.0f;
	}

	void applyForce(const glm::vec3 & f) {
		force += f;
	}
};

