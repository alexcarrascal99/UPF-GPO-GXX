/*
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "button.h"
#include "entity.h"
#include "shader.h"

class ParticleSystem {
public:
	static const int MAX_PARTICLES = 500;
	struct Particle {
		Vector2 position;
		Vector2 velocity;
		Color color;
		float acceleration;
		float ttl;
		bool inactive;
	};

	Particle particles[MAX_PARTICLES];
	void Init();
	void Update(float dt);
	void Render(Image* framebuffer);
};

class Application
{
public:

	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;

	// Input
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame
	bool fill_shapes = false;
	enum Exercise {
		EX1,
		EX2,
		EX3,
		EX4	
	};

	Exercise current_exercise = EX1;

	Mesh* mesh;
	Shader* shader;
	Shader* raster_shader;
	Shader* phong_shader;
	Shader* gouraud_shader;
	
	Shader* current_shader = nullptr;

	sUniformData uniformdata;

	char current_subtask = 'a';

	FloatImage z_buffer;

	Color current_color = Color::WHITE;
	int start_x;
	int start_y;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float distance = 0.0f;
	std::vector<sLight> lights;
	bool dragging = false;
	bool is_second_click = false;
	bool is_painting = false;
	int last_mouse_x = 0;
	int last_mouse_y = 0;
	int borderWidth = 7;

	bool lab5_scene = false;

	bool use_color_texture = true;
	bool use_specular_texture = true;
	bool use_normal_texture = true;

	int active_lights = 2;


	Button all_buttons[16];
	Vector2 triangle_points[3];
	int triangle_click_counter = 0;
	Entity* entity[3];
	Camera* camera;
	Texture* texture;

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init(void);
	void Render(void);
	void Update(float dt);

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport(0, 0, width, height);
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
	}

	Vector2 GetWindowSize()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return Vector2(float(w), float(h));
	}
};