#pragma once
#include "mesh.h"
#include "camera.h"
#include "image.h"


class Entity {
public:
	enum Mode {
		NORMAL,
		ROTATE,
		TRANSLATE,
		SCALE

	};
	Mesh* mesh;
	Matrix44 modelMatrix;
	Mode mode;

	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, const Color& Sc);
	void Entity::Translate(float x, float y, float z);
	void Entity::Update(float seconds);

};

