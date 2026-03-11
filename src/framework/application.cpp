#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
    this->window = createWindow(caption, width, height);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);

    this->mouse_state = 0;
    this->time = 0.f;
    this->window_width = w;
    this->window_height = h;
    this->keystate = SDL_GetKeyboardState(nullptr);
    this->framebuffer.Resize(w, h);
    this->camera = new Camera(); 
    this->current_exercise = EX1;
	this->current_subtask = 0;
    this->shader = nullptr;
    this->mesh = nullptr;
}

Application::~Application()
{
}

void Application::Init(void)
{
    float aspect = (float)window_width / (float)window_height;
    camera->LookAt(Vector3(0, 0, 2.0f), Vector3(0, 0.2f, 0), Vector3(0, 1, 0));
    camera->SetPerspective(45, aspect, 0.05f, 3.0f);

    mesh = new Mesh();
    mesh->CreateQuad();
    shader = Shader::Get("../res/shaders/quad.vs", "../res/shaders/quad.fs");
    raster_shader = Shader::Get("../res/shaders/raster.vs", "../res/shaders/raster.fs");
	texture = Texture::Get("../res/images/fruits.png");

   gouraud_shader = Shader::Get("../res/shaders/gourand.vs", "../res/shaders/gourand.fs");
   phong_shader = Shader::Get("../res/shaders/phong.vs", "../res/shaders/phong.fs");
   current_shader = gouraud_shader;


    entity[0] = new Entity();
    entity[0]->material->shader = raster_shader;
    entity[0]->material->color_texture = Texture::Get("../res/textures/lee_color_specular.tga");
    entity[0]->material->normal_texture = Texture::Get("../res/textures/lee_normal.tga");

    lights.clear();

    sLight light1;
    light1.position = Vector3(-3.0f, 3.0f, 3.0f);   // izquierda arriba
    light1.color = Vector3(1.0f, 1.0f, 1.0f);

    sLight light2;
    light2.position = Vector3(3.0f, -3.0f, 3.0f);   // derecha abajo
    light2.color = Vector3(1.0f, 1.0f, 1.0f);

    sLight light3;
    light3.position = Vector3(0.0f, 4.0f, 3.0f);
    light3.color = Vector3(0.0f, 1.0f, 0.0f);

    sLight light4;
    light4.position = Vector3(0.0f, -4.0f, 3.0f);
    light4.color = Vector3(0.0f, 0.0f, 1.0f);

    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);
    lights.push_back(light4);

    uniformdata.ambient_light = Vector3(0.2f, 0.2f, 0.2f);

    uniformdata.light.position = Vector3(2.0f, 2.0f, 2.0f);
    uniformdata.light.color = Vector3(1.0f, 1.0f, 1.0f);
}



// Render one frame
void Application::Render(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    float aspect = (float)window_width / (float)window_height;

 
    if (lab5_scene) {
        uniformdata.use_color_texture = use_color_texture;
        uniformdata.use_specular_texture = use_specular_texture;
        uniformdata.use_normal_texture = use_normal_texture;
        uniformdata.viewprojection = camera->viewprojection_matrix;
        uniformdata.camera_position = camera->eye;
		entity[0]->material->shader = current_shader;
        if (current_shader == phong_shader)
        {
            int num_lights = active_lights;
            if (num_lights > lights.size())
                num_lights = lights.size();

            for (int i = 0; i < num_lights; ++i)
            {
                uniformdata.light = lights[i];
                uniformdata.first_pass = (i == 0);

                if (i == 0)
                {
                    glDisable(GL_BLEND);
                    glDepthMask(GL_TRUE);
                    glDepthFunc(GL_LESS);
                }
                else
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_ONE, GL_ONE);
                    glDepthMask(GL_FALSE);
                    glDepthFunc(GL_EQUAL);
                }

                entity[0]->Render(uniformdata);
            }
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
		}
		else {
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);

            if (!lights.empty())
                uniformdata.light = lights[0];

            uniformdata.first_pass = true;
            entity[0]->Render(uniformdata);
		}
    }
    else {
        shader->Enable();
        shader->SetTexture("u_texture", texture);
        shader->SetUniform1("u_aspect", aspect);
        shader->SetUniform1("u_exercise", (int)current_exercise);
        shader->SetFloat("u_time", time);
        shader->SetUniform1("u_subtask", current_subtask);
        mesh->Render();
        shader->Disable();
    }

}

