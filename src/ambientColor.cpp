#include "ambientColor.h"

ambientColor::ambientColor() {}
ambientColor::~ambientColor() {}


void ambientColor::setup() {

	vector<ofColor>rgb;
	colorPalette newPalette;

	// we start in the night
	//colour 18
	rgb.clear();
	rgb.push_back(ofColor(84, 26, 66));
	rgb.push_back(ofColor(57, 65, 69));
	rgb.push_back(ofColor(189, 178, 163));
	rgb.push_back(ofColor(185, 31, 101));
	rgb.push_back(ofColor(28, 35, 55));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("00:00:00");
	colors.push_back(newPalette);
	//colour 17
	rgb.clear();
	rgb.push_back(ofColor(33, 60, 112));
	rgb.push_back(ofColor(52, 49, 73));
	rgb.push_back(ofColor(183, 172, 212));
	rgb.push_back(ofColor(237, 228, 63));
	rgb.push_back(ofColor(31, 49, 69));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("00:15:00");
	colors.push_back(newPalette);
	//colour 16
	rgb.clear();
	rgb.push_back(ofColor(67, 67, 110));
	rgb.push_back(ofColor(36, 39, 36));
	rgb.push_back(ofColor(215, 222, 219));
	rgb.push_back(ofColor(105, 38, 87));
	rgb.push_back(ofColor(31, 44, 75));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("00:15:00");
	colors.push_back(newPalette);
	//colour 15
	rgb.clear();
	rgb.push_back(ofColor(74, 89, 108));
	rgb.push_back(ofColor(71, 62, 71));
	rgb.push_back(ofColor(201, 182, 209));
	rgb.push_back(ofColor(217, 214, 97));
	rgb.push_back(ofColor(65, 36, 38));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("00:45:00");
	colors.push_back(newPalette);
	//colour 14
	rgb.clear();
	rgb.push_back(ofColor(110, 89, 166));
	rgb.push_back(ofColor(46, 50, 134));
	rgb.push_back(ofColor(252, 229, 217));
	rgb.push_back(ofColor(68, 115, 185));
	rgb.push_back(ofColor(41, 42, 87));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("01:15:00");
	colors.push_back(newPalette);
	//colour 13
	rgb.clear();
	rgb.push_back(ofColor(18, 93, 137));
	rgb.push_back(ofColor(46, 78, 91));
	rgb.push_back(ofColor(216, 232, 240));
	rgb.push_back(ofColor(38, 170, 204));
	rgb.push_back(ofColor(71, 14, 62));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("02:00:00");
	colors.push_back(newPalette);
	//colour 12
	rgb.clear();
	rgb.push_back(ofColor(119, 23, 26));
	rgb.push_back(ofColor(26, 68, 98));
	rgb.push_back(ofColor(226, 205, 206));
	rgb.push_back(ofColor(76, 148, 160));
	rgb.push_back(ofColor(8, 42, 53));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("03:15:00");
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
	newPalette.setTimeCode("15:00:00");
	colors.push_back(newPalette);
	//colour 3
	rgb.clear();
	rgb.push_back(ofColor(196, 209, 65));
	rgb.push_back(ofColor(227, 193, 210));
	rgb.push_back(ofColor(210, 220, 216));
	rgb.push_back(ofColor(210, 81, 152));
	rgb.push_back(ofColor(51, 52, 42));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("17:00:00");
	colors.push_back(newPalette);
	//colour 4
	rgb.clear();
	rgb.push_back(ofColor(242, 231, 137));
	rgb.push_back(ofColor(74, 148, 125));
	rgb.push_back(ofColor(226, 210, 221));
	rgb.push_back(ofColor(178, 191, 53));
	rgb.push_back(ofColor(8, 42, 53));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("18:00:00");
	colors.push_back(newPalette);
	//colour 5
	rgb.clear();
	rgb.push_back(ofColor(249, 184, 39));
	rgb.push_back(ofColor(68, 105, 115));
	rgb.push_back(ofColor(248, 222, 190));
	rgb.push_back(ofColor(217, 87, 75));
	rgb.push_back(ofColor(60, 47, 83));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("18:30:00");
	colors.push_back(newPalette);
	//colour 6
	rgb.clear();
	rgb.push_back(ofColor(119, 16, 43));
	rgb.push_back(ofColor(1, 37, 63));
	rgb.push_back(ofColor(207, 235, 238));
	rgb.push_back(ofColor(28, 66, 47));
	rgb.push_back(ofColor(91, 77, 78));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("19:30:00");
	colors.push_back(newPalette);
	//colour 7
	rgb.clear();
	rgb.push_back(ofColor(61, 29, 62));
	rgb.push_back(ofColor(12, 46, 48));
	rgb.push_back(ofColor(108, 199, 182));
	rgb.push_back(ofColor(117, 192, 82));
	rgb.push_back(ofColor(48, 33, 46));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("19:45:00");
	colors.push_back(newPalette);
	//colour 8
	rgb.clear();
	rgb.push_back(ofColor(87, 55, 110));
	rgb.push_back(ofColor(39, 41, 103));
	rgb.push_back(ofColor(89, 181, 230));
	rgb.push_back(ofColor(195, 75, 110));
	rgb.push_back(ofColor(14, 22, 49));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("20:00:00");
	colors.push_back(newPalette);
	//colour 9
	rgb.clear();
	rgb.push_back(ofColor(228, 231, 150));
	rgb.push_back(ofColor(159, 167, 159));
	rgb.push_back(ofColor(233, 178, 151));
	rgb.push_back(ofColor(201, 102, 93));
	rgb.push_back(ofColor(45, 47, 43));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("20:30:00");
	colors.push_back(newPalette);
	//colour 10
	rgb.clear();
	rgb.push_back(ofColor(153, 75, 85));
	rgb.push_back(ofColor(22, 47, 48));
	rgb.push_back(ofColor(254, 215, 174));
	rgb.push_back(ofColor(219, 55, 57));
	rgb.push_back(ofColor(45, 16, 18));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("20:45:00");
	colors.push_back(newPalette);
	//colour 11
	rgb.clear();
	rgb.push_back(ofColor(128, 77, 73));
	rgb.push_back(ofColor(73, 80, 76));
	rgb.push_back(ofColor(179, 187, 177));
	rgb.push_back(ofColor(215, 129, 107));
	rgb.push_back(ofColor(6, 35, 65));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("21:00:00");
	colors.push_back(newPalette);
	//colour 12
	rgb.clear();
	rgb.push_back(ofColor(119, 23, 26));
	rgb.push_back(ofColor(26, 68, 98));
	rgb.push_back(ofColor(226, 205, 206));
	rgb.push_back(ofColor(76, 148, 160));
	rgb.push_back(ofColor(8, 42, 53));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("21:15:00");
	colors.push_back(newPalette);
	//colour 13
	rgb.clear();
	rgb.push_back(ofColor(18, 93, 137));
	rgb.push_back(ofColor(46, 78, 91));
	rgb.push_back(ofColor(216, 232, 240));
	rgb.push_back(ofColor(38, 170, 204));
	rgb.push_back(ofColor(71, 14, 62));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("21:40:00");
	colors.push_back(newPalette);
	//colour 14
	rgb.clear();
	rgb.push_back(ofColor(110, 89, 166));
	rgb.push_back(ofColor(46, 50, 134));
	rgb.push_back(ofColor(252, 229, 217));
	rgb.push_back(ofColor(68, 115, 185));
	rgb.push_back(ofColor(41, 42, 87));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("22:00:00");
	colors.push_back(newPalette);
	//colour 15
	rgb.clear();
	rgb.push_back(ofColor(74, 89, 108));
	rgb.push_back(ofColor(71, 62, 71));
	rgb.push_back(ofColor(201, 182, 209));
	rgb.push_back(ofColor(217, 214, 97));
	rgb.push_back(ofColor(65, 36, 38));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("22:30:00");
	colors.push_back(newPalette);
	//colour 16
	rgb.clear();
	rgb.push_back(ofColor(67, 67, 110));
	rgb.push_back(ofColor(36, 39, 36));
	rgb.push_back(ofColor(215, 222, 219));
	rgb.push_back(ofColor(105, 38, 87));
	rgb.push_back(ofColor(31, 44, 75));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("23:00:00");
	colors.push_back(newPalette);
	//colour 17
	rgb.clear();
	rgb.push_back(ofColor(33, 60, 112));
	rgb.push_back(ofColor(52, 49, 73));
	rgb.push_back(ofColor(183, 172, 212));
	rgb.push_back(ofColor(237, 228, 63));
	rgb.push_back(ofColor(31, 49, 69));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("23:30:00");
	colors.push_back(newPalette);
	//colour 18
	rgb.clear();
	rgb.push_back(ofColor(84, 26, 66));
	rgb.push_back(ofColor(57, 65, 69));
	rgb.push_back(ofColor(189, 178, 163));
	rgb.push_back(ofColor(185, 31, 101));
	rgb.push_back(ofColor(28, 35, 55));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("23:59:59");
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
	//cout << "currentTime converted" << currentTime<<endl;
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