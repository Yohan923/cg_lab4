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

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

// Project includes
#include "maths_funcs.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>



/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "../lab4/resources/models/Koltuk.3ds"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/

#pragma region SimpleTypes
typedef struct
{
	size_t mPointCount = 0;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec2> mTextureCoords;
} ModelData;
#pragma endregion SimpleTypes

using namespace std;
GLuint shaderProgramID;

ModelData mesh_data;
unsigned int mesh_vao = 0;
int width = 800;
int height = 600;

GLuint loc1, loc2, loc3;
GLfloat transX, transY, transZ;
GLfloat scaleX, scaleY, scaleZ;
GLfloat rotX, rotY, rotZ;
GLboolean ortho;
GLboolean combi;
GLboolean upward;

glm::vec3 position;
GLfloat horizontalAngle;
GLfloat verticalAngle;
glm::vec3 direction;
glm::vec3 rightSide;
glm::vec3 up;

glm::vec4 transMat;
glm::vec4 scaleMat;
glm::vec4 rotateMat;

// Initial Field of View
GLfloat fov = 45.0f;
GLfloat rotSpeed = 1.0f;
GLfloat cameraSpeed = 2.0f;
GLfloat transSpeed = 3.0f;
GLfloat scaleSpeed = 0.5f;
GLfloat mouseSpeed = 0.005f;

GLfloat getDelta() {
	static DWORD last_time = 0;
	DWORD curr_time = timeGetTime();
	if (last_time == 0)
		last_time = curr_time;
	float delta = (curr_time - last_time) * 0.08f;
	last_time = curr_time;
	return delta;
}


