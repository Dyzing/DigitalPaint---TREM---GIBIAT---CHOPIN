/*
	Tool_Selection.h
	Implements the Circle drawing tool
*/

#pragma once




struct newSommets {
	int** tabSommets;
	int size;
};
Colour selection = { 0.4f, 0.0f, 0.80f };
/*
	Handles mouse press events passed onto the Circ tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
	@return Has the tool handled the event?
*/
bool Tool_Selection::Pressed(int button, int state, int x, int y) {

	if (currentCanvas.checkInside(x, y)) {
		// convert mouse position into canvas coordinates
		int cx = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int cy = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		// remember the start mouse position if this is start of a drag
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown && firstPickSelect) {
			isMouseDown = true;
			startMouseX = cx;
			startMouseY = cy;
			departX = cx;
			departY = cy;
			firstPickSelect = false;
			return true;
		}
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN) && !isMouseDown) {

			// get the rect coordinates
			float minX = startMouseX;
			float maxX = cx;
			float minY = startMouseY;
			float maxY = cy;
			currentCanvas.DrawALine(startMouseX, startMouseY, cx, cy, selection);


			CotesFenetre.push_back({ startMouseX,startMouseY });
		}

		startMouseX = cx;
		startMouseY = cy;
		isMouseDown = false;
		return true;
	}
	return false;
}



/*bool Tool_Selection::Pressed(int button, int state, int x, int y) {
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
					currentCanvas.SetPixelColour(x, maxY, selection);
				}
				for (int y = minY; y <= maxY; y++) {
					currentCanvas.SetPixelColour(minX,y, selection);
					currentCanvas.SetPixelColour(maxX, y, selection);
				}
				posDepX = minX;
				posDepY = minY;
				posFinX = maxX;
				posFinY = maxY;
				
				
				// go through each pixel in the bounding rect, colour it if it lies within ellipse
				
			}
			isMouseDown = false;
			return true;
		}
	}
	return false;
}*/


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


const int MAX_POINTS = 50;

// Returns x-value of point of intersectipn of two 
// lines 
int x_intersect(int x1, int y1, int x2, int y2,
    int x3, int y3, int x4, int y4)
{
    int num = (x1 * y2 - y1 * x2) * (x3 - x4) -
        (x1 - x2) * (x3 * y4 - y3 * x4);
    int den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    return num / den;
}

// Returns y-value of point of intersectipn of 
// two lines 
int y_intersect(int x1, int y1, int x2, int y2,
    int x3, int y3, int x4, int y4)
{
    int num = (x1 * y2 - y1 * x2) * (y3 - y4) -
        (y1 - y2) * (x3 * y4 - y3 * x4);
    int den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    return num / den;
}

// This functions clips all the edges w.r.t one clip 
// edge of clipping area 
void clip(int** poly_points, int& poly_size,
    int x1, int y1, int x2, int y2)
{
    int new_points[MAX_POINTS][2], new_poly_size = 0;

    // (ix,iy),(kx,ky) are the co-ordinate values of 
    // the points 
    for (int i = 0; i < poly_size; i++)
    {
        // i and k form a line in polygon 
        int k = (i + 1) % poly_size;
        int ix = poly_points[i][0], iy = poly_points[i][1];
        int kx = poly_points[k][0], ky = poly_points[k][1];

        // Calculating position of first point 
        // w.r.t. clipper line 
        int i_pos = (x2 - x1) * (iy - y1) - (y2 - y1) * (ix - x1);

        // Calculating position of second point 
        // w.r.t. clipper line 
        int k_pos = (x2 - x1) * (ky - y1) - (y2 - y1) * (kx - x1);

        // Case 1 : When both points are inside 
        if (i_pos < 0 && k_pos < 0)
        {
            //Only second point is added 
            new_points[new_poly_size][0] = kx;
            new_points[new_poly_size][1] = ky;
            new_poly_size++;
        }

        // Case 2: When only first point is outside 
        else if (i_pos >= 0 && k_pos < 0)
        {
            // Point of intersection with edge 
            // and the second point is added 
            new_points[new_poly_size][0] = x_intersect(x1,
                y1, x2, y2, ix, iy, kx, ky);
            new_points[new_poly_size][1] = y_intersect(x1,
                y1, x2, y2, ix, iy, kx, ky);
            new_poly_size++;

            new_points[new_poly_size][0] = kx;
            new_points[new_poly_size][1] = ky;
            new_poly_size++;
        }

        // Case 3: When only second point is outside 
        else if (i_pos < 0 && k_pos >= 0)
        {
            //Only point of intersection with edge is added 
            new_points[new_poly_size][0] = x_intersect(x1,
                y1, x2, y2, ix, iy, kx, ky);
            new_points[new_poly_size][1] = y_intersect(x1,
                y1, x2, y2, ix, iy, kx, ky);
            new_poly_size++;
        }

        // Case 4: When both points are outside 
        else
        {
            //No points are added 
        }
    }

    // Copying new points into original array 
    // and changing the no. of vertices 
	
	poly_size = new_poly_size;

	for (int i = 0; i < poly_size; i++)
		{
			poly_points[i][0] = new_points[i][0];
			poly_points[i][1] = new_points[i][1];
		}
}

