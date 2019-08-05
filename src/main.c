#include "rtv1.h"

void	game_quit(t_main *m)
{
	m->running = false;
	SDL_PixelFormat* pixelFormat = m->screen->format;
	Uint32 pixelFormatEnum = pixelFormat->format;
	const char* surfacePixelFormatName = SDL_GetPixelFormatName(pixelFormatEnum);
	SDL_Log("The surface's pixelformat is %s", surfacePixelFormatName);
	SDL_DestroyWindow(m->window);
	SDL_Quit();
}

void	handle_events(t_main *m, SDL_Event e)
{
	char		c = 0;

	if (e.key.keysym.sym == SDLK_DOWN && ++c)
		m->cam->xang += 2 * M_PI / 180.0;
	else if (e.key.keysym.sym == SDLK_UP && ++c)
		m->cam->xang -= 2 * M_PI / 180.0;
	else if (e.key.keysym.sym == SDLK_RIGHT && ++c)
		m->cam->yang += 2 * M_PI / 180.0;
	else if (e.key.keysym.sym == SDLK_LEFT && ++c)
		m->cam->yang -= 2 * M_PI / 180.0;
	else if (e.key.keysym.sym == SDLK_x && ++c)
		m->cam->zang += 2 * M_PI / 180.0;
	else if (e.key.keysym.sym == SDLK_z && ++c)
			m->cam->zang -= 2 * M_PI / 180.0;
	else if (e.key.keysym.sym == SDLK_KP_4 && ++c)
		(*m->cam->loc)[0] -= 10;
	else if (e.key.keysym.sym == SDLK_KP_6 && ++c)
		(*m->cam->loc)[0] += 10;
	else if (e.key.keysym.sym == SDLK_KP_2 && ++c)
		(*m->cam->loc)[1] -= 10;
	else if (e.key.keysym.sym == SDLK_KP_8 && ++c)
		(*m->cam->loc)[1] += 10;
	else if (e.key.keysym.sym == SDLK_s && ++c)
		(*m->cam->loc)[2] += 10;
	else if (e.key.keysym.sym == SDLK_w && ++c)
		(*m->cam->loc)[2] -= 10;
	m->cam->xcos = cos(m->cam->xang);
	m->cam->xsin = sin(m->cam->xang);
	m->cam->ycos = cos(m->cam->yang);
	m->cam->ysin = sin(m->cam->yang);
	m->cam->zcos = cos(m->cam->zang);
	m->cam->zsin = sin(m->cam->zang);
	(c) ? render(m) : 0;
}

t_cam	*init_cam(t_vec3f *loc, double xang, double yang, double zang)
{
	t_cam	*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (NULL);
	cam->loc = loc;
	cam->xang = xang;
	cam->xcos = cos(xang);
	cam->xsin = sin(xang);
	cam->yang = yang;
	cam->ycos = cos(yang);
	cam->ysin = sin(yang);
	cam->zang = zang;
	cam->zcos = cos(zang);
	cam->zsin = sin(zang);
	cam->focus = FOCUS;
	return (cam);
}

int		main(int argc, char *argv[])
{
	t_main		*m;
	SDL_Event	e;

	if (SDL_Init(SDL_INIT_VIDEO) != 0
		|| !(m = malloc(sizeof(t_main)))
		|| !(m->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, W, H, 0))
		|| !(m->screen = SDL_GetWindowSurface(m->window))
		|| !(m->cam = init_cam(&(t_vec3f){0, 14, 42},
					M_PI,
					0,
					M_PI)))
	{
		//printf("Error\n");
		return (1);
	}
/*	m->objects[0] = new_torus(&(t_vec3f){0.0, 0.0, -50.0},
		(t_vec3f){0.0, 0.0, 1.0}, 20.0, 12.0,
		(SDL_Color){255, 0, 255, 255});
	printf(m->objects[0] ? "Torus ready\n": "Failed with a torus\n");*/
	if (argc < 2)
		return (10);
	m->objects = parse_scene(m, argv[1]);
	m->ray = &(t_vec3f){0, 1, 0};
	m->bpp = m->screen->format->BytesPerPixel;
	SDL_RaiseWindow(m->window);
	m->running = true;
	render(m);
	while (m->running)
	{
		while (SDL_PollEvent(&e))
		{	
			if (e.key.keysym.sym == SDLK_ESCAPE ||
				(e.type == SDL_WINDOWEVENT &&
				e.window.event == SDL_WINDOWEVENT_CLOSE))
				game_quit(m);
			else if (e.type == SDL_KEYDOWN)
				handle_events(m, e);
		}
	}
	exit(0);
	return (0);
}