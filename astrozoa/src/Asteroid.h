#pragma once
#include "Sprite.h"

// Class to implement Asteroid functionality
// Extends Sprite.h for physics and Shape.h for transformations
// Hosts the helper function for making random Asteroid shapes

class Asteroid : public Sprite {
public:

	Asteroid(float radius);

	void draw() override;
	void update(float dt);
	void makeAsteroid(float r);

};
