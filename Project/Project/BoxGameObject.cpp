#include "BoxGameObject.h"

BoxGameObject::BoxGameObject(glm::vec3& entityPosition, glm::vec3& entityScale, GLint entityNumElements) :
	GuiGameObject(entityPosition, entityScale, boxTextureID, hoveredBoxTextureID, completedBoxTextureID, entityNumElements) {}

void BoxGameObject::complete() {
	completed = true;
	texture = completedBoxTextureID;
	hoveredTexture = completedBoxTextureID;
	clickable = false;
}

GLuint BoxGameObject::boxTextureID = 0;
GLuint BoxGameObject::hoveredBoxTextureID = 0;
GLuint BoxGameObject::completedBoxTextureID = 0;

int BoxGameObject::setTextures(void (setFuncPtr)(GLuint w, const char* fname, int skipX, int skipY, int rowHeight, int colLength), GLuint* textures, int offset) {
    int rowHeight = 130;
    int colWidth = 134;

    setFuncPtr(textures[offset + 0], "Assets\\gui\\upgrade-cell.png", 0, 0, colWidth, rowHeight);
    setFuncPtr(textures[offset + 1], "Assets\\gui\\upgrade-cell.png", colWidth, 0, colWidth, rowHeight);
    setFuncPtr(textures[offset + 2], "Assets\\gui\\upgrade-cell.png", colWidth * 2, 0, colWidth, rowHeight);
    boxTextureID = textures[offset + 0];
    hoveredBoxTextureID = textures[offset + 1];
    completedBoxTextureID = textures[offset + 2];

    return offset + 3;
}