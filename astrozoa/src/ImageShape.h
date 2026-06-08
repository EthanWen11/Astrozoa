#pragma once
#include "Shape.h"

class ImageShape : public Shape {
public:
	ofImage image;

	ImageShape() { };
	ImageShape(ofImage image) { this->image = image; }

	void setImage(ofImage image) { this->image = image; }

	void draw() {
		ofPushMatrix();
		ofMultMatrix(getTransform());
		ofSetColor(ofColor::white);
		image.draw(glm::vec3(-image.getWidth() / 2.0, -image.getHeight()/2.0, 0));
		ofPopMatrix();
	}
	
	bool inside(glm::vec3 p) {
		glm::vec3 p1 = glm::inverse(Shape::getTransform()) * glm::vec4(p, 1.0f);
		p1.x += image.getWidth() * 0.5f;
		p1.y += image.getHeight() * 0.5f;

		// Check if point is inside the rectangle
		if (p1.x >= 0 && p1.x < image.getWidth() && p1.y >= 0 && p1.y < image.getHeight()) {

			// check alpha channel of the pixel
			ofColor pixelColor = image.getColor(p1.x, p1.y);
			if (pixelColor.a != 0) { // non-transparent
				return true;
			}
		}

		return false;
	}
};
