#include "entity.h"

Entity::Entity() {
	mesh = new Mesh();
	mesh->LoadOBJ(“meshes/lee.obj”);
}


void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	for(int i = 0; i < mesh.GetVertices().size(); i+=3){
		Vector3 v0 = mesh.GetVertices()[i];
		Vector3 v1 = mesh.GetVertices()[i + 1];
		Vector3 v2 = mesh.GetVertices()[i + 2];

		// De local a espacio del mundo
		v0 = modelMatrix * v0;
		v1 = modelMatrix * v1;
		v2 = modelMatrix * v2;

		// Del espacio al mundo al espacio de la camara
		v0 = camera.ProjectVector(v0);
		v1 = camera.ProjectVector(v1);
		v2 = camera.ProjectVector(v2);

		// Espacio de la camara al de la pantalla
		v0 = camera.viewport_matrix * v0;
		v1 = camera.viewport_matrix * v1;
		v2 = camera.viewport_matrix * v2;

		if (v0.z < 0 || v1.z < 0 || v2.z < 0) {
			framebuffer->DrawTriangle(v0, v1, v2, c, true, c);
		}
	}
}