// Implements Sutherland–Hodgman algorithm 
int suthHodgClip(int** poly_points, int poly_size,
    int** clipper_points, int clipper_size)
{
	int j = 0;
    //i and k are two consecutive indexes 
    for (int i = 0; i < clipper_size; i++)
    {
        int k = (i + 1) % clipper_size;

        // We pass the current array of vertices, it's size 
        // and the end points of the selected clipper line 
        clip(poly_points, poly_size, clipper_points[i][0],
            clipper_points[i][1], clipper_points[k][0],
            clipper_points[k][1]);
    }

    // Printing vertices of clipped polygon 
	
	for (j; j < poly_size - 1; j++) {
		//std::cout << '(' << poly_points[j][0] << ", " << poly_points[j][1] << ") " << "Draw with "<< '(' << poly_points[j+1][0] << ", " << poly_points[j+1][1] << ") \n ";
		currentCanvas.DrawALine(poly_points[j][0], poly_points[j][1], poly_points[j+1][0], poly_points[j+1][1],selectedColour);
	}
	currentCanvas.DrawALine(poly_points[j][0], poly_points[j][1], poly_points[0][0], poly_points[0][1], selectedColour);


	return poly_size;
}


void Tool_Selection::End_Selection() {
	float minX = departX;
	float maxX = startMouseX;
	float minY = departY;
	float maxY = startMouseY;

	float y;
	float x;
	int i = 0;
	currentCanvas.DrawALine(departX, departY, startMouseX, startMouseY, selection);
	CotesFenetre.push_back({ startMouseX,startMouseY });
	int sizeFenetre = CotesFenetre.size();
	int** tab = new int* [sizeFenetre];
	int* tab2 = new int[sizeFenetre * 2];

	for (int i = 0; i < sizeFenetre; i++) {
		tab[i] = &tab2[i * 2];

		tab[i][0] = CotesFenetre.front().x;
		tab[i][1] = CotesFenetre.front().y;
		CotesFenetre.pop_front();
	}
	currentCanvas = NewCanvas(500, 500, 100, 100);
	std::list<newSommets> ListeNewPolygone;
	for (std::list<Tuple> sommets : Tool_Polygone::MultiSommets) {
		int sizePolygon = sommets.size();
		int** tabPoly = new int* [sizePolygon * 2];
		int* tabPoly2 = new int[sizePolygon * 4];
		for (int i = 0; i < sizePolygon * 2; i++) {
			tabPoly[i] = &tabPoly2[i * 2];
		}
		for (int i = 0; i < sizePolygon; i++) {

			tabPoly[i][0] = sommets.front().x;
			tabPoly[i][1] = sommets.front().y;
			sommets.pop_front();
		}
		
		int size = suthHodgClip(tabPoly, sizePolygon, tab, sizeFenetre);
		if (size > 0) {
			ListeNewPolygone.push_back({ tabPoly, size });
		}
	}
	int ind = 0;
	for (ind; ind < sizeFenetre - 1; ind++) {
		currentCanvas.DrawALine(tab[ind][0], tab[ind][1], tab[ind + 1][0], tab[ind + 1][1], selection);
	}
	currentCanvas.DrawALine(tab[ind][0], tab[ind][1], tab[0][0], tab[0][1], selection);

	Tool_Polygone::MultiSommets.clear();
	std::list<Tuple> tmp;
	for (newSommets som : ListeNewPolygone) {
		for (int b = 0; b < som.size; b++) {
			tmp.push_back({ som.tabSommets[b][0], som.tabSommets[b][1] });
		}
		Tool_Polygone::MultiSommets.push_back(tmp);
		tmp.clear();
	}

	Tool_Fill::initEdgeTable();
	std::list<cotes> test = Tool_Polygone::ListeCotes;
	tmp = Tool_Polygone::MultiSommets.back();
	int** tab1 = new int* [tmp.size()];
	int* tabxy = new int[tmp.size() * 2];
	int size = tmp.size();
	int iter = 0;
	for (int i = 0; i < size; i++) {
		tab1[i] = &tabxy[i * 2];
		tab1[i][0] = tmp.front().x;
		tab1[i][1] = tmp.front().y;
		tmp.pop_front();
	}
	for (iter; iter < size - 1; iter++) {

		storeEdgeInTable(tab1[iter][0], tab1[iter][1], tab1[iter + 1][0], tab1[iter + 1][1]);

	}
	storeEdgeInTable(tab1[iter][0], tab1[iter][1], tab1[0][0], tab1[0][1]);
	firstPickSelect = true;
}