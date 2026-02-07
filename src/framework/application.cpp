#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

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
    entity[0]->Translate(-0.5, 0, 2);
    entity[2]->Translate(-0.5, 0, 2);

    
	current_color = Color::WHITE;


}



// Render one frame
void Application::Render(void)
{
	framebuffer.Fill(Color::BLACK);
    if (current_mode == PARTICLES_MODE) {
        framebuffer.Fill(Color::BLACK);
        p_system->Render(&framebuffer);
    }

	entity[0]->Render(&framebuffer, camera, Color::WHITE);
	entity[1]->Render(&framebuffer, camera, Color::RED);
	entity[2]->Render(&framebuffer, camera, Color::BLUE);

	entity[0]->mode = Entity::ROTATE;
	entity[1]->mode = Entity::TRANSLATE;
	entity[2]->mode = Entity::SCALE;

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
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
    switch (event.keysym.sym) {
    case SDLK_ESCAPE: exit(0); break;

    case SDLK_1: // geometry mode
        current_mode = PAINT;
        current_task = LINE; 
        framebuffer.Fill(Color::BLACK);
        break;

    case SDLK_2: // particle mode
        current_mode = PARTICLES_MODE;
        p_system->Init();
        break;

    case SDLK_l: if (current_mode == PAINT) current_task = LINE; break;
    case SDLK_r: if (current_mode == PAINT) current_task = RECT; break;
    case SDLK_t: if (current_mode == PAINT) current_task = TRIANGLE; break;
    case SDLK_p: if (current_mode == PAINT) current_task = PAINT_MODE; break;
	case SDLK_f: fill_shapes = !fill_shapes; break;

    case SDLK_PLUS:
    case SDLK_KP_PLUS: 
        borderWidth++;
        break;

    case SDLK_MINUS:
    case SDLK_KP_MINUS: 
        if (borderWidth > 1) {
            borderWidth--;
        }
        
        break;

    case SDLK_c: framebuffer.Fill(Color::BLACK); break;
    }

}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
    if (event.button != SDL_BUTTON_LEFT) return;

    int current_y = window_height - event.y;

    // Zona de botones
    for (int i = 0; i < 16; ++i)
    {
        if (all_buttons[i].IsMouseInside(Vector2(event.x, current_y)))
        {
            switch (all_buttons[i].type)
            {
            case BTN_PENCIL:   current_mode = PAINT; current_task = PAINT_MODE; break;
            case BTN_ERASER:   current_mode = PAINT; current_task = ERASER; break;
            case BTN_LINE:     current_mode = PAINT; current_task = LINE; break;
            case BTN_RECT:     current_mode = PAINT; current_task = RECT; break;
            case BTN_TRIANGLE: current_mode = PAINT; current_task = TRIANGLE; break;
            case BTN_CLEAR:    framebuffer.Fill(Color::BLACK); break;
            case BTN_LOAD:     framebuffer.LoadTGA("output.tga", true); break;
            case BTN_SAVE:     framebuffer.SaveTGA("output.tga"); break;

            case BTN_COLOR_BLACK:  current_color = Color::BLACK; break;
            case BTN_COLOR_WHITE:  current_color = Color::WHITE; break;
            case BTN_COLOR_RED:    current_color = Color::RED; break;
            case BTN_COLOR_GREEN:  current_color = Color::GREEN; break;
            case BTN_COLOR_BLUE:   current_color = Color::BLUE; break;
            case BTN_COLOR_YELLOW: current_color = Color::YELLOW; break;
            case BTN_COLOR_CYAN:   current_color = Color::CYAN; break;
            case BTN_COLOR_PINK:   current_color = Color::PURPLE; break;
            }

            return; 
        }
    }
    Vector2 mouse_pos((float)event.x, (float)current_y);
    if (current_y < 40) return;
    switch (current_task)
    {
    case LINE:
        if (!is_second_click) {
            start_x = event.x;
            start_y = current_y;
            is_second_click = true;
        }
        else {
            framebuffer.DrawLineDDA(start_x, start_y, event.x, current_y, current_color);
            is_second_click = false;
        }
        break;

    case RECT:
        if (!is_second_click) {
            start_x = event.x;
            start_y = current_y;
            is_second_click = true;
        }
        else {
            int x = std::min(start_x, (int)event.x);
            int y = std::min(start_y, current_y);
            int w = std::abs((int)event.x - start_x);
            int h = std::abs(current_y - start_y);
            framebuffer.DrawRect(x, y, w, h, current_color, borderWidth, fill_shapes, current_color);
            is_second_click = false;
        }
        break;

    case TRIANGLE:
        // para que el triángulo use su propio contador de clics

        triangle_points[triangle_click_counter] = mouse_pos;
        triangle_click_counter++;

        if (triangle_click_counter == 3) {
            framebuffer.DrawTriangle(triangle_points[0], triangle_points[1], triangle_points[2],current_color, fill_shapes, Color::WHITE);
            triangle_click_counter = 0; 
        }
        break;

    case ERASER:
    {
        is_painting = true;
        last_mouse_x = event.x;
        last_mouse_y = current_y;

        int grosor = 10;

        for (int i = -grosor; i <= grosor; i++)
        {
            framebuffer.DrawLineDDA(
                last_mouse_x + i, last_mouse_y,
                event.x + i, current_y,
                Color::BLACK
            );
        }
    }
    break;



    case PAINT_MODE:
        
        is_painting = true;
        last_mouse_x = event.x;
        last_mouse_y = current_y;
        framebuffer.SetPixel(event.x, current_y, current_color);
        break;

    }
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
        is_painting = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
    if ((current_task == PAINT_MODE) && is_painting) {
        int current_y = window_height - event.y;
        framebuffer.DrawLineDDA(last_mouse_x, last_mouse_y, event.x, current_y,  current_color);
    }
    else if (current_task == ERASER && is_painting) {
        int current_y = window_height - event.y;

        int grosor = 10;
        for (int i = -grosor; i <= grosor; i++)
        {
            framebuffer.DrawLineDDA(last_mouse_x + i, last_mouse_y,event.x + i, current_y, Color::BLACK);
        }

    }
    last_mouse_x = event.x;
    last_mouse_y = window_height - event.y;
	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}

void ParticleSystem::Init() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].position = Vector2((float)(rand() % 1300), (float)(rand() % 1200));
        particles[i].velocity = Vector2(0, (float)((rand() % 100) * -1));
        particles[i].color = Color::WHITE;
        particles[i].acceleration = 9.8f;
        particles[i].ttl = (rand() % 300) / 100.0f + 1.0f;
        particles[i].inactive = false;
    }
}

void ParticleSystem::Update(float dt) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].inactive) {
            particles[i].position = Vector2((float)(rand() % 1200), (float)(rand() % 1200));
            particles[i].velocity = Vector2(0, (float)((rand() % 100) * -1));
            particles[i].ttl = (rand() % 300);
            particles[i].inactive = false;
            continue;
        }

        particles[i].position.x += particles[i].velocity.x * dt;
        particles[i].position.y += particles[i].velocity.y * dt;

        particles[i].ttl -= dt;

        if (particles[i].ttl <= 0) {
            particles[i].inactive = true;
        }
    }
}

void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particles[i].inactive) {
            framebuffer->SetPixel((int)particles[i].position.x, (int)particles[i].position.y, particles[i].color);
        }
    }
}