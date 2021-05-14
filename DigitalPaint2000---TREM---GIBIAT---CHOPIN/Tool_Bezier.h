#pragma once


/*
	Handles mouse press events passed onto the Rect tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/

bool Tool_Bezier::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// convert mouse position into canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the start mouse position if this is start of a drag

		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown && firstPick) {
			Tool_Fill::initEdgeTable();
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			departX = cx;
			departY = cy;
			ListeSommets.clear();
			ListeSommets.push_back({ startMouseX, startMouseY });
			currentCanvas.SetPixelColour(cx, cy, selectedColour);
			firstPick = false;
			return true;
		}

		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {

			// get the rect coordinates
			float minX = startMouseX;
			float maxX = cx;
			float minY = startMouseY;
			float maxY = cy;
			float difX = abs(minX - maxX);
			float difY = abs(minY - maxY);
			float CoefD = ((maxY - minY) / (maxX - minX));
			float b = minY - CoefD * minX;
			/*currentCanvas.DrawALine(startMouseX, startMouseY, cx, cy, selectedColour);*/
			currentCanvas.SetPixelColour(cx, cy, selectedColour);


			ListeCotes.push_back({ startMouseX,startMouseY,cx,cy,CoefD, b });
			ListeSommets.push_back({ cx, cy });


		}


		startMouseX = cx;
		startMouseY = cy;
		isMouseDown = false;


		

		if (firstBezier && ListeSommets.size() == 4) {
			Bezier();
			firstBezier = false;
		}else if (!firstBezier && (ListeSommets.size() - 4) % 3 == 0) {
			Bezier();
		}

		return true;
	}
	return false;
}


void Tool_Bezier::Bezier() {

	/*ListeSommetsCurve = {};

	step = 10;

	std::list<std::array<Tuple, 4>> ListeSommetsBezier;

	int i = 0;

	std::vector<Tuple> SommetsBezier{};
	std::vector<Tuple> sommetsTemp{};
	for (auto const& it : ListeSommets) {
		
		SommetsBezier.push_back(it);
	
	}

	Tuple temp;
	ListeSommetsCurve.push_back(SommetsBezier[0]);
	for (int i = 1; i < step; i++) {

		for (int i = 0; i < SommetsBezier.size(); i++) {

			sommetsTemp = SommetsBezier;
			temp = sommetsTemp[0];
			for (int k = 1; k < SommetsBezier.size(); k++) {


				sommetsTemp[0] = Bary(i, step, temp, SommetsBezier[k]);
				temp = sommetsTemp[k];

			}

			SommetsBezier = sommetsTemp;
		}
		ListeSommetsCurve.push_back(SommetsBezier[0]);

	}
	ListeSommetsCurve.push_back(SommetsBezier.back());*/

	ListeSommetsCurve = {};

	step = 10;

	std::list<std::array<Tuple, 4>> ListeSommetsBezier;

	int i = 0;
	 
	std::array<Tuple, 4> SommetsBezier{};
	for (auto const& it : ListeSommets) {
		

		SommetsBezier[i] = it;

		if (i == 3) {
			ListeSommetsBezier.push_back(SommetsBezier);
			SommetsBezier[0] = SommetsBezier[3];
			//SommetsBezier[1] = SommetsBezier[3];
			i = 0;
		}

		i++;
		
	}




	for (auto const& it : ListeSommetsBezier) {

		SommetsBezier = it;

		ListeSommetsCurve.push_back(SommetsBezier[0]);

		for (int i = 1; i < step; i++) {

			Tuple a1 = Bary(i, step, SommetsBezier[0], SommetsBezier[1]);
			Tuple a2 = Bary(i, step, SommetsBezier[1], SommetsBezier[2]);
			Tuple a3 = Bary(i, step, SommetsBezier[2], SommetsBezier[3]);

			Tuple a4 = Bary(i, step, a1, a2);
			Tuple a5 = Bary(i, step, a2, a3);
			Tuple a6 = Bary(i, step, a4, a5);
			ListeSommetsCurve.push_back(a6);

		}

	}

	drawCurve();
}

void Tool_Bezier::BezierEtienne() {

	ListeSommetsCurve = {};

	nIter = 10;

	std::list<std::array<Tuple, 4>> ListeSommetsBezier;

	std::array<Tuple, 4> SommetsBezier{};
	int i = 0;
	for (auto const& it : ListeSommets) {
		
		SommetsBezier[i] = it;
		i++;

	}

	ListeSommetsBezier.push_back(SommetsBezier);

	i = 0;
	std::list<std::array<Tuple, 4>>::iterator it = ListeSommetsBezier.begin();
	while(it != ListeSommetsBezier.end()){
		SommetsBezier = *it;
	
		Tuple a1 = Mid(SommetsBezier[0], SommetsBezier[1]);
		Tuple a2 = Mid(SommetsBezier[1], SommetsBezier[2]);
		Tuple a3 = Mid(SommetsBezier[2], SommetsBezier[3]);

		Tuple a4 = Mid(a1, a2);
		Tuple a5 = Mid(a2, a3);

		currentCanvas.DrawALine(a1.x, a1.y, a4.x, a4.y, selectedColour);
		currentCanvas.DrawALine(a4.x, a4.y, a5.x, a5.y, selectedColour);
		currentCanvas.DrawALine(a5.x, a5.y, a3.x, a3.y, selectedColour);
		


		if (i < nIter) {
			ListeSommetsBezier.push_back({ SommetsBezier[0], a1, a4, a5 });
			ListeSommetsBezier.push_back({ a4, a5, a3, SommetsBezier[3] });
		}
		

		std::list<std::array<Tuple, 4>>::iterator pos = ListeSommetsBezier.erase(it);
		it = pos;
		i++;

	}
	
}

void Tool_Bezier::drawCurve() {

	Tuple temp;
	int i = 0;
	for (auto const& it : ListeSommetsCurve) {

		if (i != 0) {
			currentCanvas.DrawALine(temp.x, temp.y, it.x, it.y, selectedColour);
		}

		temp = it;
		i++;

	}

	
}


Tuple Tool_Bezier::Mid(Tuple a, Tuple b) {

	return { (a.x + b.x) / 2, (a.y + b.y) / 2};
}

Tuple Tool_Bezier::Bary(int iStep, int step,  Tuple a, Tuple b) {
	float iStepf = static_cast<float>(iStep);
	float stepf = static_cast<float>(step);

	return { (a.x + static_cast<int>(((iStepf / stepf) * (b.x - a.x)))), (a.y + static_cast<int>(((iStepf / stepf) * (b.y - a.y))))};
}