#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData load_mesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate | aiProcess_PreTransformVertices
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS
char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		std::cerr << "Error creating shader..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	const char* pShaderSource = readShaderSource(pShaderText);

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024] = { '\0' };
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cerr << "Error compiling "
			<< (ShaderType == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader program: " << InfoLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		std::cerr << "Error creating shader program..." << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "../lab4/resources/shaders/simpleVertexShader.txt", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "../lab4/resources/shaders/simpleFragmentShader.txt", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { '\0' };
	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
		std::cerr << "Press enter/return to exit..." << std::endl;
		std::cin.get();
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void generateObjectBufferMesh() {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	mesh_data = load_mesh(MESH_NAME);
	unsigned int vp_vbo = 0;
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);
	unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	unsigned int vt_vbo = 0;
	//	glGenBuffers (1, &vt_vbo);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glBufferData (GL_ARRAY_BUFFER, monkey_head_data.mTextureCoords * sizeof (vec2), &monkey_head_data.mTextureCoords[0], GL_STATIC_DRAW);

	unsigned int vao = 0;
	glBindVertexArray(vao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	glEnableVertexAttribArray (loc3);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}
#pragma endregion VBO_FUNCTIONS


void display() {
	getDelta();
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);

	//Declare your uniform variables that will be used in your shader
	int matrix_location = glGetUniformLocation(shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation(shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation(shaderProgramID, "proj");

	// Root of the Hierarchy
	glm::mat4 scaler = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));
	glm::mat4 rotator = glm::rotate(glm::mat4(1.0f), rotX, glm::vec3(1.0f, 0.0f, 0.0f));
	rotator = glm::rotate(rotator, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
	rotator = glm::rotate(rotator, rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 translator = glm::translate(glm::mat4(1.0f), glm::vec3(transX, transY, transZ));
	glm::mat4 model = translator * rotator * scaler;
	glm::mat4 view = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 projection;
	if (ortho) {
		projection = glm::ortho(3.0f, -3.0f, -3.0f, 3.0f, 0.1f, 100.0f);
	}
	else {
		projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 100.0f);
	}

	// update uniforms & draw
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	// Set up the child matrix
	// glm::mat4 modelChild = glm::mat4(1.0);
	// modelChild = glm::translate(modelChild, glm::vec3(0.0f, 1.9f, 0.0f));

	// Apply the root matrix to the child matrix
	// modelChild = model * modelChild;

	// Update the appropriate uniform and draw the mesh again
	// glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &modelChild[0][0]);
	// glDrawArrays(GL_TRIANGLES, 0, mesh_data.mPointCount);

	glutSwapBuffers();
}

void updateScene() {

	if (combi) {
		GLfloat delta = getDelta() * 0.001;
		
		if (upward) {
			transY += delta * transSpeed;
			rotX += rotSpeed * delta * 50;
			rotX = fmodf(rotX, 360.0f);
			scaleX += delta * scaleSpeed * 2;
			scaleY += delta * scaleSpeed * 2;
			scaleZ += delta * scaleSpeed * 2;
		}
		else {
			transY -= delta * transSpeed;
			rotX -= rotSpeed * delta * 50;
			rotX = fmodf(rotX, 360.0f);
			scaleX = max(0.0f, scaleX - (delta * scaleSpeed * 2));
			scaleY = max(0.0f, scaleY - (delta * scaleSpeed * 2));
			scaleZ = max(0.0f, scaleZ - (delta * scaleSpeed * 2));
		}
	}

	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Set up the shaders
	GLuint shaderProgramID = CompileShaders();
	// load mesh into a vertex buffer array
	generateObjectBufferMesh();

	transX = transY = transZ = 0.0f;
	scaleX = scaleY = scaleZ = 1.0f;
	rotX = rotY = rotZ = 0.0f;
	ortho = false;
	combi = false;
	upward = false;

	position = glm::vec3(0.0f, 0.0f, 5.0f);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	rightSide = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	up = glm::cross(rightSide, direction);
}

// Placeholder code for the keypress
void keypress(unsigned char key, int x, int y) {

	GLfloat delta = getDelta();
	key = tolower(key);
	bool shift = glutGetModifiers() == GLUT_ACTIVE_SHIFT;

	switch (key) {
		// ctrl + w
	case '\x17':
		rotY += rotSpeed * delta;
		rotY = fmodf(rotY, 360.0f);
		break;
	case 'w':
		if (shift) {
			scaleY += delta * scaleSpeed;
		}
		else {
			transY += delta * transSpeed;
		}
		break;
		// ctrl + s
	case '\x13':
		rotY -= rotSpeed * delta;
		rotY = fmodf(rotY, 360.0f);
		break;
	case 's':
		if (shift) {
			scaleY = max(0.0f, scaleY - (delta * scaleSpeed));
		}
		else {
			transY -= delta * transSpeed;
		}
		break;
		// ctrl + d
	case '\4':
		rotX += rotSpeed * delta;
		rotX = fmodf(rotX, 360.0f);
		break;
	case 'd':
		if (shift) {
			scaleX += delta * scaleSpeed;
		}
		else {
			transX += delta * transSpeed;
		}
		break;
		// ctrl + a
	case '\1':
		rotX -= rotSpeed * delta;
		rotX = fmodf(rotX, 360.0f);
		break;
	case 'a':
		if (shift) {
			scaleX = max(0.0f, scaleX - (delta * scaleSpeed));
		}
		else {
			transX -= delta * transSpeed;
		}
		break;
		// ctrl + q
	case '\x11':
		rotZ -= rotSpeed * delta;
		rotZ = fmodf(rotZ, 360.0f);
		break;
	case 'q':
		if (shift) {
			scaleZ = max(0.0f, scaleZ - (delta * scaleSpeed));
		}
		else {
			scaleX = max(0.0f, scaleX - (delta * scaleSpeed));
			scaleY = max(0.0f, scaleY - (delta * scaleSpeed));
			scaleZ = max(0.0f, scaleZ - (delta * scaleSpeed));
		}
		break;
		// ctrl + e
	case '\5':
		rotZ += rotSpeed * delta;
		rotZ = fmodf(rotZ, 360.0f);
		break;
	case 'e':
		if (shift) {
			scaleZ += delta * scaleSpeed;
		}
		else {
			scaleX += delta * scaleSpeed;
			scaleY += delta * scaleSpeed;
			scaleZ += delta * scaleSpeed;
		}
		break;
	case 'o':
		ortho = true;
		break;
	case 'p':
		ortho = false;
		break;
	case 'i':
		combi = !combi;
		break;
	case 'u':
		upward = !upward;
		break;
	case 27: // Escape key
		glutDestroyWindow(glutGetWindow());
		exit(0);
		break;
	}

	glutPostRedisplay();
}

void specialKey(int key, int x, int y) {
	GLfloat delta = getDelta();

	switch (key)
	{
	case GLUT_KEY_UP:
		position += direction * delta * cameraSpeed;
		break;
	case GLUT_KEY_DOWN:
		position -= direction * delta * cameraSpeed;
		break;
	case GLUT_KEY_RIGHT:
		position += rightSide * delta * cameraSpeed;
		break;
	case GLUT_KEY_LEFT:
		position -= rightSide * delta * cameraSpeed;
		break;
	}

	glutPostRedisplay();
}

void mouseMove(int x, int y) {

	GLfloat delta = getDelta();

	horizontalAngle += mouseSpeed * delta * float((width / 2) - x);
	verticalAngle += mouseSpeed * delta * float((height / 2) - y);

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	rightSide = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	up = glm::cross(rightSide, direction);

	glutPostRedisplay();
	glutWarpPointer(width / 2, height / 2);
}

void mouseScroll(int button, int dir, int x, int y) {
	GLfloat delta = getDelta();

	if (dir > 0) {
		fov = min(46.0f, fov + (1 * delta));
	}
	else {
		fov = max(44.0f, fov - (1 * delta));
	}

	glutPostRedisplay();
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Hello Triangle");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(specialKey);
	glutPassiveMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseScroll);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
