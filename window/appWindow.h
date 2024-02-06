#pragma once

#include <SDL3/SDL.h>

void app();
static int APP_IS_RUNNING;
void process_input();
void update();
void render(SDL_Renderer* renderer);
void destroy_window(SDL_Window* window, SDL_Renderer* renderer);