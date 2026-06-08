#include "ofApp.h"
#include "Shape.h"

//--------------------------------------------------------------
void ofApp::setup(){
	player = Player();

	timeBetweenEmitters = 8000;
	timeToNextEmitter = timeBetweenEmitters;

	// Create an array of Emitters for the Player
	int numEmitters = 3;
	for (int i = 0; i < numEmitters; i++) {
		emitter = new Emitter();

		float baseStart = -30.0f;
		float baseEnd = 30.0f;
		float baseY = 0.0f;
		float spacing = (baseEnd - baseStart) / (numEmitters - 1);

		float x = baseStart + i * spacing;
		glm::vec3 offset(x, baseY, 0);
		glm::vec3 pos = player.position + offset;

		emitter->position = pos;
		emitter->rotation = -player.rotation;
		emitter->drawable = false;
		emitter->start();
		emitters.push_back(emitter);
	}

	// Start with just one Asteroid Emitter at setup
	aemitter = new AsteroidEmitter();
	aemitter->drawable = false;
	aemitter->start();
	aemitters.push_back(aemitter);

	// Emitter GUI
	gui.setup("GUI");
	gui.add(rate.setup("rate", 1, 1, 10));
	gui.add(life.setup("life", 5, .1, 10));
	gui.add(speed.setup("speed", 200, 200, 1000));
	gui.add(scaleSlider.setup("Scale", 1, 0, 10));

	bHide = true;

	c.player = &player;
	c.aemitters = &aemitters;
	c.emitters = &emitters;
	c.temp_aemitters = &temp_aemitters;

	image.load("images/health.png");
	if (!image.isAllocated()) {
		ofLogError() << "Failed to load health.png!";
	}
	healthIcon.setImage(image);
	float scaleX = healthIconSize / image.getWidth();
	float scaleY = healthIconSize / image.getHeight();
	healthIcon.scale = glm::vec3(scaleX, scaleY, 1.0f);

	background.load("images/uthros.png");
	if (!image.isAllocated()) {
		ofLogError() << "Failed to load uthros.png!";
	}
	backgroundShape.setImage(background);
	backgroundShape.position = glm::vec3(ofGetWidth() / 2, ofGetHeight() / 2, 0);

	thrustSFX.load("sounds/thrust.wav");
	thrustSFX.setLoop(true);
	thrustSFX.setVolume(0.3);
	asteroidHitSFX.load("sounds/zap.wav");
	asteroidHitSFX.setVolume(0.4);
	playerHitSFX.load("sounds/bubble.wav");
	playerHitSFX.setVolume(0.7);

	c.asteroidHitSound = &asteroidHitSound;
	c.playerHitSound = &playerHitSound;

	explodeEmitter.setVelocity(ofVec3f(0, -200, 0));
	explodeEmitter.setOneShot(true);
	explodeEmitter.setEmitterType(RadialEmitter);
	explodeEmitter.setGroupSize(500);
	explodeEmitter.setLifespan(1000);
	explodeEmitter.setRate(1.0);
	explodeEmitter.setParticleRadius(3.0);
	explodeEmitter.visible = false;
}

