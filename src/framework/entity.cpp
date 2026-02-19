#include "entity.h"
#include "application.h"


Entity::Entity() {
	mesh = new Mesh();
	mesh->LoadOBJ("meshes/lee.obj");
	modelMatrix.SetIdentity();
	texture = NULL;
}


void Entity::Render(Image* framebuffer, Camera* camera, FloatImage* zBuffer) {
	for (int i = 0; i < mesh->GetVertices().size(); i += 3) {
		Vector3 v0 = mesh->GetVertices()[i];
		Vector3 v1 = mesh->GetVertices()[i + 1];
		Vector3 v2 = mesh->GetVertices()[i + 2];

		// De local a espacio del mundo 
		v0 = modelMatrix * v0;
		v1 = modelMatrix * v1;
		v2 = modelMatrix * v2;


		// Del espacio al mundo al espacio de la camara 
		v0 = camera->ProjectVector(v0);
		v1 = camera->ProjectVector(v1);
		v2 = camera->ProjectVector(v2);


		if (v0.z < -1.0f && v1.z < -1.0f && v2.z < -1.0f || v0.z > 1.0f && v1.z > 1.0f && v2.z > 1.0f) {
			continue;
		}


		// De espacio de la camara a espacio de pantalla
		v0.x = (v0.x + 1) * 0.5f * framebuffer->width;
		v0.y = (v0.y + 1) * 0.5f * framebuffer->height;

		v1.x = (v1.x + 1) * 0.5f * framebuffer->width;
		v1.y = (v1.y + 1) * 0.5f * framebuffer->height;

		v2.x = (v2.x + 1) * 0.5f * framebuffer->width;
		v2.y = (v2.y + 1) * 0.5f * framebuffer->height;




		
		framebuffer->DrawLineDDA(v0.x, v0.y, v1.x, v1.y, Color::WHITE);
		framebuffer->DrawLineDDA(v1.x, v1.y, v2.x, v2.y, Color::WHITE);
		framebuffer->DrawLineDDA(v2.x, v2.y, v0.x, v0.y, Color::WHITE);

	}
}

void Entity::Translate(float x, float y, float z)
{
	Matrix44 T;
	T.MakeTranslationMatrix(x, y, z);

	modelMatrix = modelMatrix * T;
}
void Entity::Update(float seconds)
{
	
}