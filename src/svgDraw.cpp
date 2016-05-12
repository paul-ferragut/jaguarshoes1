#include "svgDraw.h"

svgDraw::svgDraw()
{

}

//--------------------------------------------------------------
svgDraw::~svgDraw()
{

}

//--------------------------------------------------------------
void svgDraw::setup(string svgString) {
    
    ofSetBoxResolution( 30, 30, 30 );
    
	/*
    cam.disableMouseInput();
    cam.setDistance( 10 );
    cam.setPosition(0, 0, -10 );
    cam.lookAt( ofVec3f(0,0,0), ofVec3f(0,-1,0) );
    cam.setNearClip(1);
    cam.setFarClip(150);
    
    cam.enableMouseInput();
    
    // range of the shadow camera //
    shadow.setRange( 10, 150 );
    shadow.setBias( 0.01 );
    
    bunny.load( "lofi-bunny.ply" );
    
    vector< ofMeshFace > faces = bunny.getUniqueFaces();
    for( int i = 0; i < faces.size(); i++ ) {
        faces[i].setVertex( 0, faces[i].getVertex(0 ));
        faces[i].setNormal(0, faces[i].getFaceNormal() );
        faces[i].setNormal(1, faces[i].getFaceNormal() );
        faces[i].setNormal(2, faces[i].getFaceNormal() );
    }
    bunny.setFromTriangles( faces );
    bunny.smoothNormals( 60 );
    cout << "Bunny normals = " << bunny.getNumNormals() << endl;
	*/
	//"illu2.svg"
	svg.load(svgString);

	//cam.setLookAt(ofx2DCam::OFX2DCAM_TOP_INVERT);
	//cam.setTranslation2D(ofVec2f(0.0, 1080));
	//cam.setScale(2.0);
	//cam.disableMouseInput();


	//ofMesh mesh;
	vector<ofColor>groupByCol;
	groupByCol.clear();

	for (int i = 0; i < svg.getNumPath(); i++) {

		ofPath p = svg.getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		ofColor colToDepth = p.getFillColor();
		p.setColor(ofColor(255, 255, 255));
		//p.setFillColor(ofColor(255, 255, 255));
		if (isGray(colToDepth)) {
			float zPos = ofMap(colToDepth.getBrightness(), 0, 255, 0, -200);
			paths.push_back(p);
			zPosition.push_back(zPos);
		}
		else {
			bool colExist = false;

			for (int j = 0;j < groupByCol.size();j++) {
				if (colToDepth == groupByCol[j]) {
					colExist = true;
				}
			}
			if (colExist == false) {
				groupByCol.push_back(colToDepth);
				//cout << "color" << colToDepth << endl;
				//vector<ofPath>tP;
				//ofPath pp
				//tP.push_back(pp);
				//pathsShader.push_back(tP);
			}
		}
	}
	//cout << "first loop" << endl;
	vector<ofPath>tempP;
	tempP.clear();
	//tempP.assign(1,tp);
	pathsShader.assign(groupByCol.size(), tempP);
	//cout << groupByCol.size() << "group" << endl;

	for (int i = 0; i < svg.getNumPath(); i++) {

		ofPath p = svg.getPathAt(i);
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		ofColor colToDepth = p.getFillColor();

		if (isGray(colToDepth)) {
		}
		else {
			///bool colExist = false;
			for (int j = 0;j < groupByCol.size();j++) {
				if (groupByCol[j] == colToDepth) {
					pathsShader[j].push_back(p);
					//return;
				}
			}
		}

	}


	for (int i = 0; i < paths.size();i++) {

		//paths[i].setPolyWindingMode(OF_POLY_WINDING_ODD);
		//cout << paths[i].getFillColor() << endl;
		paths[i].setColor(ofColor(255,0,255));
		paths[i].setFillColor(ofColor(0, 0, 255));
		paths[i].setPolyWindingMode(OF_POLY_WINDING_ODD);
		paths[i].setUseShapeColor(false);
		//paths[i].set
		//cout <<"after"<< paths[i].getFillColor() << endl;
		
	}

}

bool svgDraw::isGray(ofColor cGray) {
	bool gray = true;
	if (cGray.r >= cGray.g - 2 && cGray.r <= cGray.g + 2) {
	}
	else {
		gray = false;
	}

	if (cGray.g >= cGray.b - 2 && cGray.g <= cGray.b + 2) {
	}
	else {
		gray = false;
	}

	return gray;

}

//--------------------------------------------------------------
void svgDraw::update(){
    //shadow.setLightPosition( ofVec3f(cos(ofGetElapsedTimef()*0.6) * 10, -3, -25) );
    //shadow.setLightLookAt( ofVec3f(0,0,0), ofVec3f(0,-1,0) );
}

//--------------------------------------------------------------
void svgDraw::draw(){
    
	ofSetColor(255, 255, 255);
	//ofPushMatrix();
	for (int i = 0; i < paths.size();i++) {
		ofPushMatrix();
		ofTranslate(0, 0, zPosition[i]);
		//ofSetColor(255, 255, 255);
		//paths[i].setPolyWindingMode(OF_POLY_WINDING_ODD);
		paths[i].draw();
		//cout << "during" << paths[i].getFillColor() << endl;
		ofPopMatrix();
	}
	//ofPopMatrix();

}
