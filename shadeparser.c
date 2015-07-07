#include "SDL.h"
#include <stdio.h>

int main(int argc, char **argv)
{
  if(SDL_Init(SDL_INIT_VIDEO))
  {
    printf("failed to initialize SDL\n");
    return -1;
  }
  
  SDL_Quit();
  return 0;
}
