#pragma once

#include "ofMain.h"
#include "pingPongBuffer.h"

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
    
    pingPongBuffer particles_p_PingPong;
    ofVboMesh particles_mesh;
    ofFbo fbo_render;
    
    int c_num;
    int tex_w;
    int tex_h;
    
    ofShader sh_p_pos;
    ofShader sh_p_render;
};
