/*
	Tool_Fill.h
	Implements the Fill tool
*/

#pragma once

#include <vector>
#include <list>

/*
	Fill algorithm from a position

	@param startColour - The colour we should be replacing
	@param cx - x coordinate of pixel to fill from
	@param cy - y coordinate of pixel to fill from
*/
struct Intersection {
	int x;
	int ymax;
	int coefD;
	int y;
};
void affichage() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	if (canvasAssigned) {
		currentCanvas.Draw(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		ToolEvents::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}


	glPopMatrix();
	glutSwapBuffers();
}
void CreateSI(std::list<Tuple> cote) {
	const int y1 = Tool_Selection::posFinY;
	const int y2 = Tool_Selection::posDepY;
	std::list<std::list<Intersection>> SI;
	for (int y = Tool_Selection::posDepY + 1; y < Tool_Selection::posFinY; y++) {
		for (int x = Tool_Selection::posDepX + 1; x < Tool_Selection::posFinX; x++) {
			for (Tuple tp : cote) {
				cote.size();
			}
		}
	}

}
std::list<Intersection> SI[4];

Tuple c = { 0,0 };
void Tool_Fill::Fill(Colour startColour, int cx, int cy) {
	std::list<Tuple> LCA = {};
	Tool_Polygone::ListeCotes;
	//CreateSI(Tool_Polygone::ListeCotes);
	LCA.push_back(c);
	//Tuple LCA[2] = { {0,0},{0,0} };
	for (int y = Tool_Selection::posDepY+1; y < Tool_Selection::posFinY; y++) {
		for (int x = Tool_Selection::posDepX+1; x < Tool_Selection::posFinX; x++) {
			if (currentCanvas.GetPixelColour(x, y) != startColour) {
				Tuple tmp = { x,y };
				LCA.push_back(tmp);
				
			}
		}
		while (LCA.size() != 0 and LCA.size() != 1) {
			Tuple t1 = LCA.front();
			LCA.pop_front();
			Tuple t2 = LCA.front();
			LCA.pop_front();
			for (int x2 = t1.x; x2 < t2.x; x2++) {
					currentCanvas.SetPixelColour(x2, t2.y, selectedColour);
				
				}
				
		}
		affichage();
		LCA.clear();
	}
}

/*void Tool_Fill::Fill(Colour startColour, int cx, int cy) {
	// algorithm expands from point filling an area
	// vec stores outer pixels for the next iteration
	std::vector<Tuple> vec;
	std::vector<Tuple> newvec;
	Tuple startCoord = { cx, cy };
	vec.push_back(startCoord);
	while (vec.size() > 0) {
		for (int i = 0; i < vec.size(); i++) {
			// change pixel colour for pixels on outside of the fill area
			Colour colourAtPixel = currentCanvas.GetPixelColour(vec[i].x, vec[i].y);
			if ((colourAtPixel.r == startColour.r) && (colourAtPixel.g == startColour.g) && (colourAtPixel.b == startColour.b)) {
				currentCanvas.SetPixelColour(vec[i].x, vec[i].y, selectedColour);
				if (i % 10 == 0) {
					affichage();
				}
				// add neighbours to the outside vector for the next iteration
				if (vec[i].x < currentCanvas.width - 1) {
					Tuple newCoord = { vec[i].x + 1, vec[i].y };
					newvec.push_back(newCoord);
				}
				if (vec[i].x > 0) {
					Tuple newCoord = { vec[i].x - 1, vec[i].y };
					newvec.push_back(newCoord);
				}
				if (vec[i].y < currentCanvas.height - 1) {
					Tuple newCoord = { vec[i].x, vec[i].y + 1 };
					newvec.push_back(newCoord);
				}
				if (vec[i].y > 0) {
					Tuple newCoord = { vec[i].x, vec[i].y - 1 };
					newvec.push_back(newCoord);
				}
			}

		}
		vec.empty();
		vec = newvec;
		std::vector<Tuple> b;
		newvec = b;
	}
} 
*/

/*
	Handles mouse press events passed onto the Fill tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Fill::Pressed(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		int canvasCoordX = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int canvasCoordY = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		Colour colourWherePressed = currentCanvas.GetPixelColour(canvasCoordX, canvasCoordY);
		if ((colourWherePressed.r != selectedColour.r) || (colourWherePressed.g != selectedColour.g) || (colourWherePressed.b != selectedColour.b)) {
			Tool_Fill::Fill(colourWherePressed, canvasCoordX, canvasCoordY);
			return true;
		}
	}
	return false;
}