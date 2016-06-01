#include "ambientColor.h"

ambientColor::ambientColor() {}
ambientColor::~ambientColor() {}


void ambientColor::setup() {

	vector<ofColor>rgb;
	colorPalette newPalette;

	// we start in the night
	//colour 8
	rgb.clear();
	rgb.push_back(ofColor(87, 55, 110));
	rgb.push_back(ofColor(39, 41, 103));
	rgb.push_back(ofColor(89, 181, 230));
	rgb.push_back(ofColor(195, 75, 110));
	rgb.push_back(ofColor(14, 22, 49));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("00:00:00");
	colors.push_back(newPalette);
	//colour 1
	rgb.clear();
	rgb.push_back(ofColor(234, 235, 218));
	rgb.push_back(ofColor(229, 201, 194));
	rgb.push_back(ofColor(248, 239, 229));
	rgb.push_back(ofColor(175, 116, 104));
	rgb.push_back(ofColor(80, 73, 79));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("10:00:00");
	colors.push_back(newPalette);
	//colour 2
	rgb.clear();
	rgb.push_back(ofColor(167, 214, 201));
	rgb.push_back(ofColor(191, 185, 215));
	rgb.push_back(ofColor(239, 238, 247));
	rgb.push_back(ofColor(142, 53, 128));
	rgb.push_back(ofColor(108, 129, 151));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("12:00:00");
	colors.push_back(newPalette);
	//colour 3
	rgb.clear();
	rgb.push_back(ofColor(196, 209, 65));
	rgb.push_back(ofColor(227, 193, 210));
	rgb.push_back(ofColor(210, 220, 216));
	rgb.push_back(ofColor(210, 81, 152));
	rgb.push_back(ofColor(51, 52, 42));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("14:00:00");
	colors.push_back(newPalette);
	//colour 4
	rgb.clear();
	rgb.push_back(ofColor(242, 231, 137));
	rgb.push_back(ofColor(74, 148, 125));
	rgb.push_back(ofColor(226, 210, 221));
	rgb.push_back(ofColor(178, 191, 53));
	rgb.push_back(ofColor(8, 42, 53));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("16:00:00");
	colors.push_back(newPalette);
	//colour 5
	rgb.clear();
	rgb.push_back(ofColor(249, 184, 39));
	rgb.push_back(ofColor(68, 105, 115));
	rgb.push_back(ofColor(248, 222, 190));
	rgb.push_back(ofColor(217, 87, 75));
	rgb.push_back(ofColor(60, 47, 83));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("18:00:00");
	colors.push_back(newPalette);
	//colour 6
	rgb.clear();
	rgb.push_back(ofColor(119, 16, 43));
	rgb.push_back(ofColor(1, 37, 63));
	rgb.push_back(ofColor(207, 235, 238));
	rgb.push_back(ofColor(28, 66, 47));
	rgb.push_back(ofColor(91, 77, 78));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("20:00:00");
	colors.push_back(newPalette);
	//colour 7
	rgb.clear();
	rgb.push_back(ofColor(61, 29, 62));
	rgb.push_back(ofColor(12, 46, 48));
	rgb.push_back(ofColor(108, 199, 182));
	rgb.push_back(ofColor(117, 192, 82));
	rgb.push_back(ofColor(48, 33, 46));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("22:00:00");
	colors.push_back(newPalette);




	startDay = ofGetDay();
}

