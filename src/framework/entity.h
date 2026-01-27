#pragma once
#include "mesh.h"
#include "camera.h"
#include "image.h"


class Entity {
public:
	Mesh* mesh;
	Matrix44 modelMatrix;

	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& Sc);
};
