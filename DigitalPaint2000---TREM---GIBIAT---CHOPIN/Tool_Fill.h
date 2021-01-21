/*
	Tool_Fill.h
	Implements the Fill tool
*/

#pragma once

#include <vector>
#define maxHt 800 
#define maxWd 600 
#define maxVer 10000 



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


// Start from lower left corner 
typedef struct edgebucket
{
	int ymax;   //max y-coordinate of edge 
	float xofymin;  //x-coordinate of lowest edge point updated only in aet 
	float slopeinverse;
}EdgeBucket;

typedef struct edgetabletup
{
	// the array will give the scanline number 
	// The edge table (ET) with edges entries sorted  
	// in increasing y and x of the lower end 

	int countEdgeBucket;    //no. of edgebuckets 
	EdgeBucket buckets[maxVer];
}EdgeTableTuple;

EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;


// Scanline Function 
void Tool_Fill::initEdgeTable()
{
	int i;
	for (i = 0; i < maxHt; i++)
	{
		EdgeTable[i].countEdgeBucket = 0;
	}

	ActiveEdgeTuple.countEdgeBucket = 0;
}



void printTuple(EdgeTableTuple* tup)
{
	int j;

	if (tup->countEdgeBucket)
		printf("\nCount %d-----\n", tup->countEdgeBucket);

	for (j = 0; j < tup->countEdgeBucket; j++)
	{
		printf(" %d+%.2f+%.2f",
			tup->buckets[j].ymax, tup->buckets[j].xofymin, tup->buckets[j].slopeinverse);
	}
}

void Tool_Fill::printTable()
{
	int i;

	for (i = 0; i < maxHt; i++)
	{
		if (EdgeTable[i].countEdgeBucket)
			printf("\nScanline %d", i);

		printTuple(&EdgeTable[i]);
	}
}


/* Function to sort an array using insertion sort*/
void insertionSort(EdgeTableTuple* ett)
{
	int i, j;
	EdgeBucket temp;

	for (i = 1; i < ett->countEdgeBucket; i++)
	{
		temp.ymax = ett->buckets[i].ymax;
		temp.xofymin = ett->buckets[i].xofymin;
		temp.slopeinverse = ett->buckets[i].slopeinverse;
		j = i - 1;

		while ((temp.xofymin < ett->buckets[j].xofymin) && (j >= 0))
		{
			ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
			ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
			ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
			j = j - 1;
		}
		ett->buckets[j + 1].ymax = temp.ymax;
		ett->buckets[j + 1].xofymin = temp.xofymin;
		ett->buckets[j + 1].slopeinverse = temp.slopeinverse;
	}
}



