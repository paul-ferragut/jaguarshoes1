#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofAppGLFWWindow window;

	if(NUM_SCREEN>1){
	window.setMultiDisplayFullscreen(true);
	}
	ofSetupOpenGL(&window,1920*NUM_SCREEN,1080,OF_WINDOW);			// <-------- setup the GL context
	//ofSetWindowPosition(0, 0);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
