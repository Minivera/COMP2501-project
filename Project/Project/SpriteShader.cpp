#include "SpriteShader.h"

bool SpriteShader::setupSprite = false;
GLuint SpriteShader::vbo_sprite;
GLuint SpriteShader::ebo_sprite;

SpriteShader::SpriteShader(const char* vertPath, const char* fragPath): Shader(vertPath, fragPath) {
	if (!setupSprite) {
		makeSprite(); // do it only once
	}
}

void SpriteShader::makeSprite()
{
	// The face of the square is defined by four vertices and two triangles

	// Number of attributes for vertices and faces
	//	const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
	//	const int face_att = 3; // Vertex indices (3)

	GLfloat vertex[] = {
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

	// Create buffer for vertices
	glGenBuffers(1, &vbo_sprite);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	// Create buffer for faces (index buffer)
	glGenBuffers(1, &ebo_sprite);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_sprite);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

	setupSprite = true; // finished creating sprite 
}

void SpriteShader::setAttributes() {
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_sprite);

	// Set attributes for shaders
	// Should be consistent with how we created the buffers for the square
	GLint vertex_att = glGetAttribLocation(shaderProgram, "vertex");
	glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);

	GLint color_att = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(color_att);

	GLint tex_att = glGetAttribLocation(shaderProgram, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);
}