/*
	main.cpp
	Entry point for 2D Drawing Tool
*/

//CREATED BY GEC

#include "imgui/imgui.h"
#include "backends/imgui_impl_glut.h"
#include "backends/imgui_impl_opengl2.h"


#include <stddef.h>
#include <iostream>
#include <math.h>
#include <glut.h>

// For each static class we import from a header file, we need to redefine its variables here

#include "Colour.h"
Colour selectedColour = { 1.0f, 0.0f, 0.0f };
bool canvasAssigned = false;
#include "File Management.h"
#include "Button.h"
#include "Fonts.h"
#include "Pointer.h"
#include "Cover.h"
bool Cover::show = false;
#include "Canvas.h"
#include "Colour Palette.h"
const Colour ColourPalette::colours[] = {
	{ 1.0f, 0.0f, 0.0f },		// red
	{ 0.0f, 1.0f, 0.0f },		// green
	{ 0.0f, 0.0f, 1.0f },		// blue
	{ 0.0f, 0.0f, 0.0f },		// black
	{ 1.0f, 1.0f, 1.0f }		// white
};
static ImVec4 color = ImVec4(255.0f, .0f, 0.0f, 1.f);
int ColourPalette::selectedIndex = 0;
int ColourPalette::palette_x_pos = 750;
#include "Alert Dialogue.h"
bool AlertDialogue::show = false;
Button AlertDialogue::okButton;
std::string AlertDialogue::message = "";
#include "Yes No Dialogue.h"
bool YesNoDialogue::show = false;
Button YesNoDialogue::yesButton;
Button YesNoDialogue::noButton;
std::string YesNoDialogue::message = "";
Callback YesNoDialogue::yesCallback;
#include "Open File Dialogue.h"
bool OpenFileDialogue::show = false;
Button OpenFileDialogue::cancelButton;
std::vector<Button> OpenFileDialogue::fileButtons;
std::vector<Button> OpenFileDialogue::crossButtons;
std::string OpenFileDialogue::deletionPendingFileName = "";
#include "Save File Dialogue.h"
bool SaveFileDialogue::show = false;
Button SaveFileDialogue::cancelButton;
Button SaveFileDialogue::saveButton;
std::string SaveFileDialogue::fileName = "";
bool SaveFileDialogue::showTooLongText = false;
#include "Top Menu Bar Callbacks.h"
#include "Top Menu Bar.h"
std::vector<Button> TopMenuBar::buttons;
#include "Toolbar.h"

static int toolSelectedButton = 0;

int Toolbar::selectedButton = 0;
Button Toolbar::penButton;
Button Toolbar::moveButton;
Button Toolbar::fillLCAButton;
Button Toolbar::fillCircleButton;
Button Toolbar::rectButton;
Button Toolbar::selectionButton;
Button Toolbar::LCAButton;

#include "Tool_Pen.h"
bool Tool_Pen::isMouseDown = false;
int Tool_Pen::mouseLastX = 0;
int Tool_Pen::mouseLastY = 0;
#include "Tool_Fill.h"
#include "Tool_Rect.h"
bool Tool_Rect::isMouseDown = false;
int Tool_Rect::startMouseX = 0;
int Tool_Rect::startMouseY = 0;
#include "Tool_Selection.h"
bool Tool_Selection::isMouseDown = false;
int Tool_Selection::startMouseX = 0;
int Tool_Selection::startMouseY = 0;
int Tool_Selection::posDepX = 0;
int Tool_Selection::posDepY = 0;
int Tool_Selection::posFinX = 0;
int Tool_Selection::posFinY = 0;
int Tool_Selection::departX = 0;
int Tool_Selection::departY = 0;
bool Tool_Selection::firstPickSelect = true;
std::list<Tuple> Tool_Selection::CotesFenetre = {};
int Tool_Selection::polygone_index = 20;
int Tool_Selection::nbpoly = 20;

#include "Tool_Circle.h"
bool Tool_Circle::isMouseDown = false;
int Tool_Circle::startMouseX = 0;
int Tool_Circle::startMouseY = 0;
#include "Tool_Polygone.h"
bool Tool_Polygone::isMouseDown = false;
int Tool_Polygone::startMouseX = 0;
int Tool_Polygone::startMouseY = 0;
int Tool_Polygone::departX = 0;
int Tool_Polygone::departY = 0;
bool Tool_Polygone::firstPick = true;
Colour Tool_Polygone::bord_color = { 0.0f, 0.0f, 0.0f };
std::list<Tuple> Tool_Polygone::ListeSommets = {};
std::list<cotes> Tool_Polygone::ListeCotes = {};
std::list<std::list<Tuple>> Tool_Polygone::MultiSommets = {};
#include "Tool_Move.h"
int Tool_Move::flickerFrameCount;
bool Tool_Move::flickerColor;
bool Tool_Move::isMouseDown;
int Tool_Move::startMouseX;
int Tool_Move::startMouseY;
int Tool_Move::endMouseX;
int Tool_Move::endMouseY;
bool Tool_Move::isDisplaying;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static bool show_demo_window = true;

