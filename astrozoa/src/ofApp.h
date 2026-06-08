#pragma once

#include "ofMain.h"
#include "CircleShape.h"
#include "ImageShape.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Player.h"
#include "AsteroidEmitter.h"
#include "CollisionManager.h"
#include "ParticleEmitter.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		Player player;

		bool leftPressed;
		bool rightPressed;
		bool upPressed;
		bool downPressed;

		bool bHide;
		ofxPanel gui;
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxFloatSlider speed;
		ofxLabel screenSize;
		ofxFloatSlider rotationSpeed;
		ofxFloatSlider scaleSlider;

		Emitter * emitter = NULL;
		vector<Emitter *> emitters;
		AsteroidEmitter * aemitter = NULL;
		vector<AsteroidEmitter *> aemitters;
		float timeToNextEmitter;
		float timeBetweenEmitters;
		CollisionManager c;
		TimedEmitter * temp_aemitter = NULL;
		vector<TimedEmitter> temp_aemitters;

		ofImage image;
		ImageShape healthIcon;
		float healthIconSize = 32.0f;
		void gameOverScript();
		bool bGameOver = false;
		ofImage background;
		ImageShape backgroundShape;
		ParticleEmitter explodeEmitter;

		// sound
		bool thrustSound;
		ofSoundPlayer thrustSFX;
		bool asteroidHitSound;
		ofSoundPlayer asteroidHitSFX;
		bool playerHitSound;
		ofSoundPlayer playerHitSFX;

};
