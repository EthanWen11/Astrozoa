#pragma once
#include "Sprite.h"

// Class to implement Player functionality
// Extends Sprite.h for physics and Shape.h for transformations
// Hosts the default sprite vector constructor
// Manages Player movement when public functions are called by keyPressed
// Handles Player Health and Death

class Player : public Sprite {
public:
	bool tentaclesFlipped = false;
	float lastFlipTime = 0.0f;
	float flipInterval = 0.8f;

	float thrustPower = 200.0f;
	float linearDrag = 0.9f;
	float radius = 30;

	float health = 3;
	float score = 0;
	float scoreToNextEmitter = 1000;

	Player();

	void draw() override;

	void update(float dt);

	void methodToDebugPlayerInfo();
};
