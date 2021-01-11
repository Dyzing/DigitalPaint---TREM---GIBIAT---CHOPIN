/*
	Tool_Selection.h
	Implements the Circle drawing tool
*/

#pragma once


/*
	Handles mouse press events passed onto the Circ tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
	@return Has the tool handled the event?
*/
Colour selection = { 0.4f, 0.0f, 0.80f };
Colour blanc = { 1.0f, 1.0f, 1.0f };
bool operator==(const Colour col1, const Colour col2) {
	return (col1.b == col2.b && col1.r == col2.r && col1.g == col2.g);
}
bool Tool_Selection::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// convert mouse position into canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the start mouse position if this is start of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			currentCanvas.SetPixelColour(cx, cy, selection);
			return true;
		}
		// draw the circle if this is the end of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			if ((startMouseX == cx) && (startMouseY == cy)) {
				// if the mouse hasn't moved, just colour one pixel
				currentCanvas.SetPixelColour(cx, cy, selection);
			}
			else {
				// get the rect coordinates to put the circle in
				int minX = std::min(cx, startMouseX);
				int maxX = std::max(cx, startMouseX);
				int minY = std::min(cy, startMouseY);
				int maxY = std::max(cy, startMouseY);
				for (int x = 0; x < currentCanvas.width; x++) {

					for (int y = 0; y < currentCanvas.height; y++) {
						if (!(x > minX && x < maxX && y > minY && y < maxY) || currentCanvas.GetPixelColour(x, y) == selection) {
							currentCanvas.SetPixelColour(x, y, blanc);

						}
					}
				}
				for (int x = minX; x <= maxX; x++) {
					currentCanvas.SetPixelColour(x, minY, selection);
				}
				for (int x = minX; x <= maxX; x++) {
					currentCanvas.SetPixelColour(x, maxY, selection);
				}
				for (int y = minY; y <= maxY; y++) {
					currentCanvas.SetPixelColour(minX,y, selection);
				}
				for (int y = minY; y <= maxY; y++) {
					currentCanvas.SetPixelColour(maxX, y, selection);
				}

				
				
				// go through each pixel in the bounding rect, colour it if it lies within ellipse
				
			}
			isMouseDown = false;
			return true;
		}
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
bool Tool_Selection::BlockMousePress(int button, int state, int x, int y) {
	// if during a drag, this tool should take the mouse events first
	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}