//--------------------------------------------------------------
void ofApp::update() {

	player.methodToDebugPlayerInfo();

	// handle Player movement controls
	float dt = ofGetLastFrameTime();
	float baseRot = player.rotation;
	glm::vec3 headingDir(sin(baseRot), -cos(baseRot), 0);

	if (upPressed && !bGameOver) {
		player.applyForce(headingDir * player.thrustPower);
	}
	if (downPressed && !bGameOver) {
		player.applyForce(-headingDir * player.thrustPower);
	}
	if (leftPressed && !bGameOver) {
		player.angularAcceleration -= player.turnPower;
	}
	if (rightPressed && !bGameOver) {
		player.angularAcceleration += player.turnPower;
	}

	player.update(dt);

	// Update Player Emitters

	float projSpeed = speed;
	glm::vec3 projVel = -headingDir * projSpeed;

	// If Player's Score is above a certain threshold, add a new Emitter
	if (player.score > player.scoreToNextEmitter && emitters.size() < 10) {
		emitter = new Emitter();
		emitter->drawable = false;
		emitter->start();
		emitters.push_back(emitter);
		player.scoreToNextEmitter += 1000;
	}

	for (int i = 0; i < emitters.size(); i++) {

		int numEmitters = emitters.size();

		// handle dynamic spacing for any number of Emitters
		float baseStart = -30.0f;
		float baseEnd = 30.0f;
		float baseY = 0.0f;
		float spacing = (baseEnd - baseStart) / (numEmitters - 1);

		// handle position and facing of Emitters
		float x = baseStart + i * spacing;
		float angle = player.rotation;
		float cosA = cos(angle);
		float sinA = sin(angle);
		glm::vec3 rotatedOffset(x * cosA - baseY * sinA, x * sinA + baseY * cosA, 0);
		glm::vec3 pos = player.position + rotatedOffset;

		emitters[i]->position = pos;
		emitters[i]->rotation = baseRot;
		emitters[i]->setRate(rate);
		emitters[i]->setLifespan(life * 1000); // convert to milliseconds
		emitters[i]->setVelocity(projVel);
		emitters[i]->update();
	}
	for (int i = 0; i < emitters.size(); i++) {
		for (int j = 0; j < emitters[i]->sys->sprites.size(); j++) {

			// Get values from sliders and update sprite systems dynamically
			//
			float sc = scaleSlider / 6.0;
			emitters[i]->sys->sprites[j].scale = glm::vec3(sc, sc, sc);
		}
	}

	// Every set amount of time, create a new Asteroid Emitter to increase the difficulty
	float time = ofGetElapsedTimeMillis();
	if (time > timeToNextEmitter && aemitters.size() < 10) {
		timeToNextEmitter = time + timeBetweenEmitters;
		aemitter = new AsteroidEmitter();
		aemitter->drawable = false;
		aemitter->start();
		aemitters.push_back(aemitter);
	}

	// Update Asteroid Emitters
	for (int i = 0; i < aemitters.size(); i++) {
		aemitters[i]->update();
	}

	// Update Collision Manager
	c.update();

	if (c.triggerGameOverNextFrame && !bGameOver) {
		gameOverScript();
		c.triggerGameOverNextFrame = false;
	}

	// Update Thrust Sound Effect
	if (thrustSound && !thrustSFX.isPlaying()) {
		thrustSFX.play();
	} else if (!thrustSound && thrustSFX.isPlaying()) {
		thrustSFX.stop();
	}
	if (asteroidHitSound && !asteroidHitSFX.isPlaying()) {
		asteroidHitSFX.play();
		asteroidHitSound = false;
	} 
	if (playerHitSound && !playerHitSFX.isPlaying()) {
		playerHitSFX.play();
		playerHitSound = false;
	}

	explodeEmitter.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
	backgroundShape.draw();
	if (!bGameOver) {
		player.draw();

		// Draw all Emitters
		for (int i = 0; i < emitters.size(); i++) {
			emitters[i]->draw();
		}
		ofSetColor(ofColor::white);
		for (int i = 0; i < aemitters.size(); i++) {
			aemitters[i]->draw();
		}
		if (c.temp_aemitters && !c.temp_aemitters->empty()) {
			for (auto & t : *c.temp_aemitters) {
				if (t.emitter) t.emitter->draw();
			}
		}

		// Display Health Icons
		float padding = 10.0f;
		ofSetColor(ofColor::white);
		for (int i = 0; i < player.health; i++) {
			float x = ofGetWidth() - (i + 1) * (healthIconSize + padding) - healthIconSize / 2;
			float y = padding + healthIconSize / 2;
			healthIcon.position = glm::vec3(x, y, 0);
			healthIcon.draw();
		}

		// Display Score
		ofxGuiSetTextColor(ofColor::yellow);
		std::string scoreStr = "Score: " + std::to_string(static_cast<int>(player.score));
		ofDrawBitmapStringHighlight(scoreStr, 10, 30);

		if (!bHide) {
			gui.draw();
		}
	} else {
		ofSetColor(ofColor::blue);
		explodeEmitter.draw();

		ofSetColor(255, 0, 0);
		ofDrawBitmapString("GAME OVER", ofGetWidth() / 2 - 50, ofGetHeight() / 2);
		ofDrawBitmapString("Final Score: " + std::to_string(static_cast<int>(player.score)), ofGetWidth() / 2 - 50, ofGetHeight() / 2 + 50);
		return;
	}
}

void ofApp::gameOverScript() {
	bGameOver = true;

	ofLogNotice() << "Game Over! Final score: " << static_cast<int>(player.score);

	for (auto * e : emitters)
		e->sys->clear();
	for (auto * a : aemitters)
		a->sys->clear();
	for (auto & t : temp_aemitters) {
		if (t.emitter) t.emitter->sys->clear();
	}

	emitters.clear();
	aemitters.clear();
	temp_aemitters.clear();

	c.aemitters = nullptr;
	c.emitters = nullptr;
	c.temp_aemitters = nullptr;

	explodeEmitter.setPositionPublic(player.position);
	explodeEmitter.sys->reset();
	explodeEmitter.start();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	thrustSound = true;
	if (key == OF_KEY_LEFT) {
		leftPressed = true;
	}
	if (key == OF_KEY_RIGHT) {
		rightPressed = true;
	}
	if (key == OF_KEY_UP) {
		upPressed = true;
	}
	if (key == OF_KEY_DOWN) {
		downPressed = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	thrustSound = false;
	if (key == OF_KEY_LEFT) {
		leftPressed = false;
	}
	if (key == OF_KEY_RIGHT) {
		rightPressed = false;
	}
	if (key == OF_KEY_UP) {
		upPressed = false;
	}
	if (key == OF_KEY_DOWN) {
		downPressed = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
