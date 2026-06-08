#pragma once
#include "Shape.h"

class CircleShape : public Shape {

	public:
		float radius;

		CircleShape();
		CircleShape(glm::vec3 pos, float r);

		void draw();
		bool inside(glm::vec3 point);
};
