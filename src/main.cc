#include <cstdio>
#include <SDL.h>

int main(int, char **) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  SDL_Quit();
  return 0;
}
