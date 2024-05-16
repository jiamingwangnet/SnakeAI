#include "Camera2D.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"

GEngine::Camera2D::Camera2D(GameObject& holder, int scrnWidth, int scrnHeight)
	: Component{ holder },
	  scrnWidth{scrnWidth}, scrnHeight{scrnHeight},
	  cameraMatrix{1.0f}, needUpdate{true}, scale{1.0f},
	  orthoMatrix{glm::ortho(0.0f, (float)scrnWidth, 0.0f, (float)scrnHeight)}
{
	
}

GEngine::Camera2D::~Camera2D()
{}

void GEngine::Camera2D::VInit() 
{
	
}
void GEngine::Camera2D::VUpdate()
{
	Vector2 position = GetHolder().GetPosition();

	Vector3 translation{ -position.x + scrnWidth / 2.0f, -position.y + scrnHeight / 2.0f, 0.0f }; // change coordinates system to have (0, 0) in the center
	cameraMatrix = glm::translate(orthoMatrix, translation);

	Vector3 scaling{ scale, scale, 0.0f };
	cameraMatrix = glm::scale(glm::mat4{ 1.0f }, scaling) * cameraMatrix;
}

void GEngine::Camera2D::VDraw()
{

}

GEngine::Vector2 GEngine::Camera2D::ScreenToWorldCoords(Vector2 screenCoords)
{
	// invert y direction
	screenCoords.y = scrnHeight - screenCoords.y;

	screenCoords -= Vector2{ scrnWidth / 2.0f, scrnHeight / 2.0f }; // change the center of the screen to be (0, 0)
	screenCoords /= scale; // scale coords

	screenCoords += GetHolder().GetPosition();

	return screenCoords;
}