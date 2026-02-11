#pragma once
#include "mesh.h"
#include "camera.h"
#include "image.h"
#include "texture.h"


class Entity {
public:
	enum Mode {
		NORMAL,
		ROTATE,
		TRANSLATE,
		SCALE

	};

	enum eRenderMode {
		POINTCLOUD,
		WIREFRAME,
		TRIANGLES,
		TRIANGLES_INTERPOLATED
	};

	eRenderMode render_mode = WIREFRAME;

	bool use_texture = false;
	bool show_zbuffer = false;
	
	Mesh* mesh;
	Matrix44 modelMatrix;
	Mode mode;
	Image* texture;

	

	Entity();
	void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
	void Entity::Translate(float x, float y, float z);
	void Entity::Update(float seconds);

};