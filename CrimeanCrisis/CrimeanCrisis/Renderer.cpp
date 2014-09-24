#include "Renderer.h"

using namespace glm;

Renderer::Renderer() : window(NULL)
{

}

Renderer::Renderer(std::list<GraphicObject> *o)
{
	objList = o;		//przeciążyć operator
}

Renderer::~Renderer()
{
	delete sun;
	delete gameUI;
	delete mainMenu;
	delete serverMenu;
}

void Renderer::updateWindow()
{
	glutSwapBuffers();
}

void Renderer::init()
{
	// EKRAN STARTOWY
	//this->screen = Screen::GameScreen;
	this->screen = Screen::MainMenuScreen;

	this->win.width = 1024;
	this->win.height = 640;
	this->win.title = "CrimeanCrisis BETA";
	this->win.field_of_view_angle = 45;
	this->win.z_near = 0.0f;
	this->win.z_far = 1500.0f;
	plain = new GraphicObject();
	plain->loadOBJ("models/plain.obj", "grafiki/tex1.bmp");

	leftParam = rightParam = topParam = bottomParam = CUT_PARAM;
	nearParam = 15.0;
	farParam = 200.0;

	this->cam.x = 25.0;
	this->cam.y = 50.0;
	this->cam.z = 25.0;
	this->dir.x = this->dir.y = this->dir.z = 0;

	mousePressed = false;

	// sloneczko
	Vector lightPos(0, 70, 0), lightDir(0, -1, 0);
	float att[3] = { 1.5, 0, 0 };
	sun = new Light(0, lightPos, lightDir, att, 80, 0);
	// deszcz
	isRaining = false;
	// UI
	gameUI = new GameUI(win.width, win.height);
	mainMenu = new MainMenu();
	serverMenu = new ServerMenu(win.width, win.height);

	// FLAME TEST
	flame1 = new Flame(0, 10, 0);

	x1 = x2 = y1 = y2 = 0;
}

void Renderer::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (screen)
	{
	case MainMenuScreen:
		this->displayMainMenuScreen();
		break;

	case ServerMenuScreen:
		this->displayServerMenuScreen();
		break;

	case GameScreen:
		this->displayGameScreen();
		break;
	}

	glutSwapBuffers();
}

void Renderer::displayMainMenuScreen()
{
	set2D(this->win.width, this->win.height);

	mainMenu->drawMainMenu();
}

void Renderer::displayServerMenuScreen()
{
	set2D(this->win.width, this->win.height);

	serverMenu->drawServerMenu();
}

void Renderer::displayGameScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cam.x, cam.y, cam.z, dir.x, dir.y, dir.z, 0.0, 1.0, 0.0);

	set3D(this->win.width, this->win.height);

	// sloneczko
	setLight(*sun);
	drawBulb(*sun); // to sie wytnie

	glPopMatrix();
	glPushMatrix();
	plain->Draw();		// rysuj mapę

	std::list<GraphicObject>::iterator iter;	// rysuj jednostki
	for (iter = objList->begin(); iter != objList->end(); ++iter) {
		iter->Draw();
	}
		
	glPopMatrix();

	if (isRaining)
	{
		rain->displayRain();
	}

	// elementy pomocnicze
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 11, 0);
	glVertex3f(100, 11, 0);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 11, 0);
	glVertex3f(0, 11, 100);
	glEnd();

	// FLAME TEST
	flame1->displayFlame();

	// RYSOWANIE UI
	set2D(this->win.width, this->win.height);

	gameUI->drawUI();
}

void Renderer::animate()
{
	if (isRaining)
	{
		rain->emitParticles();
		rain->affectParticles();
	}

	// FLAME TEST
	flame1->emitParticles();
	flame1->affectParticles();

	glutPostRedisplay();
}

