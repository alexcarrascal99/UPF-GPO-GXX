#include "entity.h"

Entity::Entity() {
	mesh = new Mesh();
	mesh->LoadOBJ("meshes/lee.obj");
	modelMatrix.SetIdentity();
}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	for(int i = 0; i < mesh->GetVertices().size(); i+=3){
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

		// De espacio de la camara a espacio de pantalla
		v0.x = (v0.x + 1) * 0.5f * framebuffer->width;
		v0.y = (1 - (v0.y + 1) * 0.5f) * framebuffer->height;
		Vector2 v0_2d = Vector2(v0.x, v0.y);
		v1.x = (v1.x + 1) * 0.5f * framebuffer->width;
		v1.y = (1 - (v1.y + 1) * 0.5f) * framebuffer->height;
		Vector2 v1_2d = Vector2(v1.x, v1.y);
		v2.x = (v2.x + 1) * 0.5f * framebuffer->width;
		v2.y = (1 - (v2.y + 1) * 0.5f) * framebuffer->height;
		Vector2 v2_2d = Vector2(v2.x, v2.y);

		framebuffer->DrawLineDDA(v0_2d.x, v0_2d.y, v1_2d.x, v1_2d.y, c);
		framebuffer->DrawLineDDA(v1_2d.x, v1_2d.y, v2_2d.x, v2_2d.y, c);

	}
}

