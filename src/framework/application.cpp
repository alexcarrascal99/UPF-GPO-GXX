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
}

Application::~Application()
{
}

void Application::Init(void)
{
    std::cout << "Initiating app..." << std::endl;

    camera = new Camera();

    for (int i = 0; i < 3; i++) {
        entity[i] = new Entity();
    }
    entity[0]->Translate(0.5, 0, 0.5);
    entity[2]->Translate(-0.9, 0, 2);


    float aspect = (float)window_width / (float)window_height;
    camera->SetPerspective(45.0f, aspect, 0.01f, 100.0f);
    camera->LookAt(Vector3(0, 0, 90), Vector3(0, 0, 0), Vector3(0, 1, 0));
    camera->UpdateViewMatrix();
    camera->UpdateProjectionMatrix();
    camera->UpdateViewProjectionMatrix();

    Vector3 dir = camera->eye - camera->center;

    distance = dir.Length();

    yaw = atan2(dir.x, dir.z);
    pitch = asin(dir.y / distance);



	z_buffer = FloatImage(window_width, window_height);
	z_buffer.Fill(1.0f);

    current_color = Color::WHITE;

}



// Render one frame
void Application::Render(void)
    {
    framebuffer.Fill(Color::BLACK);

    z_buffer.Fill(10000.0f);

    entity[1]->Render(&framebuffer, camera, &z_buffer);


    if (current_scene == SINGLE_ENTITY) {
        entity[1]->Render(&framebuffer, camera, &z_buffer);
        entity[1]->mode = Entity::NORMAL;
    }

    if (current_scene == MULTIPLE_ENTITIES) {
        entity[0]->Render(&framebuffer, camera, &z_buffer);
        entity[1]->Render(&framebuffer, camera, &z_buffer);
        entity[2]->Render(&framebuffer, camera, &z_buffer);
        
        entity[0]->mode = Entity::ROTATE;
        entity[1]->mode = Entity::TRANSLATE;
        entity[2]->mode = Entity::SCALE;
    }

    framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{
    // mode 1: rota, mode 2: trasllada,
    for (int i = 0; i < 3; i++) {
        entity[i]->Update(seconds_elapsed);
    }

}
void Application::OnKeyPressed(SDL_KeyboardEvent event)
{
    float aspect = (float)window_width / (float)window_height;

    // KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch (event.keysym.sym) {
    case SDLK_ESCAPE: exit(0); break;

    case SDLK_1: // single entity mode
        current_scene = SINGLE_ENTITY;
        break;

    case SDLK_2: // multiples entities mode
        current_scene = MULTIPLE_ENTITIES;
        break;

        // camera near / camera far / fov

    case SDLK_n:
        current_property = CAM_NEAR;
        break;

    case SDLK_f:
        current_property = CAM_FAR;
        break;

    case SDLK_v:
        current_property = FOV;
        break;

        // switches between texture, triangles and triangles interpolated

    case SDLK_t:
        entity[0]->use_texture = !entity[0]->use_texture;
        entity[1]->use_texture = !entity[1]->use_texture;
        entity[2]->use_texture = !entity[2]->use_texture;
        break;

    case SDLK_z:
        entity[0]->show_zbuffer = !entity[0]->show_zbuffer;
        entity[1]->show_zbuffer = !entity[1]->show_zbuffer;
        entity[2]->show_zbuffer = !entity[2]->show_zbuffer;
        break;

    case SDLK_c:
        for (int i = 0; i < 3; i++) {
                entity[i]->render_mode = Entity::TRIANGLES;
            } 
        break;

    case SDLK_i :
    for (int i = 0; i < 3; i++) {
        entity[i]->render_mode = Entity::TRIANGLES_INTERPOLATED;
    }
    break;

    case SDLK_w:
        entity[0]->render_mode = Entity::WIREFRAME;
        entity[1]->render_mode = Entity::WIREFRAME;
        entity[2]->render_mode = Entity::WIREFRAME;
        break;

    case SDLK_p:
        entity[0]->render_mode = Entity::POINTCLOUD;
        entity[1]->render_mode = Entity::POINTCLOUD;
        entity[2]->render_mode = Entity::POINTCLOUD;
        break;

    // increase current property
    case SDLK_PLUS:
    case SDLK_KP_PLUS:
        if (current_property == CAM_NEAR) {
            camera->near_plane -= 0.0005f;
        }
        else if (current_property == CAM_FAR) {
            camera->far_plane += 2.0f;
        }
        else if (current_property == FOV) {
            camera->fov -= 2.0f;
        }
        camera->SetPerspective(camera->fov, aspect, camera->near_plane, camera->far_plane);
        break;

    // decrease current property
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        if (current_property == CAM_NEAR) {
            camera->near_plane += 0.0005f;
        }
        else if (current_property == CAM_FAR) {
            camera->far_plane -= 2.0f;
        }
        else if (current_property == FOV) {
            camera->fov += 2.0f;
        }
        camera->SetPerspective(camera->fov, aspect, camera->near_plane, camera->far_plane);
        break;
    }
}

void Application::OnMouseButtonDown(SDL_MouseButtonEvent event)
{
    if (event.button != SDL_BUTTON_LEFT) return;

    int current_y = window_height - event.y;
    Vector2 mouse_pos((float)event.x, (float)current_y);
    if (current_y < 40) return;

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

    float dx = (event.x - last_mouse_x) * 0.001f;
    float dy = (event.y - last_mouse_y) * 0.004f;

    last_mouse_x = event.x;
    last_mouse_y = event.y;

    yaw += dx;
    pitch += dy;

    if (pitch > 1.4f) pitch = 1.4f;
    if (pitch < -1.4f) pitch = -1.4f;

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

