#include "Shader.h"

Shader::Shader(const char *vertPath, const char *fragPath) {
	// Load vertex program source code
	std::string vp = FileUtils::LoadTextFile(vertPath);
	const char *source_vp = vp.c_str();
	std::string fp = FileUtils::LoadTextFile(fragPath);
	const char *source_fp = fp.c_str();


	// Create a shader from vertex program source code
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &source_vp, NULL);
	glCompileShader(vs);

	// Check if shader compiled successfully
	GLint status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
	}

	// Create a shader from the fragment program source code
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &source_fp, NULL);
	glCompileShader(fs);

	// Check if shader compiled successfully
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fs, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
	}

	// Create a shader program linking both vertex and fragment shaders
	// together
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	// Check if shaders were linked successfully
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, buffer);
		throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
	}

	// Delete memory used by shaders, since they were already compiled
	// and linked
	glDeleteShader(vs);
	glDeleteShader(fs); 
	
	setAttributes();
}

void Shader::setAttributes() {}

// Sets a uniform integer variable in your shader program to a value
void Shader::setUniform1i(const GLchar *name, int value) {
	glUniform1i(glGetUniformLocation(shaderProgram, name), value);
}

// Sets a uniform float variable in your shader program to a value
void Shader::setUniform1f(const GLchar *name, float value) {
	glUniform1f(glGetUniformLocation(shaderProgram, name), value);
}

// Sets a uniform vector2 variable in your shader program to a vector
void Shader::setUniform2f(const GLchar *name, const glm::vec2 &vector) {
	glUniform2f(glGetUniformLocation(shaderProgram, name), vector.x, vector.y);
}

// Sets a uniform vector3 variable in your shader program to a vector
void Shader::setUniform3f(const GLchar *name, const glm::vec3 &vector) {
	glUniform3f(glGetUniformLocation(shaderProgram, name), vector.x, vector.y, vector.z);
}

// Sets a uniform vector4 variable in your shader program to a vector
void Shader::setUniform4f(const GLchar *name, const glm::vec4 &vector) {
	glUniform4f(glGetUniformLocation(shaderProgram, name), vector.x, vector.y, vector.z, vector.w);
}

// Sets a uniform matrix4x4 variable in your shader program to a matrix4x4
void Shader::setUniformMat4(const GLchar *name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

Shader::~Shader() {
	glDeleteProgram(shaderProgram);
}

void Shader::enable() {
	glUseProgram(shaderProgram);
}

void Shader::disable() {
	glUseProgram(0);
}