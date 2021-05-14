#include "Toolbar.h"
/*
	Tool_Rect.h
	Implements the Rect tool
*/

#pragma once

/*
	Handles mouse press events passed onto the Rect tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/

bool isEnded = false;
bool Tool_Polygone::Pressed(int button, int state, int x, int y) {
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
			firstPick = false;
			return true;
		}
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown ) {
			
				// get the rect coordinates
				float minX = startMouseX;
				float maxX = cx;
				float minY = startMouseY;
				float maxY = cy;
				float difX = abs(minX - maxX);
				float difY = abs(minY - maxY);
				float CoefD = ((maxY - minY) / (maxX - minX));
				float b = minY - CoefD * minX;
				currentCanvas.DrawALine(startMouseX, startMouseY, cx, cy, selectedColour);

				
				ListeCotes.push_back({ startMouseX,startMouseY,cx,cy,CoefD, b });
				ListeSommets.push_back({ cx, cy });
				storeEdgeInTable(startMouseX, startMouseY, cx, cy);

			}
			
			startMouseX = cx;
			startMouseY = cy;
			isMouseDown = false;
			return true;
		}
	return false;
}


/*
	Should this tool take priority for receiving mouse events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Should this tool take priority for receiving mouse events
*/
bool Tool_Polygone::BlockMousePress(int button, int state, int x, int y) {
	// if during a drag, this tool should take the mouse events first
	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}

void Tool_Polygone::EndPolygon() {
	bord_color = selectedColour;
	float minX = departX;
	float maxX = startMouseX;
	float minY = departY;
	float maxY = startMouseY;

	float difX = abs(minX - maxX);
	float difY = abs(minY - maxY);
	float CoefD = ((maxY - minY) / (maxX - minX));
	float b = minY - CoefD * minX;
	currentCanvas.DrawALine(startMouseX, startMouseY, departX, departY, selectedColour);
	ListeCotes.push_back({ departX,departY,startMouseX,startMouseY,CoefD });
	MultiSommets.push_back(ListeSommets);
	storeEdgeInTable(departX, departY, startMouseX, startMouseY);
	firstPick = true;
}