void storeEdgeInTuple(EdgeTableTuple* receiver, int ym, int xm, float slopInv)
{
	// both used for edgetable and active edge table.. 
	// The edge tuple sorted in increasing ymax and x of the lower end. 
	(receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
	(receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
	(receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

	// sort the buckets 
	insertionSort(receiver);

	(receiver->countEdgeBucket)++;
}


void storeEdgeInTable(int x1, int y1, int x2, int y2)
{
	float m, minv;
	int ymaxTS, xwithyminTS, scanline; //ts stands for to store 

	if (x2 == x1)
	{
		minv = 0.000000;
	}
	else
	{
		m = ((float)(y2 - y1)) / ((float)(x2 - x1));

		// horizontal lines are not stored in edge table 
		if (y2 == y1)
			return;

		minv = (float)1.0 / m;
		printf("\nSlope string for %d %d & %d %d: %f", x1, y1, x2, y2, minv);
	}

	if (y1 > y2)
	{
		scanline = y2;
		ymaxTS = y1;
		xwithyminTS = x2;
	}
	else
	{
		scanline = y1;
		ymaxTS = y2;
		xwithyminTS = x1;
	}
	// the assignment part is done..now storage.. 
	storeEdgeInTuple(&EdgeTable[scanline], ymaxTS, xwithyminTS, minv);


}


void removeEdgeByYmax(EdgeTableTuple* Tup, int yy)
{
	int i, j;
	for (i = 0; i < Tup->countEdgeBucket; i++)
	{
		if (Tup->buckets[i].ymax == yy)
		{
			printf("\nRemoved at %d", yy);

			for (j = i; j < Tup->countEdgeBucket - 1; j++)
			{
				Tup->buckets[j].ymax = Tup->buckets[j + 1].ymax;
				Tup->buckets[j].xofymin = Tup->buckets[j + 1].xofymin;
				Tup->buckets[j].slopeinverse = Tup->buckets[j + 1].slopeinverse;
			}
			Tup->countEdgeBucket--;
			i--;
		}
	}
}


void updatexbyslopeinv(EdgeTableTuple* Tup)
{
	int i;

	for (i = 0; i < Tup->countEdgeBucket; i++)
	{
		(Tup->buckets[i]).xofymin = (Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
	}
}



void Tool_Fill::Fill(Colour startColour)
{
	Tool_Fill::initEdgeTable();
	std::list<Tuple> tmp;

	std::list<std::list<Tuple>>::iterator it = Tool_Polygone::MultiSommets.begin();
	std::advance(it, Tool_Selection::polygone_index % 21);
	tmp = *it;

	//tmp = Tool_Polygone::MultiSommets.back();
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

	/* Follow the following rules:
	1. Horizontal edges: Do not include in edge table
	2. Horizontal edges: Drawn either on the bottom or on the top.
	3. Vertices: If local max or min, then count twice, else count
		once.
	4. Either vertices at local minima or at local maxima are drawn.*/


	int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

	// we will start from scanline 0;  
	// Repeat until last scanline: 
	for (i = 0; i < maxHt; i++)//4. Increment y by 1 (next scan line) 
	{

		// 1. Move from ET bucket y to the 
		// AET those edges whose ymin = y (entering edges) 
		for (j = 0; j < EdgeTable[i].countEdgeBucket; j++)
		{
			storeEdgeInTuple(&ActiveEdgeTuple, 
							EdgeTable[i].buckets[j].ymax, 
							EdgeTable[i].buckets[j].xofymin,
							EdgeTable[i].buckets[j].slopeinverse);
		}
		printTuple(&ActiveEdgeTuple);

		// 2. Remove from AET those edges for  
		// which y=ymax (not involved in next scan line) 
		removeEdgeByYmax(&ActiveEdgeTuple, i);

		//sort AET (remember: ET is presorted) 
		insertionSort(&ActiveEdgeTuple);

		printTuple(&ActiveEdgeTuple);

		//3. Fill lines on scan line y by using pairs of x-coords from AET 
		j = 0;
		FillFlag = 0;
		coordCount = 0;
		x1 = 0;
		x2 = 0;
		ymax1 = 0;
		ymax2 = 0;
		while (j < ActiveEdgeTuple.countEdgeBucket)
		{
			if (coordCount % 2 == 0)
			{
				x1 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
				ymax1 = ActiveEdgeTuple.buckets[j].ymax;
				if (x1 == x2)
				{
					/* three cases can arrive-
						1. lines are towards top of the intersection
						2. lines are towards bottom
						3. one line is towards top and other is towards bottom
					*/
					if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
					{
						x2 = x1;
						ymax2 = ymax1;
					}

					else
					{
						coordCount++;
					}
				}

				else
				{
					coordCount++;
				}
			}
			else
			{
				x2 = (int)ActiveEdgeTuple.buckets[j].xofymin;
				ymax2 = ActiveEdgeTuple.buckets[j].ymax;

				FillFlag = 0;

				// checking for intersection... 
				if (x1 == x2)
				{
					/*three cases can arive-
						1. lines are towards top of the intersection
						2. lines are towards bottom
						3. one line is towards top and other is towards bottom
					*/
					if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
					{
						x1 = x2;
						ymax1 = ymax2;
					}
					else
					{
						coordCount++;
						FillFlag = 1;
					}
				}
				else
				{
					coordCount++;
					FillFlag = 1;
				}


				if (FillFlag)
				{
					//drawing actual lines... 
					currentCanvas.DrawALine(x1, i, x2, i, selectedColour);
					affichage();
					// printf("\nLine drawn from %d,%d to %d,%d",x1,i,x2,i); 
				}

			}

			j++;
		}


		// 5. For each nonvertical edge remaining in AET, update x for new y 
		updatexbyslopeinv(&ActiveEdgeTuple);
	}
	std::list<Tuple> tmpfin;
	tmpfin = Tool_Polygone::MultiSommets.back();
	int** tabfin = new int* [tmpfin.size()];
	int* tabxfinxy = new int[tmpfin.size() * 2];
	int sizefin = tmpfin.size();
	int iterfin = 0;
	for (int i = 0; i < sizefin; i++) {
		tabfin[i] = &tabxfinxy[i * 2];
		tabfin[i][0] = tmpfin.front().x;
		tabfin[i][1] = tmpfin.front().y;
		tmpfin.pop_front();
	}
	for (iterfin; iterfin < sizefin - 1; iterfin++) {

		currentCanvas.DrawALine(tabfin[iterfin][0], tabfin[iterfin][1], tabfin[iterfin + 1][0], tabfin[iterfin + 1][1], Tool_Polygone::bord_color);

	}
	currentCanvas.DrawALine(tabfin[iterfin][0], tabfin[iterfin][1], tabfin[0][0], tabfin[0][1], Tool_Polygone::bord_color);

	printf("\nScanline filling complete");

}



/*
	Fill algorithm from a position

	@param startColour - The colour we should be replacing
	@param cx - x coordinate of pixel to fill from
	@param cy - y coordinate of pixel to fill from
*/


//Tuple c = { 0,0 };
/*void Tool_Fill::Fill(Colour startColour, int cx, int cy) {


	std::vector<Tuple> intersection;
	int nbintersection = intersection.size();
	int parite = 0;
	for (int y = 0; y < Tool_Selection::posFinY; y++)
	{
		parite = 0;
		for (int x = 0; x < Tool_Selection::posFinX; x++)
		{
			for (const cotes vertex : Tool_Polygone::ListeCotes)
			{
				float maxY = std::max(vertex.yA, vertex.yB);
				float maxX = std::max(vertex.xA, vertex.xB);
				float minY = std::min(vertex.yA, vertex.yB);
				float minX = std::min(vertex.xA, vertex.xB);
				float CoefD = ((maxY - minY) / (maxX - minX));
				float b = minY - CoefD * minX;
				if (CoefD * x + b - y == 0)
				{
					++parite;
					break;
				}
			}
			if (parite % 2 != 0)
			{
				currentCanvas.SetPixelColour(x, y, selectedColour);
			}

		}

	}
	*/
	/*Tuple LCA[2] = { {0,0},{0,0} };
	for (int y = Tool_Selection::posDepY+1; y < Tool_Selection::posFinY; y++) {
		for (int x = Tool_Selection::posDepX+1; x < Tool_Selection::posFinX; x++) {
			if (currentCanvas.GetPixelColour(x, y) != startColour) {
				if (LCA[0].x == 0 and LCA[0].y == 0) {
					Tuple tmp = { x,y };
					LCA[0] = tmp;
				}
				else if (LCA[1].x == 0 and LCA[1].y == 0) {
					Tuple tmp = { x,y };
					LCA[1] = tmp;
					break;
				}
			}
		}
		if (LCA[0].x != c.x and LCA[0].y != c.y and LCA[1].x != c.x and LCA[1].y != c.y) {
			for (int x2 = LCA[0].x; x2 < LCA[1].x; x2++) {
				currentCanvas.SetPixelColour(x2, LCA[1].y, selectedColour);
				
			}
			affichage();
			Tuple tmp = { 0,0 };
			LCA[0] = tmp;
			LCA[1] = tmp;
		}
	}
}
*/
void Tool_Fill::FillCercle(Colour startColour, int cx, int cy) {
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
				if (i % 50 == 0) {
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
			Tool_Fill::FillCercle(colourWherePressed, canvasCoordX, canvasCoordY);
			return true;
		}
	}
	return false;
}

bool Tool_Fill::PressedLCA(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		int canvasCoordX = (x - currentCanvas.xOffset) / currentCanvas.zoom;
		int canvasCoordY = (y - currentCanvas.yOffset) / currentCanvas.zoom;
		Colour colourWherePressed = currentCanvas.GetPixelColour(canvasCoordX, canvasCoordY);
		if ((colourWherePressed.r != selectedColour.r) || (colourWherePressed.g != selectedColour.g) || (colourWherePressed.b != selectedColour.b)) {
			Tool_Fill::Fill(colourWherePressed);
			return true;
		}
	}
	return false;
}