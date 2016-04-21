#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofAppGLFWWindow window;
//	window.setMultiDisplayFullscreen(true);

	ofSetupOpenGL(&window,1920*4,1080,OF_WINDOW);			// <-------- setup the GL context
	//ofSetWindowPosition(0, 0);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
