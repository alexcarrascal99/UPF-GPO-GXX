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
    this->shader = nullptr;
    this->mesh = nullptr;
}

Application::~Application()
{
}

void Application::Init(void)
{
    std::cout << "Cargando Shaders..." << std::endl;
    mesh = new Mesh();
    mesh->CreateQuad();

    // Prueba con la ruta que creas más lógica
   
    shader = Shader::Get("../res/shaders/quad.vs", "../res/shaders/quad.fs");
}

// Render one frame
void Application::Render(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float aspect = (float)window_width / (float)window_height;

    shader->Enable();

    shader->SetUniform1("u_aspect", aspect);
    shader->SetUniform1("u_exercise", (int)current_exercise); // Enviamos 1, 2, 3 o 4
    shader->SetUniform1("u_subtask", current_subtask);        // Enviamos 0, 1, 2...

    mesh->Render();
    shader->Disable();
}

// Called after render
void Application::Update(float seconds_elapsed)
{


}
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    float aspect = (float)window_width / (float)window_height;

    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch (event.keysym.sym) {
    case SDLK_ESCAPE: exit(0); break;

    case SDLK_1: current_exercise = EX1; break;
    case SDLK_2: current_exercise = EX2; break;
    case SDLK_3: current_exercise = EX3; break;
    case SDLK_4: current_exercise = EX4; break;

        // Selección de Subtarea (se aplica al ejercicio que esté activo)
    case SDLK_a: current_subtask = 0; break;
    case SDLK_b: current_subtask = 1; break;
    case SDLK_c: current_subtask = 2; break;
    case SDLK_d: current_subtask = 3; break;
    case SDLK_e: current_subtask = 4; break;
    case SDLK_f: current_subtask = 5; break;
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
    // Aquí puedes añadir la lógica que desees para el evento de mouse down
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