void vRappelSousMenu1(int i)
{
	switch (i)
	{
	case 11:
		// Rouge
		color = ImVec4(ColourPalette::colours[0].r, ColourPalette::colours[0].g, ColourPalette::colours[0].b,1.0f);
		break;
	case 12:
		// Vert
		color = ImVec4(ColourPalette::colours[1].r, ColourPalette::colours[1].g, ColourPalette::colours[1].b, 1.0f);
		break;
	case 13:
		// Bleu
		color = ImVec4(ColourPalette::colours[2].r, ColourPalette::colours[2].g, ColourPalette::colours[2].b, 1.0f);
		break;
	case 14:
		// Noir
		color = ImVec4(ColourPalette::colours[3].r, ColourPalette::colours[3].g, ColourPalette::colours[3].b, 1.0f);
		break;
	case 15:
		// Blanc
		color = ImVec4(ColourPalette::colours[4].r, ColourPalette::colours[4].g, ColourPalette::colours[4].b, 1.0f);
		break;
	}
}

void vRappelSousMenu2(int i)
{
	printf("rappel de l'element %d\n", i);
}

void vRappelMenuPrincipal(int i)
{
	switch (i)
	{
	case 1:
		// Bouton Ligne selectionner
		Toolbar::selectedButton = 3;
		break;
	case 5:
		// Bouton cercle
		Toolbar::selectedButton = 5;
		break;
	case 3:
		// Bouton Fenetre/Select selectionner
		Toolbar::selectedButton = 4;
		break;
	case 4:
		// Bouton Remplissage selectionner
		Toolbar::selectedButton = 2;
		break;
	case 6:
		// Bouton Remplissage selectionner
		Toolbar::selectedButton = 0;
		break;
	case 2:
		// Bouton Trace fenetre
		Toolbar::selectedButton = 10;
		break;
	case 7:
		// Bouton Polygone
		Toolbar::selectedButton = 6;
		break;
	case 8:
		// Bouton LCA
		Toolbar::selectedButton = 7;
		break;
	case 21:
		// Bouton Polygone 1
		Toolbar::selectedButton = 21;
		break;
	case 22:
		// Bouton Polygone 2
		Toolbar::selectedButton = 22;
		break;
	case 23:
		// Bouton Polygone 1
		Toolbar::selectedButton = 23;
		break;
	case 24:
		// Bouton Polygone 2
		Toolbar::selectedButton = 24;
		break;
	case 25:
		// Bouton Polygone 1
		Toolbar::selectedButton = 25;
		break;

	}

}

void menu(int item)
{
	switch (item) {
	case 1:
		vRappelSousMenu2(item);
		break;
	case 2:
		exit(0);
		break;
	}
}

static bool showToolBar = true;

void NewConfirmedCallback() {
	canvasAssigned = true;
	currentCanvas = NewCanvas(600, 600, 0,0);
	Tool_Polygone::ListeCotes.clear();
	Tool_Polygone::ListeSommets.clear();
	Tool_Polygone::MultiSommets.clear();
	Tool_Polygone::firstPick = true;
	Tool_Selection::firstPickSelect = true;
	int nbmenuajoute = (Tool_Selection::nbpoly)%20;
	for (int i = 0; i < nbmenuajoute; ++i)
	{
		glutRemoveMenuItem(glutGet(GLUT_MENU_NUM_ITEMS));
	}
	Tool_Selection::nbpoly = 20;
}

void OpenButtonPressed() {
	OpenFileDialogue::Show();
}

void SaveButtonPressed() {
	// If there is no current canvas, show an error
	if (!canvasAssigned) {
		AlertDialogue::Alert("No canvas has been created. Create one before saving.");
		return;
	}
	// If the current canvas is new (without a file path), go to Save File Dialogue (just like Save As)
	if (currentCanvas.fileName == "") {
		SaveFileDialogue::Reset();
		SaveFileDialogue::Show();
		return;
	}
	// If it is new (with a file path) save it to the path
	FileManagement::WriteFile(currentCanvas.fileName, currentCanvas.Serialize());
	AlertDialogue::Alert("Saved to " + currentCanvas.fileName + ".dti");
}

