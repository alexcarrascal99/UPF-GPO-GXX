#include "entity.h"

Entity::Entity() {
	mesh = new Mesh();
	mesh->LoadOBJ("meshes/lee.obj");
	mode = NORMAL;
	modelMatrix.SetIdentity();
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

		// De espacio de la camara a espacio de pantalla
		v0.x = (v0.x + 1) * 0.5f * framebuffer->width;
		v0.y = (1.0f - (v0.y + 1) * 0.5f) * framebuffer->height;

		v1.x = (v1.x + 1) * 0.5f * framebuffer->width;
		v1.y = (1.0f - (v1.y + 1) * 0.5f) * framebuffer->height;

		v2.x = (v2.x + 1) * 0.5f * framebuffer->width;
		v2.y = (1.0f - (v2.y + 1) * 0.5f) * framebuffer->height;
		

		v0.z = -v0.z;
		v1.z = -v1.z;
		v2.z = -v2.z;

		// Clipping básico por far/near (bien hecho)
		if (
			(v0.z < -1 && v1.z < -1 && v2.z < -1) || // todos detrás
			(v0.z > 1 && v1.z > 1 && v2.z > 1)    // todos lejos
			)
		{
			continue;
		}




		if (this->render_mode == WIREFRAME) {
			framebuffer->DrawLineDDA(v0.x, v0.y, v1.x, v1.y, Color::WHITE);
			framebuffer->DrawLineDDA(v1.x, v1.y, v2.x, v2.y, Color::WHITE);
			framebuffer->DrawLineDDA(v2.x, v2.y, v0.x, v0.y, Color::WHITE);
		}

		else if (this->render_mode == POINTCLOUD) {
			framebuffer->SetPixel((unsigned int)v0.x, (unsigned int)v0.y, Color::WHITE);
			framebuffer->SetPixel((unsigned int)v1.x, (unsigned int)v1.y, Color::WHITE);
			framebuffer->SetPixel((unsigned int)v2.x, (unsigned int)v2.y, Color::WHITE);
				
		}

		else if (this->render_mode == TRIANGLES) {
			Vector2 p0(v0.x, v0.y);
			Vector2 p1(v1.x, v1.y);
			Vector2 p2(v2.x, v2.y);
			framebuffer->DrawTriangle(p0, p1, p2, Color::WHITE, true, Color::WHITE);
		}

		else if (this->render_mode == TRIANGLES_INTERPOLATED) {
			Vector3 p0(v0.x, v0.y, v0.z);
			Vector3 p1(v1.x, v1.y, v1.z);
			Vector3 p2(v2.x, v2.y, v2.z);

			Vector2 uv0 = mesh->GetUVs()[i];
			Vector2 uv1 = mesh->GetUVs()[i + 1];
			Vector2 uv2 = mesh->GetUVs()[i + 2];
			Image* image = new Image();
			framebuffer->DrawTriangleInterpolated(p0, p1, p2, Color::RED, Color::GREEN, Color::BLUE, zBuffer, texture,  uv0, uv1, uv2);
		}
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