void ambientColor::update(float simulatedTime, bool useSimulateTime) {


	//FINDING THE RANGE ACCORDING TO THE TIME
	int iLow;
	int iHigh;

	vector<int>valuesColorTime;
	for (int i = 0;i < colors.size();i++) {
		valuesColorTime.push_back(convertTime(colors[i].getHour(), colors[i].getMinute(), colors[i].getSeconds()));
		//cout << "time convert" << convertTime(colors[i].getHour(), colors[i].getMinute(), colors[i].getSeconds()) << endl;
	}
	int currentTime = convertTime(ofGetHours(), ofGetMinutes(), ofGetSeconds());
	
	if (useSimulateTime)currentTime = simulatedTime;
	
	int closestIndex = searchArray(valuesColorTime, currentTime);
	//cout << "currentTime" << currentTime<<endl;
	//cout << "closestIndex" << closestIndex << endl;
	if (valuesColorTime[closestIndex] > currentTime) {
		iHigh = closestIndex;
		iLow = closestIndex - 1;
		if (iLow < 0) { 
	
			iLow = colors.size() - 1; 

			//valuesColorTime[iLow] -= valuesColorTime[iHigh];
		}
	}
	else if (valuesColorTime[closestIndex] < currentTime) {
		iLow = closestIndex;
		iHigh = closestIndex + 1;
		if (iHigh > colors.size() - 1) { 
			iHigh = 0; 
			//valuesColorTime[iHigh] += valuesColorTime[iLow];
		}
	}
	else if (valuesColorTime[closestIndex] == currentTime) {
		currentColor = colors[closestIndex];
		return;
	}
	
	//cout << "iLow" << iLow << endl;
	//cout << "iHigh" << iHigh << endl;

	//cout << "val iLow" << valuesColorTime[iLow] << endl;
	//cout << "val iHigh" << valuesColorTime[iHigh] << endl;
	//previousColor= colors[iLow];
	//nextColor = colors[iHigh];

	float tNorm = ofMap(currentTime, valuesColorTime[iLow], valuesColorTime[iHigh], 0.0, 1.0);
	if (valuesColorTime[iLow] > valuesColorTime[iHigh]) {
		tNorm = ofMap(currentTime, valuesColorTime[iHigh], valuesColorTime[iLow], 0.0, 1.0);
	}

	//cout << "tNorm" << tNorm;
	for (int i = 0;i < NUM_COLORS_IN_PALETTE;i++) {
		ofColor c1=colors[iLow].getCol(i);
		ofColor c2 = colors[iHigh].getCol(i);;

		//cout << "c1" << c1<<endl;;
		currentColor.setCol(ofColor(ofLerp(c1.r, c2.r, tNorm), ofLerp(c1.g, c2.g, tNorm), ofLerp(c1.b, c2.b, tNorm)), i);
	}

}
void ambientColor::setColor(ofColor colIn,int colIndex) {
	colors[colIndex].setCol(colIn, colIndex);
}


ofColor ambientColor::getColor(int indx) {
	return currentColor.getCol(indx);
}

vector<ofColor> ambientColor::getColors() {
	return currentColor.getCols();
}


vector<ofFloatColor> ambientColor::getFloatColors() {
	return currentColor.getFloatCols();
}


int ambientColor::convertTime(int h, int m, int s) {

	int hours=ofMap(h, 0, 24, 0, 100000);
	int minutes = ofMap(m, 0, 60, 0, 1000);
	int seconds = ofMap(s, 0, 60, 0, 100);

	if (ofGetDay() != startDay)hours += 100000;//WARNING THIS WORK ONLY FOR ONE DAY ON

	return hours + minutes + seconds;

}


int ambientColor::searchArray(vector<int>a, int b) {


	//find nearest element to key            
	//int refElem = b;
	//std::vector<int> v{ 1, 5, 36, 50 };
	auto i = min_element(begin(a), end(a), [=](int x, int y)
	{
		return abs(x - b) < abs(y - b);
	});

	//cout <<"nearestValue"<< distance(begin(a), i)<<endl; 
	return  distance(begin(a), i);
	// Prints 2
	/*
	int min_dist = 0; // keep track of mininum distance seen so far
	int min_index = 0; // keep track of the index of the element 
					   // that has the min index

	for (int i = 0; i < a.size(); i++) { // a.Length is the size of the array

		cout << a[i] << "firstkey" << b << endl;
		if (a[i] == b) { // if we find the exact one, stop
			return i;
		}
		else { // otherwise, keep looking for the closest
			if (abs(a[i] - b) < min_dist) { // if this one is closer, update
				min_dist = abs(a[i] - b);
				min_index = i;

				cout << a[i] << "key" << b << endl;
			}
		}
	}
	return min_index; // when we finish return the index of the closest
	*/
}