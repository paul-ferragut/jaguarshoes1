#pragma once

#include "ofMain.h"
#define NUM_COLORS_IN_PALETTE 5


struct colorPalette {

	float r[NUM_COLORS_IN_PALETTE], g[NUM_COLORS_IN_PALETTE], b[NUM_COLORS_IN_PALETTE];
	float rFloat[NUM_COLORS_IN_PALETTE], gFloat[NUM_COLORS_IN_PALETTE], bFloat[NUM_COLORS_IN_PALETTE];


	string timeCode;
	int hour;
	int minute;
	int second;

	ofColor getCol(int i) {
		return ofColor(r[i], g[i], b[i]);
	}


	vector<ofColor>getCols() {

		vector<ofColor>passCol;
		for (int i = 0;i < NUM_COLORS_IN_PALETTE;i++) {
			passCol.push_back(getCol(i));
		}

		return passCol;
	}

	ofFloatColor getColFloat(int i) {
		return ofFloatColor(rFloat[i], gFloat[i], bFloat[i]);
	}

	void setColors(vector<ofColor>col) {

		for (int i = 0;i < col.size();i++) {
			if (col.size() <= NUM_COLORS_IN_PALETTE) {
				r[i] = col[i].r;
				g[i] = col[i].g;
				b[i] = col[i].b;
			}
		}

	}

	void setCol(ofColor col, int i) {
		r[i] = col.r;
		g[i] = col.g;
		b[i] = col.b;
		//
		rFloat[i] = ofMap(r[i], 0, 255, 0.0, 1.0);
		gFloat[i] = ofMap(g[i], 0, 255, 0.0, 1.0);
		bFloat[i] = ofMap(b[i], 0, 255, 0.0, 1.0);
	}
	void setCol(ofFloatColor col, int i) {
		rFloat[i] = col.r;
		gFloat[i] = col.g;
		bFloat[i] = col.b;
		//
		r[i] = ofMap(rFloat[i], 0, 1.0, 0.0, 255);
		g[i] = ofMap(gFloat[i], 0, 1.0, 0.0, 255);
		b[i] = ofMap(bFloat[i], 0, 1.0, 0.0, 255);
	}

	void setTimeCode(string tc) {
		timeCode = tc;
		vector<string> parseTimeCode = ofSplitString(tc, ":");
		if(parseTimeCode.size()==3){
		hour = ofToInt(parseTimeCode[0]);//*10000
		minute = ofToInt(parseTimeCode[1]) ;//* 100
		second = ofToInt(parseTimeCode[2]);
		}
	}

	int getHour() {
		return hour;
	}
	int getMinute() {
		return minute;
	}
	int getSeconds() {
		return second;
	}

};

class ambientColor{
		
	public:
		ambientColor();
		~ambientColor();
	
		
		void setup();
		void update();
	
		ofColor getColor(int indx);
		vector<ofColor> getColors();

		int convertTime(int h, int m, int s);
		int searchArray(vector<int>a, int b);
		int startDay;

	private:
		
		float currentMappedTime;


		//colorPalette nextColor;
		//colorPalette previousColor;
		colorPalette currentColor;

		vector<colorPalette>colors;
		

};