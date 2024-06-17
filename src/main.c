#include "display.h"
#include "vector.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Declare an array of vectors/points
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
float fov_factor = 128;

bool is_running = false;

void setup(void) {
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  assert(color_buffer);

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  int point_count = 0;

  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t new_point = {x, y, z};
        cube_points[point_count++] = new_point;
      }
    }
  }
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT: {
    is_running = false;
    break;
  }
  case SDL_KEYDOWN: {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      is_running = false;
    }
    break;
  }
  }
}

vec2_t project(vec3_t point) {
  vec2_t projected_point = {.x = (fov_factor * point.x),
                            .y = (fov_factor * point.y)};
  return projected_point;
}

void update(void) {
  for (int i = 0; i < N_POINTS; ++i) {
    vec3_t point = cube_points[i];

    vec2_t projected_point = project(point);
    projected_points[i] = projected_point;
  }
}

void render(void) {
  // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  // SDL_RenderClear(renderer);
  draw_grid();
  // draw_pixel(20, 20, 0xFFFFFF00);
  // draw_rect(300, 200, 300, 150, 0xFFFF00FF);
  //
  for (int i = 0; i < N_POINTS; ++i) {
    vec2_t projected_point = projected_points[i];
    draw_rect(projected_point.x + (window_width / 2),
              projected_point.y + (window_height / 2), 4, 4, 0xFFFFFF00);
  }
  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
}

int main(void) {
  // Create a sdl window
  is_running = initialize_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }
  destroy_window();
  return 0;
}
