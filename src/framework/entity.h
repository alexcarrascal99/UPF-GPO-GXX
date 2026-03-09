#pragma once
#include "mesh.h"
#include "camera.h"
#include "image.h"
#include "shader.h"
#include "texture.h"
#include "material.h"


class Entity {
public:
	
	

	bool use_texture = false;
	bool show_zbuffer = false;

	Mesh* mesh;
	Matrix44 modelMatrix;
	Texture* texture;
	Shader* shader;
	Material* material;



	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
	void Entity::Translate(float x, float y, float z);
	void Entity::Update(float seconds);
	void Entity::Render(Camera* camera);
	void Entity::Render(sUniformData& uniform_data);

};