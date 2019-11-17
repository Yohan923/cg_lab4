#pragma once

#include <unordered_map>
#include <stdafx.h>

class Input
{
public:
	static float deltaTime;

	static bool getKey(const char name);
	static bool getKeyDown(const char name);
	static bool getKeyUp(const char name);

	static bool getMouseButton(int button);
	static bool getMouseButtonDown(int button);
	static bool getMouseButtonUp(int button);

	static void init();
	static void update();

	static glm::vec2 getMousePos() { return curr.mousePos; }
	static glm::vec2 getPrevMousePos() { return prev.mousePos; }

	class InputState
	{
	public:
		bool keyDown[256];
		std::unordered_map<int, bool> specialKeyDown;
		std::unordered_map<int, bool> mouseDown;
		glm::vec2 mousePos;
		int wheel = 0;

		InputState() { memset(keyDown, 0, 256); }
	};

	static InputState prev;
	static InputState curr;
	static InputState next;

protected:
	Input();
	~Input();

	static void onKeyDown(unsigned char key, int x, int y);
	static void onKeyUp(unsigned char key, int x, int y);
	static void onSpecialKeyDown(int key, int x, int y);
	static void onSpecialKeyUp(int key, int x, int y);
	static void onMouseStateChanged(int button, int state, int x, int y);
	static void onMouseMoved(int x, int y);
	static void onMouseWheelChanged(int wheel, int direction, int x, int y);
};