#include "../headerFiles/stb_image.h"
#include "../headerFiles/stb_image_resize2.h"
#include "../headerFiles/ascii_generation.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  struct Config config;
  config.path = "";
  config.size = -1;
  config.help = false;
  setupConfig(&config, argc, argv);

  if(config.help){
      printf("%s\n", "\n-i -> Specifies the image to use.\n-w [optional] -> Specifies the width of the console output (default is 300 pixels). The height uses the same proportions as the original image.");
      return 0;
  }

  if (argc % 2 == 0) {
    printf("%s\n", "ERROR: There is an empty argument\n");
    return 0;
  }

  if (argc == 1 || strcmp(config.path, "") == 0) {
    printf("%s\n", "ERROR: You must at least specify the path to the image\n");
    return 0;
  }

  int width, height, channels;
  unsigned char *data = stbi_load(config.path, &width, &height, &channels, 0);

   if (!data) {
     fprintf(stderr, ": %s\n", config.path);
     return 1;
   }

    int newWidth = config.size;
  if (newWidth == -1) {
    newWidth = 300;
  }

  float ratio = (float)width / (float)newWidth;
  int newHeight = 0;
  if (ratio < 1) {
    newHeight = (int)ceil((float)height / ((float)newWidth / (float)width));
  } else {
    newHeight = (int)ceil((float)height / ratio);
  }

  unsigned char* dataResized = resizeImage(data,width, height, newWidth, newHeight, channels);

  if (argc % 2 == 0) {
    printf("%s\\n", "ERROR: There is an empty argument\\n");
    return 0;
  }

  char *coloredAsciiArt = generateAsciiArt(dataResized, newWidth, newHeight, channels);
  if (coloredAsciiArt == NULL) {
      return 1; // Indicate error
  }

  printf("%s", coloredAsciiArt);

  free(coloredAsciiArt);
  return 0;
}