void Renderer::keyboard(unsigned char key, int x, int y)
{
	float yChange = 1;

	switch (key) {
	case 'w':
		cam.y -= 0.5;
		dir.y -= 0.5;
		break;
	case 's':
		cam.y += 0.5;
		dir.y += 0.5;
		break;
	/*case 'u':
		obj->rot.x += 2.0;
		break;
	case 'o':
		obj->rot.x -= 2.0;
		break;
	case 'y':
		obj->rot.y += 2.0;
		break;
	case 'h':
		obj->rot.y -= 2.0;
		break;
	case 't':
		obj->rot.z += 2.0;
		break;
	case 'g':
		obj->rot.z -= 2.0;
		break;
	case 'j':
		obj->pos.x -= 1.0;
		break;
	case 'l':
		obj->pos.x += 1.0;
		break;
	case 'i':
		obj->pos.z -= 1.0;
		break;
	case 'k':
		obj->pos.z += 1.0;
		break;*/
	case KEY_ESCAPE:
		exit(0);
		break;

	// Rain
	case 'r':
	case 'R':
		if (isRaining)
		{
			delete rain;
			isRaining = false;
		}
		else
		{
			rain = new Rain();
			isRaining = true;
		}
		break;

	// Camera zoom
	case '-':
		if (cam.y < 100) {
			float deltaX = cam.x - dir.x;
			float deltaZ = cam.z - dir.z;
			cam.x = (cam.y + yChange) * deltaX / cam.y;
			cam.z = (cam.y + yChange) * deltaZ / cam.y;
			cam.y += yChange;
		}
		break;
	case '+':
	case '=':
		if (cam.y > 25) {
			float deltaX = cam.x - dir.x;
			float deltaZ = cam.z - dir.z;
			cam.x = (cam.y - yChange) * deltaX / cam.y;
			cam.z = (cam.y - yChange) * deltaZ / cam.y;
			cam.y -= yChange;
		}
		break;
	}
}

void Renderer::specialKeys(int key, int x, int y)
{
	float move = 2.5;

	switch (key)
	{
	case GLUT_KEY_RIGHT:
		cam.x += move;
		dir.x += move;
		cam.z -= move;
		dir.z -= move;
		break;
	case GLUT_KEY_LEFT:
		cam.x -= move;
		dir.x -= move;
		cam.z += move;
		dir.z += move;
		break;
	case GLUT_KEY_UP:
		cam.x -= move;
		dir.x -= move;
		cam.z -= move;
		dir.z -= move;
		break;
	case GLUT_KEY_DOWN:
		cam.x += move;
		dir.x += move;
		cam.z += move;
		dir.z += move;
		break;
	}
}

void Renderer::mouse(int button, int state, int x, int y)
{
	ClickResult result = ClickResult::NoneResult;
	ServerProperties * serverProperties = new ServerProperties; 
	strcpy_s(serverProperties->host, "");
	strcpy_s(serverProperties->port, "");


	switch (screen)
	{
	case MainMenuScreen:
		result = mainMenu->whatIsClicked(x, win.height - y);
		break;

	case ServerMenuScreen:
		result = serverMenu->whatIsClicked(x, win.height - y, serverProperties);
		break;

	case GameScreen:
		result = gameUI->whatIsClicked(x, win.height - y);
		break;
	}
	
	if (result != ClickResult::NoneResult && state == GLUT_DOWN)
	{
		switch (result)
		{
		case Multiplayer:
			screen = Screen::ServerMenuScreen;
			break;

		case SelectServer:
			// TODO: 
			// mamy juz dane servera (moga byc puste, jesli nie wybrano)
			screen = Screen::GameScreen;
			break;	

		case Back:
			screen = Screen::MainMenuScreen;
			break;

		case Quit:
			exit(0);
			break;
		}
		return;
	}


	// UWAGA, PROGRAM TU NIE WEJDZIE JESLI KLIKNIEMY W UI
	// SPRAWDZIC CZY JESTESMY W GAME SCREEN
	switch (button)
		//case GLUT_LEFT_BUTTON:
		//	if (state == GLUT_DOWN)			// TEST
		//		printf("Click!");
		//	else if (state == GLUT_UP)
		//		printf("Clock!");
		case GLUT_LEFT_BUTTON:
	{
		// TODO
		// znacznik bool selected

		if (state == GLUT_DOWN)		// pobranie początkowych współrzędnych
		{
			x1 = x;
			y1 = y;
		}
		if (state == GLUT_UP)		// pobranie końcowych współrzędnych i ray casting + konwersja ze współrzędnych kursora do mapy
		{
			x2 = x;
			y2 = y;
		}
		printf("X1 coordinate: %d\nY1 coordinate: %d\n\n", x1, y1);
		printf("X2 coordinate: %d\nY2 coordinate: %d\n\n", x2, y2);
	}
}

