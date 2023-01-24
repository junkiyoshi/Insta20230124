#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetLineWidth(2);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	vector<glm::vec2> target_list;
	auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
	for (int i = 0; i < 5; i++) {
	
		target_list.push_back(glm::vec2(
			ofMap(ofNoise(noise_seed.x,i * 0.075 + ofGetFrameNum() * 0.005), 0, 1, -230, 230),
			ofMap(ofNoise(noise_seed.y, i * 0.075 + ofGetFrameNum() * 0.005), 0, 1, -230, 230)));
	}

	for (auto& target : target_list) {

		ofDrawCircle(target, 8);
	}

	ofNoFill();
	ofBeginShape();
	ofVertices(target_list);
	ofEndShape();

	for (int deg = 0; deg < 360; deg += 10) {

		int x = 300 * cos(deg * DEG_TO_RAD);
		int y = 300 * sin(deg * DEG_TO_RAD);

		auto target = target_list[0];
		auto distance = glm::distance(glm::vec2(x, y), target_list[0]);
		for (int i = 1; i < target_list.size(); i++) {

			if (distance > glm::distance(glm::vec2(x, y), target_list[i])) {

				target = target_list[i];
				distance = glm::distance(glm::vec2(x, y), target_list[i]);
			}
		}

		this->draw_arrow(glm::vec2(x, y), target, 18, ofColor(0));
	}
}

//--------------------------------------------------------------
void ofApp::draw_arrow(glm::vec2 location, glm::vec2 target, float size, ofColor color) {

	auto angle = std::atan2(target.y - location.y, target.x - location.x);
	auto translate_location = target - glm::vec2(size * 3 * cos(angle), size * 3 * sin(angle));
	auto distance = glm::distance(translate_location, location);

	ofPushMatrix();
	ofTranslate(translate_location);

	ofSetColor(color);
	ofFill();
	ofBeginShape();
	ofVertex(glm::vec2(size * cos(angle), size * sin(angle)));
	ofVertex(glm::vec2(size * 0.25 * cos(angle + PI * 0.5), size * 0.25 * sin(angle + PI * 0.5)));
	ofVertex(glm::vec2(size * 0.25 * cos(angle - PI * 0.5), size * 0.25 * sin(angle - PI * 0.5)));
	ofEndShape();

	ofBeginShape();
	ofVertex(glm::vec2(size * 0.25 * cos(angle + PI * 0.5), size * 0.25 * sin(angle + PI * 0.5)) * 0.25);
	ofVertex(glm::vec2(size * 0.25 * cos(angle + PI * 0.5), size * 0.25 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(distance * cos(angle), distance * sin(angle)));
	ofVertex(glm::vec2(size * 0.25 * cos(angle - PI * 0.5), size * 0.25 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(distance * cos(angle), distance * sin(angle)));
	ofVertex(glm::vec2(size * 0.25 * cos(angle - PI * 0.5), size * 0.25 * sin(angle - PI * 0.5)) * 0.25);

	ofEndShape();

	ofPopMatrix();

	ofDrawCircle(location, 5);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}