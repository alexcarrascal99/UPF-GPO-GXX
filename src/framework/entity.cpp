#include "entity.h"

Entity::Entity() {
	mesh = new Mesh();
	mesh->LoadOBJ("meshes/lee.obj");
	mode = NORMAL;
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
		v0.y = ((v0.y + 1) * 0.5f) * framebuffer->height;

		v1.x = (v1.x + 1) * 0.5f * framebuffer->width;
		v1.y = ( (v1.y + 1) * 0.5f) * framebuffer->height;
		v2.x = (v2.x + 1) * 0.5f * framebuffer->width;
		v2.y = ((v2.y + 1) * 0.5f) * framebuffer->height;


		framebuffer->DrawLineDDA(v0.x, v0.y, v1.x, v1.y, c);
		framebuffer->DrawLineDDA(v1.x, v1.y, v2.x, v2.y, c);
		framebuffer->DrawLineDDA(v2.x, v2.y, v0.x, v0.y, c);




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
	static float t = 0.0f;   // ← CLAVE
	t += seconds;

	if (this->mode == NORMAL) {
		return;
	}

	if (this->mode == ROTATE) {

		Matrix44 R;
		R.MakeRotationMatrix(seconds * 0.8f, Vector3(0, 1, 0));

		Matrix44 T;
		T.MakeTranslationMatrix(sin(t * 5.0f) * 0.01f, 0, 0);

		modelMatrix = modelMatrix * R * T;
	}

	else if (this->mode == TRANSLATE) {

		Matrix44 T;
		T.MakeTranslationMatrix(sin(t * 4.0f) * 0.02f, 0, 0);

		modelMatrix = modelMatrix * T;
	}

	else if (this->mode == SCALE) {

		float s = 1.0f + sin(t * 6.0f) * 0.01f;

		Matrix44 S;
		S.MakeScaleMatrix(s, s, s);

		modelMatrix = modelMatrix * S;
	}
}
