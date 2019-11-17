#pragma once
#include <stdafx.h>
#include <Utils.h>


static BaseLight baseLight = 
 {
 	glm::vec3(1),
 	0.55f,
 	0.6f,
 	1.0f,
 	1.0f
 };

static Attenuation attenuation =
 {
 	1.0f,
 	0.0014f,
 	0.000007f,
 };

static const char* commonVS = "resources/shaders/simpleVertexShader.txt";
static const char* commonFS = "resources/shaders/simpleFragmentShader.txt";

static const char* sunVS = "resources/shaders/sunVS.txt";
static const char* sunFS = "resources/shaders/sunFS.txt";

static const char* skyboxVS = "resources/shaders/skyBoxVS.txt";
static const char* skyboxFS = "resources/shaders/skyBoxFS.txt";