void Renderer::resize(int w, int h)
{
	// Set viewport size to be entire OpenGL window.
	glViewport(0, 0, w, h);
	set3D(w, h);
	this->win.width = gameUI->width = w;
	this->win.height = gameUI->height = h;
}

void Renderer::set3D(int w, int h)
{
	// Set matrix mode to projection.
	glMatrixMode(GL_PROJECTION);
	// Clear current projection matrix to identity.
	glLoadIdentity();
	// wysokosc okna wiêksza od wysokosci okna
	if (w < h && w > 0) {
		glFrustum(-leftParam, rightParam, -bottomParam * h / w, topParam * h / w, nearParam, farParam);
	}
	else {
		// szerokosc okna wiêksza lub równa wysokosci okna
		if (w >= h && h > 0) {
			glFrustum(-leftParam * w / h, rightParam * w / h, -bottomParam, topParam, nearParam, farParam);
		}
	}
	glEnable(GL_LIGHT0);
}

void Renderer::set2D(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	// Clear current projection matrix to identity.
	glLoadIdentity();
	// Specify the orthographic projection, 
	glOrtho(0, w, 0, h, 0, 1);
	// Set matrix mode to modelview.
	glMatrixMode(GL_MODELVIEW);
	// Clear current modelview matrix to identity.
	glLoadIdentity();
	glDisable(GL_LIGHT0);
}

void Renderer::defaultMaterial() {
	GLfloat defaultAmbient[] = { 0.2, 0.2, 0.2, 1 };
	GLfloat defaultDiffuse[] = { 0.8, 0.8, 0.8, 1 };
	GLfloat defaultSpecular[] = { 0, 0, 0, 1 };
	GLfloat defaultShininess = 0;
	GLfloat defaultEmission[] = { 0, 0, 0, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, defaultShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defaultEmission);
}

void Renderer::drawBulb(Light light)
{
	float emission[] = { 0.9, 0.9, 0.8, 1 };
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glColor4f(1.0, 1.0, 0.8, 0.95);

	glPushMatrix();
	Vector pos = light.getPos();
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(0.7, 20, 20);
	glPopMatrix();

	defaultMaterial();
}

void Renderer::setLight(Light light) {
	GLenum number = GL_LIGHT0 + light.getNumber();
	Vector pos = light.getPos();
	float tab[4];
	tab[0] = pos.x;
	tab[1] = pos.y;
	tab[2] = pos.z;
	tab[3] = 1;
	glLightfv(number, GL_POSITION, tab);
	glLightfv(number, GL_SPOT_DIRECTION, light.getDir());
	glLightf(number, GL_SPOT_CUTOFF, light.getCutoff());
	glLightf(number, GL_CONSTANT_ATTENUATION, light.getAttenuation(0));
	glLightf(number, GL_LINEAR_ATTENUATION, light.getAttenuation(1));
	glLightf(number, GL_QUADRATIC_ATTENUATION, light.getAttenuation(2));
	glLightf(number, GL_SPOT_EXPONENT, light.getExponent());
}