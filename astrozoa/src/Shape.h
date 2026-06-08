#pragma once
#include "ofMain.h"

class Shape {

	public:
		glm::vec3 position = glm::vec3(0,0,0);
		float rotation = 0.0; // degrees, converts to radians in code
		glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

		virtual void draw();

		glm::mat4 getTransform() {
			glm::mat4 T = glm::translate(glm::mat4(1.0), position);
			glm::mat4 R = glm::rotate(glm::mat4(1.0), rotation, glm::vec3(0, 0, 1)); // recall that Y+ is "down"
			glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
			return (T * R * S);		// order: scale, rotate, translate
		}

		bool inside(glm::vec3 p) {
			glm::vec3 p1 = glm::inverse(getTransform()) * glm::vec4(p, 1);
			return (glm::distance(p1, glm::vec3(0,0,0)) < radius);
		}
		float radius = 20;
		
};
