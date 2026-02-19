#pragma once
#include "mesh.h"
#include "camera.h"
#include "image.h"
#include "texture.h"


class Entity {
public:
	
	

	bool use_texture = false;
	bool show_zbuffer = false;

	Mesh* mesh;
	Matrix44 modelMatrix;
	Image* texture;



	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
	void Entity::Translate(float x, float y, float z);
	void Entity::Update(float seconds);

};