#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

// Some weird thing happens when trying to rezise "by hand", can be solved easily:
// https://stackoverflow.com/questions/32294913/getting-contiunous-window-resize-event-in-sdl-2
static int resizeCallback(void *data, SDL_Event *event)
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_Window *win = SDL_GetWindowFromID(event->window.windowID);
		if (win == (SDL_Window *)data)
		{
			printf("resizing.....\n");
		}
	}
	return 0;
}


int main(int argc, char *argv[])
{
	SDL_Window *sdl_window = nullptr;

	unsigned int window_flags = SDL_WINDOW_OPENGL;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Failed to init SDL Video, error: %s", SDL_GetError());
		return -1;
	}

	int win_width = 640;
	int win_height = 480;
	enum class SCREENSIZE
	{
		is640x480,
		is1366x768,
		fullscreen
	} curr_screen_size = SCREENSIZE::is640x480,
	  last_non_fullscreen_size = SCREENSIZE::is640x480;

	// Create an application window with the following settings:
	// Arguments correspond to: window title, initial horizontal position,
	// initial vertical position, width, height and flags
	sdl_window = SDL_CreateWindow(
		"(F11: fullscreen F10: winresize ESC: quit)",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		win_width,									  // width, in pixels
		win_height,									  // height, in pixels
		window_flags
	);

	// Check that the window was successfully created
	if (sdl_window == nullptr)
	{
		printf("Could not create window: %s", SDL_GetError());
		return -1;
	}

	SDL_AddEventWatch(resizeCallback, sdl_window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GLContext Context = SDL_GL_CreateContext(sdl_window);

	glClearColor(1.f, 0.f, 1.f, 0.f); // background color
	glViewport(0, 0, win_width, win_height);

	bool isRunning = true;
	SDL_Event sdl_event;
	while (isRunning)
	{
		// Checks events, just keyboard for this moment
		while (SDL_PollEvent(&sdl_event) != 0)
		{
			if (sdl_event.type == SDL_QUIT)
			{
				isRunning = false;
			}
			else if (sdl_event.type == SDL_KEYDOWN)
			{
				switch (sdl_event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_F11:
					if (curr_screen_size != SCREENSIZE::fullscreen)
					{
						last_non_fullscreen_size = curr_screen_size;
						curr_screen_size = SCREENSIZE::fullscreen;
						SDL_SetWindowFullscreen(sdl_window, window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
					}
					else
					{
						curr_screen_size = last_non_fullscreen_size;
						SDL_SetWindowFullscreen(sdl_window, window_flags);
					}
					SDL_GetWindowSize(sdl_window, &win_width, &win_height);
					glViewport(0, 0, win_width, win_height);
					break;
				case SDLK_F10:
					switch (curr_screen_size)
					{
					case SCREENSIZE::fullscreen:
						break;
					case SCREENSIZE::is640x480:
						curr_screen_size = SCREENSIZE::is1366x768;
						SDL_SetWindowSize(sdl_window, 1366, 768);
						SDL_GetWindowSize(sdl_window, &win_width, &win_height);
						glViewport(0, 0, win_width, win_height);
						break;
					case SCREENSIZE::is1366x768:
						curr_screen_size = SCREENSIZE::is640x480;
						SDL_SetWindowSize(sdl_window, 640, 480);
						SDL_GetWindowSize(sdl_window, &win_width, &win_height);
						glViewport(0, 0, win_width, win_height);
						break;
					}

					break;
				}
			}

			// Screen "update"

			// Clear screen buffer
			glClear(GL_COLOR_BUFFER_BIT);
			// Put here anything to show now that whe buffer is clear
			// Then "load it"
			SDL_GL_SwapWindow(sdl_window);
		}
	}

	// Never forget the clean up.
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();

	return 0;
}
