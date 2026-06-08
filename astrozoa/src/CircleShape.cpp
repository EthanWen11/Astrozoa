#include "CircleShape.h"
#include "ofMain.h"

CircleShape::CircleShape(glm::vec3 pos, float r) {
	position = pos;
	radius = r;
}

CircleShape::CircleShape() {
	position = glm::vec3(0, 0, 0);
	radius = 0;
}

void CircleShape::draw() {
	ofPushMatrix();
	ofMultMatrix(Shape::getTransform());
	ofDrawCircle(glm::vec3(0,0,0), radius);
	ofPopMatrix();
	ofSetColor(255,0,255);
	ofDrawLine(position, glm::vec3(radius,0,0));
}

bool CircleShape::inside(glm::vec3 point) {
	if (glm::distance(point, position) < radius) {
		return true;
	} else {
		return false;
	}
}