// Called after render
void Application::Update(float seconds_elapsed)
{
	time += seconds_elapsed;


}
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    float aspect = (float)window_width / (float)window_height;

    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch (event.keysym.sym)
    {
    case SDLK_ESCAPE:
        exit(0);
        break;

    case SDLK_l:
        lab5_scene = !lab5_scene;
        return;

    case SDLK_g:
        current_shader = gouraud_shader;
        return;

    case SDLK_p:
        current_shader = phong_shader;
        return;
    }

    if (!lab5_scene)
    {
        switch (event.keysym.sym)
        {
        case SDLK_1: current_exercise = EX1; break;
        case SDLK_2: current_exercise = EX2; break;
        case SDLK_3: current_exercise = EX3; break;
        case SDLK_4: current_exercise = EX4; break;

        case SDLK_a: current_subtask = 0; break;
        case SDLK_b: current_subtask = 1; break;
        case SDLK_c: current_subtask = 2; break;
        case SDLK_d: current_subtask = 3; break;
        case SDLK_e: current_subtask = 4; break;
        case SDLK_f: current_subtask = 5; break;
        }
    }
    else
    {
        switch (event.keysym.sym)
        {
        case SDLK_c:
            use_color_texture = !use_color_texture;
            break;

        case SDLK_s:
            use_specular_texture = !use_specular_texture;
            break;

        case SDLK_n:
            use_normal_texture = !use_normal_texture;
            break;

        case SDLK_1:
            active_lights = 1;
            break;

        case SDLK_2:
            active_lights = 2;
            break;

        case SDLK_3:
            active_lights = 3;
            break;

        case SDLK_4:
            active_lights = 4;
            break;
        }
    }
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
    if (event.button != SDL_BUTTON_LEFT) return;

    int current_y = window_height - event.y;
    Vector2 mouse_pos((float)event.x, (float)current_y);
    if (current_y < 40) return;

    Vector3 d = camera->eye - camera->center;
    distance = d.Length();
    yaw = atan2(d.x, d.z);
    pitch = asin(d.y / distance);
    dragging = true;
    last_mouse_x = event.x;
    last_mouse_y = event.y;
}

void Application::OnMouseButtonUp(SDL_MouseButtonEvent event)
{
    if (event.button == SDL_BUTTON_LEFT) {
        is_painting = false;
        dragging = false;
    }
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if (!dragging) return;

    float dx = (event.x - last_mouse_x) * 0.002f;
    float dy = (event.y - last_mouse_y) * 0.002f;

    last_mouse_x = event.x;
    last_mouse_y = event.y;

    yaw -= dx;
    pitch += dy;

    if (pitch > 1.4f) pitch = 1.2f;
    if (pitch < -1.2f) pitch = -1.2f;

    Vector3 pos;

    pos.x = distance * cos(pitch) * sin(yaw);
    pos.y = distance * sin(pitch);
    pos.z = distance * cos(pitch) * cos(yaw);

    camera->eye = camera->center + pos;
    camera->UpdateViewMatrix();
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
    float zoomSpeed = 1.0f;

    if (event.y > 0) {
        camera->fov -= zoomSpeed;
    }

    else if (event.y < 0) {
        camera->fov += zoomSpeed;
    }

    if (camera->fov < 5.0f) camera->fov = 5.0f;
    if (camera->fov > 120.0f) camera->fov = 120.0f;

    camera->UpdateProjectionMatrix();

    // ...
}

void Application::OnFileChanged(const char* filename)
{
    Shader::ReloadSingleShader(filename);
}