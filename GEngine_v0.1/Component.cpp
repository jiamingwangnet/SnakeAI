#include "Component.h"

GEngine::Component::Component(GameObject& holder)
	: holder{holder}
{}

GEngine::Component::~Component()
{}

void GEngine::Component::Init()
{
	VInit();
}

void GEngine::Component::Update()
{
	VUpdate();
}

void GEngine::Component::Draw()
{
	VDraw();
}