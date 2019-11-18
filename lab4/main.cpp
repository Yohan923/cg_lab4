// Windows includes (For Time, IO, etc.)
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.

// OpenGL includes
#include <GL/glew.h>
#include <GL/freeglut.h>

// Project includes
#include "maths_funcs.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <GameObject.h>
#include <Input.h>
#include <Transform.h>
#include <Camera.h>
#include <GameObjectManager.h>
#include <Resources.h>
#include <Mesh.h>
#include <MeshFilter.h>
#include <MeshRenderer.h>
#include <Material.h>
#include <CameraControl.h>
#include <SolarSystem.h>

using namespace std;

int width = 1024;
int height = 768;
float elapsedSeconds = 0;
float totalSeconds = 0;
bool nextRunning = true;
bool running = true;
long long elapsedTicks;
long long totalTicks;
float timeRatio;
float prevElapsedSeconds;
float previousTotalSeconds;

void updateTime()
{
	float newTotalSeconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	if (nextRunning)
	{
		prevElapsedSeconds = elapsedSeconds;
		elapsedSeconds = newTotalSeconds - previousTotalSeconds;
		totalSeconds += elapsedSeconds;
		elapsedTicks = (long long)(elapsedSeconds * 1e7);
		totalTicks = elapsedTicks;
		timeRatio = prevElapsedSeconds == 0 ? 0 : elapsedSeconds / prevElapsedSeconds;
	}
	else
	{
		elapsedSeconds = 0;
		elapsedTicks = 0;
		timeRatio = 0;
	}

	running = nextRunning;
	previousTotalSeconds = newTotalSeconds;
}

void update()
{
	updateTime();

	Input::deltaTime = elapsedSeconds;
	Input::update();

	GameObjectManager::update();

	// exit game if press esc
	if (Input::curr.keyDown[27]) glutLeaveMainLoop();
}

void draw()
{
	GameObjectManager::draw();
}

void display() {
	update();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();

	glutSwapBuffers();
}

int initOpenGl(int* argc, char** argv)
{
	// Set up the window
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	glutDisplayFunc(display);
	glutIdleFunc(display);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	glEnable(GL_DEPTH_TEST);

	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
}

void initSkyBox() 
{
	auto* skyBox = Resources::loadSkyBox();
	auto meshes = skyBox->meshFilters;

	vector<std::string> faces =
	{
		"resources/skyboxes/nebula/left.tga",
			"resources/skyboxes/nebula/right.tga",
			"resources/skyboxes/nebula/top.tga",
			"resources/skyboxes/nebula/bottom.tga",
			"resources/skyboxes/nebula/front.tga",
			"resources/skyboxes/nebula/back.tga"
	};

	GLuint cubeMap = Resources::loadCubemap(faces);
	meshes.at(0)->mesh->setTexture(cubeMap);
}

void initCamera() {
	auto* cameraObject = new GameObject();
	cameraObject->transform->position = glm::vec3(0, 0, 0);

	// Camera
	auto* camera = new Camera();
	cameraObject->addComponent(camera);

	camera->cameraFront = glm::vec3(0, 0, -1);
	camera->fov = 45;
	camera->aspect = (float)width / height;
	camera->nearClipPlane = 0.01f;
	camera->farClipPlane = 1000.f;
	camera->updateProjectionMatrix();
	camera->pitch = 0;
	camera->yaw = -90;
	auto* cameraController = new CameraControl();
	cameraObject->addComponent(cameraController);
}

void initAcanologia()
{
	auto* acanologia = Resources::loadGameObject("resources/models/acanologia/aca.dae", commonVS, commonFS);
	acanologia->transform->position = glm::vec3(0, 0, 0);
	acanologia->transform->rotation = glm::quat(glm::vec3(0, 0, 0));
	acanologia->transform->scaling = glm::vec3(0.1, 0.1, 0.1);

	auto meshes = acanologia->meshFilters;
	meshes.at(0)->mesh->setTexture(Resources::loadTexture("resources/models/acanologia/aca.Texture/body.png"));
	meshes.at(1)->mesh->setTexture(Resources::loadTexture("resources/models/acanologia/aca.Texture/body2.png"));
	meshes.at(2)->mesh->setTexture(Resources::loadTexture("resources/models/acanologia/aca.Texture/wings.png"));
}

void initSolarSystem()
{
	auto* gameObject = new GameObject();
	auto* tmp = new SolarSystem();

	gameObject->addComponent(tmp);
}

void initObjects()
{
	Input::init();

	initCamera();

	initSkyBox();

	initAcanologia();

	initSolarSystem();

	previousTotalSeconds = 0;
}

bool getRunning()
{
	return running;
}

int main(int argc, char** argv) {

	initOpenGl(&argc, argv);
	initObjects();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
