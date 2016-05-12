#include "ambientColor.h"

ambientColor::ambientColor() {}
ambientColor::~ambientColor() {}


void ambientColor::setup() {

	vector<ofColor>rgb;
	colorPalette newPalette;

	// we start in the night
	rgb.clear();
	
	rgb.push_back(ofColor( 68,69 ,69 ));
	rgb.push_back(ofColor(181 ,255 ,233 ));
	rgb.push_back(ofColor(197,224 ,216 ));
	rgb.push_back(ofColor(201,201 ,201 ));
	rgb.push_back(ofColor(206,171 ,177 ));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("00:00:00");
	colors.push_back(newPalette);
	//
	rgb.clear();
	rgb.push_back(ofColor(13,31 ,45 ));
	rgb.push_back(ofColor(84,106 ,123 ));
	rgb.push_back(ofColor(158,163 ,176 ));
	rgb.push_back(ofColor(250,225 ,223 ));
	rgb.push_back(ofColor(228, 195,173 ));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("12:00:00");
	colors.push_back(newPalette);
	//
	rgb.clear();
	rgb.push_back(ofColor(229,217 ,242 ));
	rgb.push_back(ofColor(245, 239,255 ));
	rgb.push_back(ofColor(205,193 , 255));
	rgb.push_back(ofColor(165,148 ,249 ));
	rgb.push_back(ofColor(115,113 ,252 ));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("19:00:00");
	colors.push_back(newPalette);
	//
	rgb.clear();
	rgb.push_back(ofColor(56, 63,81 ));
	rgb.push_back(ofColor(221,219 ,241 ));
	rgb.push_back(ofColor(60,79 , 118));
	rgb.push_back(ofColor(209,190 ,176 ));
	rgb.push_back(ofColor(171,159 ,157 ));
	newPalette.setColors(rgb);
	newPalette.setTimeCode("23:50:00");
	colors.push_back(newPalette);



	startDay = ofGetDay();
}

void ambientColor::update() {


	//FINDING THE RANGE ACCORDING TO THE TIME
	int iLow;
	int iHigh;

	vector<int>valuesColorTime;
	for (int i = 0;i < colors.size();i++) {
		valuesColorTime.push_back(convertTime(colors[i].getHour(), colors[i].getMinute(), colors[i].getSeconds()));
		//cout << "time convert" << convertTime(colors[i].getHour(), colors[i].getMinute(), colors[i].getSeconds()) << endl;
	}
	int currentTime = convertTime(ofGetHours(), ofGetMinutes(), ofGetSeconds());
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



ofColor ambientColor::getColor(int indx) {
	return currentColor.getCol(indx);
}

vector<ofColor> ambientColor::getColors() {
	


	return currentColor.getCols();
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