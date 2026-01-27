#pragma once
#include "mesh.h"
#include "camera.h"


class Entity {
public:
	Mesh mesh;	
	Matrix44 modelMatrix;

	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& Sc);
}


