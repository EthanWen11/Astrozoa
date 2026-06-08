#include "Player.h"

Player::Player() {
	// default geometry for the Player
	verts.clear();
	// head
	verts.push_back(glm::vec3(0, -30, 0));
	verts.push_back(glm::vec3(25, -20, 0));
	verts.push_back(glm::vec3(30, 0, 0));
	verts.push_back(glm::vec3(-30, 0, 0));
	verts.push_back(glm::vec3(-25, -20, 0));

	// tentacles
	verts.push_back(glm::vec3(-17, 0, 0));
	verts.push_back(glm::vec3(-2, 0, 0));
	verts.push_back(glm::vec3(13, 0, 0));
	verts.push_back(glm::vec3(28, 0, 0));

	verts.push_back(glm::vec3(-28, 10, 0));
	verts.push_back(glm::vec3(-13, 10, 0));
	verts.push_back(glm::vec3(2, 10, 0));
	verts.push_back(glm::vec3(17, 10, 0));

	verts.push_back(glm::vec3(-17, 20, 0));
	verts.push_back(glm::vec3(-2, 20, 0));
	verts.push_back(glm::vec3(13, 20, 0));
	verts.push_back(glm::vec3(28, 20, 0));

	verts.push_back(glm::vec3(-28, 30, 0));
	verts.push_back(glm::vec3(-13, 30, 0));
	verts.push_back(glm::vec3(2, 30, 0));
	verts.push_back(glm::vec3(17, 30, 0));

	// default position data
	position = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	force = glm::vec3(0, 0, 0);
	rotation = 0;
	angularVelocity = 0;
	angularAcceleration = 0;
}

void Player::draw() {
	ofPushMatrix();
	ofMultMatrix(getTransform());

	// Head
	ofNoFill();
	ofSetColor(ofColor::aqua);
	ofBeginShape();
	for (int i = 0; i < 5; i++) {
		ofVertex(verts[i].x, verts[i].y);
	}
	ofEndShape(OF_CLOSE);

	// Tentacles
	ofSetColor(ofColor::steelBlue);
	ofSetLineWidth(2);
	for (int col = 0; col < 4; col++) {
		ofBeginShape();
		for (int row = 0; row < 4; row++) {
			int index = 5 + col + row * 4;
			ofVertex(verts[index].x, verts[index].y);
		}
		ofEndShape();
	}

	ofPopMatrix();
}

void Player::update(float dt) {
	// handle Euler Integrator physics movement
	integrate();

	// tentacle flip animation every interval
	float t = ofGetElapsedTimef();

	if (t - lastFlipTime > flipInterval) {
		tentaclesFlipped = !tentaclesFlipped;
		lastFlipTime = t;

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				if (!tentaclesFlipped) {
					if (row % 2 == 0) {
						verts[5 + (4 * row) + col].x += 11;
					} else {
						verts[5 + (4 * row) + col].x -= 11;
					}
				} else {
					if (row % 2 == 0) {
						verts[5 + (4 * row) + col].x -= 11;
					} else {
						verts[5 + (4 * row) + col].x += 11;
					}
				}
			}
		}
	}

	Sprite::wrapAroundScreen();
}

void Player::methodToDebugPlayerInfo() {
	ofLogNotice("Player")
		<< "Position: (" << position.x << ", " << position.y << ") "
		<< "Visible: " << (bShowImage ? "Yes" : "No") << " "
		<< "Velocity: (" << velocity.x << ", " << velocity.y << ") "
		<< "Rotation: " << rotation
		<< "  In screen? " << ((position.x >= 0 && position.x <= ofGetWidth() && position.y >= 0 && position.y <= ofGetHeight()) ? "Yes" : "No");
}