void SaveAsButtonPressed() {
	if (!canvasAssigned) {
		AlertDialogue::Alert("No canvas has been created. Create one before saving.");
		return;
	}
	SaveFileDialogue::Reset();
	SaveFileDialogue::Show();
}

void ColorPicker() {
	selectedColour = { color.x, color.y, color.z };
	// Generate a default palette. The palette will persist and can be edited.
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	static ImVec4 backup_color;
	bool open_popup = ImGui::ColorButton("MyColor##3b", color);
	ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	open_popup |= ImGui::Button("Couleur");
	if (open_popup)
	{
		ImGui::OpenPopup("mypicker");
		backup_color = color;
	}
	if (ImGui::BeginPopup("mypicker"))
	{
		ImGui::Text("Couleur");
		ImGui::Separator();
		ImGui::ColorPicker4("##picker", (float*)&color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_Uint8);
		ImGui::SameLine();

		ImGui::BeginGroup(); // Lock X position
		ImGui::Text("Current");
		ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous");
		if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			color = backup_color;
		ImGui::Separator();
		ImGui::Text("Palette");
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::PushID(n);
			if ((n % 8) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
			if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
				color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

			// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
			// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
	}
}

static ImVec4 activeButtonColor = ImVec4(25.0f/255, 118.0f/255, 210.0f/255, 1.f);
static ImVec4 notActiveButtonColor = ImVec4(0.0f, 75.0f/255, 160.0f/255, 1.f);
static ImVec4 hoveredButtonColor = ImVec4(99.0f/255, 164.0f/255, 255.0f/255, 1.f);
void activeToolBtn(int id, const char *label) {
	ImGui::PushID(1);
	if (Toolbar::selectedButton == id)
		ImGui::PushStyleColor(ImGuiCol_Button, hoveredButtonColor);
	else
		ImGui::PushStyleColor(ImGuiCol_Button, notActiveButtonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredButtonColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeButtonColor);
	if (ImGui::Button(label)) {
		Toolbar::selectedButton = id;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID();

}

void overlay(const char *titre, const char *text) {
	bool open = true;
	ImGuiIO& io = ImGui::GetIO();
	const float DISTANCE = 10.0f;
	static int corner = 3;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.35f);

	if(ImGui::Begin("overlay", &open, window_flags))
	{
		ImGui::Text(titre);
		ImGui::Separator();
		ImGui::Text(text);
	}
	ImGui::End();

}

void my_display_code()
{
	
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Fichier"))
		{
			if (ImGui::MenuItem("Nouveau")) {
				NewConfirmedCallback();
			}
			if (ImGui::MenuItem("Ouvrir")) {
				OpenButtonPressed();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Enregister")) {
				SaveButtonPressed();
			}
			if (ImGui::MenuItem("Enregister sous ")) {
				SaveAsButtonPressed();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	static bool p_open = true;
	static int display_lines = 10;
	static int type = 0;


	ImGui::SetNextWindowSize(ImVec2(160, 350));
	
	if (ImGui::Begin("Outils", &showToolBar))
	{
		
		ColorPicker();

		activeToolBtn(0, "Stylo");
		activeToolBtn(1, "Deplacer");
		activeToolBtn(2, "Remplisage cercle");
		activeToolBtn(3, "Ligne");
		activeToolBtn(6, "Polygone");
		activeToolBtn(4, "Fenetrage");
		activeToolBtn(10, "Fenetrage Rectangle");
	
	}
	ImGui::End();
	if (Toolbar::selectedButton == 1)
		overlay("Commandes", "Utiliser les fleches pour bouger");
	if (Toolbar::selectedButton == 6)
		overlay("Consignes", "Cliquer pour definir les point puis appuyez sur \"a\" pour terminer le polygone");
	if (Toolbar::selectedButton == 10)
		overlay("Consignes", "appuyer une premiere fois sur le clic gauche puis deplacez la souris et relachez pour dessiner la fenetre");
	if (Toolbar::selectedButton == 4)
		overlay("Consignes", "Cliquer pour definir les point (dans le sens trigonometrique) puis appuyez sur \"z\" pour terminer la fenetre");
}

int nSousmenu1, nSousmenu2, nSousmenu3, nMenuprincipal; // Num�ros (identifiants) des menus
int nTue = 0;
/*
	OpenGL display function
*/
void display()
{

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();

	my_display_code();

	
	

	glClear(GL_COLOR_BUFFER_BIT);

	
	glPushMatrix();

	// Rescale to "pixel" scale - position (x, y) is x pixels along, y pixels up
	// Allows user to resize window without stretching UI elements
	//glScalef((double)(1600) / (double)(glutGet(GLUT_WINDOW_WIDTH)), (double)(900) / (double)(glutGet(GLUT_WINDOW_HEIGHT)), 1.0f);

	// Draw the canvas and any overlays from tools in use
	if (canvasAssigned) {
		currentCanvas.Draw(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		ToolEvents::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}

	// Draw the colour palette
	//ColourPalette::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Draw the toolbar on left hand side
	//Toolbar::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Draw the top menu bar buttons (new, open, save, etc)
	//TopMenuBar::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Draw the dark semi-transparent cover if necessary
	Cover::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Draw File Dialgues if necessary
	OpenFileDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	SaveFileDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	AlertDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	YesNoDialogue::Display(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	//glutCreateMenu(menu);
	//glutAddMenuEntry("Next Drawing Mode", 1);
	//glutAddMenuEntry("Quit", 2);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	

	// Draw mouse pointer last (so it appears above everything else)
	Display_Pointer();

	// Rendering
	ImGui::Render();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	glPopMatrix();
	glutSwapBuffers();
}


/*
	Handles mouse click events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
*/
void mouse_click(int button, int state, int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	// If we are currently using a tool which wants all mouse events first, give it the mouse events
	// Otherwise pass the mouse event onto each UI element in turn until it gets handled, in the order of depth
	if (io.WantCaptureMouse) {
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{
				io.MouseDown[0] = true;
			}
			if (state == GLUT_UP)
			{
				io.MouseDown[0] = false;
			}
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
			{
				io.MouseDown[2] = true;
			}
			if (state == GLUT_UP)
			{
				io.MouseDown[2] = false;
			}
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN)
			{
				io.MouseDown[1] = true;
			}
			if (state == GLUT_UP)
			{
				io.MouseDown[1] = false;
			}
		default:
			break;
		}
		return;
	}
	else {

		if ((!canvasAssigned) || (!ToolEvents::BlockMousePress(button, state, x, y))) {
			// First pass the event onto the Dialogues
			if (AlertDialogue::Pressed(button, state, x, y)) {
				return;
			}
			if (YesNoDialogue::Pressed(button, state, x, y)) {
				return;
			}
			if (OpenFileDialogue::Pressed(button, state, x, y)) {
				return;
			}
			if (SaveFileDialogue::Pressed(button, state, x, y)) {
				return;
			}
			// If not handled, maybe the Cover will block it
			if (Cover::Pressed(button, state, x, y)) {
				return;
			}
			// If not handled pass it onto buttons/toolbars
			/*if (TopMenuBar::Pressed(button, state, x, y)) {
				return;
			}*/
			/*if (Toolbar::Pressed(button, state, x, y)) {
				return;
			}*/
			if (ColourPalette::Pressed(button, state, x, y)) {
				return;
			}
		}
		// If it hasn't been handled, pass it on to the selected tool if we have a canvas
		if (canvasAssigned) {
			if (ToolEvents::Pressed(button, state, x, y)) {
				return;
			}
		}
	}
}


/*
	Handles all mouse movement events

	@param x - The new x coordinate of the mouse
	@param y - The new y coordinate of the mouse
*/
void mouse_motion(int x, int y)
{
	// Remember new cursor position for the Pointer
	cursorX = x;
	cursorY = y;

	ImGuiIO& io = ImGui::GetIO();
	io.MousePos.x = x;
	io.MousePos.y = y;

	// Pass the mouse move event onto Dialogues
	if (!io.WantCaptureMouse) {
		if (AlertDialogue::Hover(x, y)) {
			return;
		}
		if (YesNoDialogue::Hover(x, y)) {
			return;
		}
		if (OpenFileDialogue::Hover(x, y)) {
			return;
		}
		if (SaveFileDialogue::Hover(x, y)) {
			return;
		}

		// If not handled, maybe the Cover will block it
		if (Cover::Hover(x, y)) {
			return;
		}

		// If not handled pass it onto buttons/toolbars
		if (Toolbar::Hover(x, y)) {
			return;
		}
		if (TopMenuBar::Hover(x, y)) {
			return;
		}

		// If it hasn't been handled, pass it on to the selected tool if we have a canvas
		if (canvasAssigned) {
			if (ToolEvents::Hover(x, y)) {
				return;
			}
		}
	}
}



/*
	Handles standard keyboard events

	@param key - The key that was pressed
	@param x - The x position of the mouse
	@param y - The y position of the mouse
*/

void keyboard(unsigned char key, int x, int y)
{
	// Save File Dialogue should steal the input events if active
	// But Yes/No Dialogue can appear above it so should block inputs if it is being displayed
	if (YesNoDialogue::show) {
		return;
	}
	// Now pass on to Save File Dialogue
	if (SaveFileDialogue::KeyboardPressed(key, x, y)) {
		return;
	} //Telling glut what function to call when the event occurs
	// otherwise check for quitting or zoom
	switch (key)
	{
	case 's':
		// zoom in
		if (canvasAssigned) {
			currentCanvas.zoom++;
		}
		break;
	case 'd':
		// zoom out
		if (canvasAssigned) {
			if (currentCanvas.zoom > 1) {
				currentCanvas.zoom--;
			}
		}
		break;
	case 'a':
		// End a Polygon
		if (canvasAssigned) {
			Tool_Polygone::EndPolygon();
		}
		break;
	case 'z':
		// End a Selection
		if (canvasAssigned) {
			Tool_Selection::End_Selection();
		}
	
	}
}


/*
	Handles special keyboard events

	@param key - The key that was pressed
	@param x - The x position of the mouse
	@param y - The y position of the mouse
*/
void special(int key, int x, int y)
{
	if (!Cover::show) {

		// Pass onto selected tool (Move tool uses arrow keys)
		if (canvasAssigned) {
			if (ToolEvents::SpecialKey(key, x, y)) {
				return;
			}
		}

		// If not handled check arrow keys for panning camera
		/*switch (key)
		{
		case GLUT_KEY_LEFT: if (canvasAssigned) { currentCanvas.xOffset -= 6; } break;
		case GLUT_KEY_RIGHT: if (canvasAssigned) { currentCanvas.xOffset += 6; } break;
		case GLUT_KEY_UP: if (canvasAssigned) { currentCanvas.yOffset -= 6; } break;
		case GLUT_KEY_DOWN: if (canvasAssigned) { currentCanvas.yOffset += 6; } break;
		}*/
	}
}


/*
	Called to intialize all classes
*/
void init()
{
	// Initialize classes where needed
	TopMenuBar::Init();
	Toolbar::Init();
	OpenFileDialogue::Init();
	SaveFileDialogue::Init();
	AlertDialogue::Init();
	YesNoDialogue::Init();

	// Enable transparency (e.g. black semi-transparent cover over screen appears with dialogues)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set initial scale for window coordinates
	gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}


/*
	idle function
*/
void idle() {
	// force a redraw
	// so we get through as many frames as possible (needed for things like blinking of Move tool)
	glutPostRedisplay();
}




/*
	Main entry point of application
*/
int main(int argc, char* argv[])
{


	// create window with title and fixed start size
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(1600,900);
	glutCreateWindow("DigitalPaint2000");

	// define the display function
	glutDisplayFunc(display);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL2_Init();

	

	// define idle and init
	glutIdleFunc(idle);

	// initialize everything
	init();

	// handlers for keyboard input
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	// define mouse pressed event handler
	glutMouseFunc(mouse_click);

	// define mouse movement event handlers
	glutPassiveMotionFunc(mouse_motion); // << when mouse is not being pressed
	glutMotionFunc(mouse_motion); // << when mouse is being pressed


	// initialize everything
	init();
	nSousmenu1 = glutCreateMenu(vRappelSousMenu1);
	glutAddMenuEntry("Rouge", 11);
	glutAddMenuEntry("Vert", 12);
	glutAddMenuEntry("Bleu", 13);
	glutAddMenuEntry("Noir", 14);
	glutAddMenuEntry("Blanc", 15);

	nSousmenu2 = glutCreateMenu(vRappelMenuPrincipal);
	glutAddMenuEntry("Pinceau libre", 6);
	glutAddMenuEntry("Lignes", 1);
	glutAddMenuEntry("Cercle", 5);

	nMenuprincipal = glutCreateMenu(vRappelMenuPrincipal);

	glutAddSubMenu("Couleurs", nSousmenu1);
	glutAddSubMenu("Formes", nSousmenu2);
	glutAddMenuEntry("Tracé Polygone", 7);

	glutAddMenuEntry("Tracé fenêtre", 2);
	glutAddMenuEntry("Fenêtrage", 3);
	glutAddMenuEntry("Remplissage Cercle", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// start first render cycle
	glutMainLoop();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	


	// Gestion de menus
	

	return 0;
}
