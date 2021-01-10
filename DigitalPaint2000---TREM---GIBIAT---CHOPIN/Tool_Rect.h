/*
	Tool_Rect.h
	Implements the Rect tool
*/

#pragma once

void swap(int& a, int& b)
{
	a, b = b, a;
}


/*
	Handles mouse press events passed onto the Rect tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool Tool_Rect::Pressed(int button, int state, int x, int y) {
	if (currentCanvas.checkInside(x, y)) {
		// convert mouse position into canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the start mouse position if this is start of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			return true;
		}
		// draw the rect if this is the end of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP) && isMouseDown) {
			if ((startMouseX == cx) && (startMouseY == cy)) {
				// if the mouse hasn't moved, just colour one pixel
				currentCanvas.SetPixelColour(cx, cy, selectedColour);
			}
			else
			{
				// get the rect coordinates
				float minX = startMouseX;
				float maxX = cx;
				float minY = startMouseY;
				float maxY = cy;
				float difX = abs(minX - maxX);
				float difY = abs(minY - maxY);
				float y;
				float x;
				float CoefD = ((maxY - minY) / (maxX - minX));
				float b = minY - CoefD * minX;

				if (difX > difY) 
				{
					for (x = std::min(cx, startMouseX); x <= std::max(cx, startMouseX); x++) 
					{
						y = CoefD * x + b;
						currentCanvas.SetPixelColour(x, y, selectedColour);
					}
				}
				else 
				{
					for (y = std::min(cy, startMouseY); y <= std::max(cy, startMouseY); y++) 
					{
						if (difX == 0)
						{
							x = minX;
						}
						else
						{
							x = (y - b) / CoefD;
						}
						currentCanvas.SetPixelColour(x, y, selectedColour);
					}
				}
				
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
bool Tool_Rect::BlockMousePress(int button, int state, int x, int y) {
	// if during a drag, this tool should take the mouse events first
	if (isMouseDown) {
		if (currentCanvas.checkInside(x, y)) {
			return true;
		}
	}
	isMouseDown = false;
	return false;
}