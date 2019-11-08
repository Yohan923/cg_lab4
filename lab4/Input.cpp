#include "Input.h"

#include <GL/freeglut.h>

using namespace glm;
using namespace std;

Input::Input()
{
}

Input::~Input()
{
}

void Input::init()
{
	glutKeyboardFunc(onKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialFunc(onSpecialKeyDown);
	glutSpecialUpFunc(onSpecialKeyUp);
	glutMouseFunc(onMouseStateChanged);
	glutMotionFunc(onMouseMoved);
	glutMouseWheelFunc(onMouseWheelChanged);
}

void Input::update()
{
	prev = curr;
	curr = next;
	next.wheel = 0;
}

bool Input::getKey(const char name)
{
	char key = tolower(name);
	return curr.keyDown[name];
}

bool Input::getKeyDown(const char name)
{
	char key = tolower(name);
	return curr.keyDown[key] && !prev.keyDown[key];
}

bool Input::getKeyUp(const char name)
{
	char key = tolower(name);
	return !curr.keyDown[key] && prev.keyDown[key];
}

bool Input::getMouseButton(int button)
{
	return curr.mouseDown[button];
}

bool Input::getMouseButtonDown(int button)
{
	return curr.mouseDown[button] && !prev.mouseDown[button];
}

bool Input::getMouseButtonUp(int button)
{
	return !curr.mouseDown[button] && prev.mouseDown[button];
}

// auto fire while key down, lower-case key
void Input::onKeyDown(unsigned char key, int x, int y)
{
	next.keyDown[tolower(key)] = true;
}

// trigger once when key up
void Input::onKeyUp(unsigned char key, int x, int y)
{
	next.keyDown[tolower(key)] = false;
}

void Input::onSpecialKeyDown(int key, int x, int y)
{
	next.specialKeyDown[key] = true;
}

void Input::onSpecialKeyUp(int key, int x, int y)
{
	next.specialKeyDown[key] = false;
}

// trigger once when mouse up or down
void Input::onMouseStateChanged(int button, int state, int x, int y)
{
	next.mouseDown[button] = state == GLUT_DOWN;

	next.mousePos = vec2(x, y);
}

// only work when mouse down
void Input::onMouseMoved(int x, int y)
{
	next.mousePos = vec2(x, y);
}

// wheel always 0, direction -1 / +1
void Input::onMouseWheelChanged(int wheel, int direction, int x, int y)
{
	next.wheel = direction;
}

Input::InputState Input::prev;
Input::InputState Input::curr;
Input::InputState Input::next;
