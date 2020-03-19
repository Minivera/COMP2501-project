#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#define GLEW_STATIC
#include <GL/glew.h> // window management library
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //
#include <SOIL/SOIL.h> // read image file
#include <chrono>
#include <thread>

#include "Window.h"
#include "Game.h"
#include "LevelState.h"
#include "GameObject.h"
#include "PlayerGameObject.h"
#include "TerrainGameObject.h"
#include "EnemyGameObject.h"
#include "WeaponGameObject.h"
#include "TreasureGameObject.h"
#include "ExitGameObject.h"
#include "ElectricityGameObject.h"
#include "SpriteShader.h"
#include "LaserShader.h"
#include "ParticleShader.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
	std::cerr << exception_object.what() << std::endl

// Globals that define the OpenGL window and viewport
const std::string window_title_g = "COMP2501 Project";
const unsigned int textures_count = 50;
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;

// Create the geometry for a square (with two triangles)
// Return the number of array elements that form the square
int CreateSquare(void) {
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[]  = {
		//  square (two triangles)
		   //  Position      Color             Texcoords
		-0.5f, 0.5f,	 1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // Top-left
		0.5f, 0.5f,		 0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // Top-right
		0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f,	 1.0f, 1.0f, 1.0f,		0.0f, 1.0f  // Bottom-left
	};


	GLuint face[] = {
		0, 1, 2, // t1
		2, 3, 0  //t2
	};

	GLuint vbo, ebo;

	// Create buffer for vertices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	// Return number of elements in array buffer (6 in this case)
	return sizeof(face) / sizeof(GLuint);
}

void setthisTexture(GLuint w, char *fname) {
	glBindTexture(GL_TEXTURE_2D, w);

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	// Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void setallTexture(GLuint* textures) {
	glGenTextures(textures_count, textures);
	int offset = LevelState::setTextures(setthisTexture, textures, 0);
	offset = GameObject::setTextures(setthisTexture, textures, offset);
	offset = PlayerGameObject::setTextures(setthisTexture, textures, offset);
	offset = WeaponGameObject::setTextures(setthisTexture, textures, offset);
	offset = TreasureGameObject::setTextures(setthisTexture, textures, offset);
	offset = EnemyGameObject::setTextures(setthisTexture, textures, offset);
	offset = TerrainGameObject::setTextures(setthisTexture, textures, offset);
	offset = ExitGameObject::setTextures(setthisTexture, textures, offset);
	offset = ElectricityGameObject::setTextures(setthisTexture, textures, offset);
}

// Main function that builds and runs the game
int main(void){
	GLuint* textures = new GLuint[textures_count];

	try {
		// Setup window
		Window window(window_width_g, window_height_g, window_title_g);
		SpriteShader spriteShader("shader.vert", "shader.frag");
		ParticleShader particleShader("particles.vert", "particles.frag");
		LaserShader laserShader("laser.vert", "laser.frag");
		spriteShader.setAttributes();
		spriteShader.enable();

		// Set up z-buffer for rendering
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable Alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Set up the textures
		setallTexture(textures);

		Game game(textures);

		// Run the main loop
		double lastTime = glfwGetTime();
		bool running = true;
		while (running && !glfwWindowShouldClose(window.getWindow())) {
			// Calculate delta time
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			running = game.loop(window, spriteShader, particleShader, laserShader, deltaTime);
		}
	}
	catch (std::exception &e){
		// print exception and sleep so error can be read
		PrintException(e);
		std::this_thread::sleep_for(std::chrono::milliseconds(100000));
	}

	delete[] textures;
	return 0;
}
