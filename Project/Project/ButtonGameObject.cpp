#include "ButtonGameObject.h"

ButtonGameObject::ButtonGameObject(ButtonType type, glm::vec3& entityPosition, glm::vec3& entityScale, GLint entityNumElements):
    GuiGameObject(entityPosition, entityScale, 0, 0, 0, entityNumElements) {
	switch (type)
	{
    case ButtonType::StartButton:
        texture = startButtonTextureID;
        hoveredTexture = startButtonActiveTextureID;
        clickedTexture = startButtonActiveTextureID;
        break;
    case ButtonType::QuitButton:
        texture = quitButtonTextureID;
        hoveredTexture = quitButtonActiveTextureID;
        clickedTexture = quitButtonActiveTextureID;
        break;
    case ButtonType::ResumeButton:
        texture = resumeButtonTextureID;
        hoveredTexture = resumeButtonActiveTextureID;
        clickedTexture = resumeButtonActiveTextureID;
        break;
    case ButtonType::AbandonButton:
        texture = abandonButtonTextureID;
        hoveredTexture = abandonButtonActiveTextureID;
        clickedTexture = abandonButtonActiveTextureID;
        break;
    default:
        throw "Unknown button type";
	}

    boundingBox = glm::vec2(entityScale);
}

GLuint ButtonGameObject::startButtonTextureID = 0;
GLuint ButtonGameObject::startButtonActiveTextureID = 0;
GLuint ButtonGameObject::quitButtonTextureID = 0;
GLuint ButtonGameObject::quitButtonActiveTextureID = 0;
GLuint ButtonGameObject::resumeButtonTextureID = 0;
GLuint ButtonGameObject::resumeButtonActiveTextureID = 0;
GLuint ButtonGameObject::abandonButtonTextureID = 0;
GLuint ButtonGameObject::abandonButtonActiveTextureID = 0;

int ButtonGameObject::setTextures(void (setFuncPtr)(GLuint w, char* fname, int skipX, int skipY, int rowHeight, int colLength), GLuint* textures, int offset) {
    int rowHeight = 85;
    int colWidth = 370;

    setFuncPtr(textures[offset + 0], "Assets\\gui\\start-button.png", 5, 0, colWidth, rowHeight);
    setFuncPtr(textures[offset + 1], "Assets\\gui\\start-button.png", 5, rowHeight, colWidth, rowHeight);
    startButtonTextureID = textures[offset + 0];
    startButtonActiveTextureID = textures[offset + 1];

    setFuncPtr(textures[offset + 2], "Assets\\gui\\quit-button.png", 5, 0, colWidth, rowHeight);
    setFuncPtr(textures[offset + 3], "Assets\\gui\\quit-button.png", 5, rowHeight, colWidth, rowHeight);
    quitButtonTextureID = textures[offset + 2];
    quitButtonActiveTextureID = textures[offset + 3];

    setFuncPtr(textures[offset + 4], "Assets\\gui\\resume-button.png", 5, 0, colWidth, rowHeight);
    setFuncPtr(textures[offset + 5], "Assets\\gui\\resume-button.png", 5, rowHeight, colWidth, rowHeight);
    resumeButtonTextureID = textures[offset + 4];
    resumeButtonActiveTextureID = textures[offset + 5];

    setFuncPtr(textures[offset + 6], "Assets\\gui\\abandon-button.png", 5, 0, colWidth, rowHeight);
    setFuncPtr(textures[offset + 7], "Assets\\gui\\abandon-button.png", 5, rowHeight, colWidth, rowHeight);
    abandonButtonTextureID = textures[offset + 6];
    abandonButtonActiveTextureID = textures[offset + 7];

    return offset + 8;
}