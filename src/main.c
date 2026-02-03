#include "../headerFiles/stb_image.h"
#include "../headerFiles/stb_image_resize2.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char asciiLetters[] = " .-=+*x#$&X@";

const int colorChannelValues[6] = {0, 95, 135, 175, 215, 255};

const int arrayLength = sizeof(asciiLetters) - 1;

struct Config {
  char *path;
  int size;
  char *output;
  bool help;
};

int average(int a, int b) { return floor((float)(a + b) / 2); }

unsigned char *halfHeight(unsigned char *data, int width, int height,
                          int channels) {
  unsigned char *data2 = malloc((width * floor((float)height / 2) * channels) *
                                sizeof(unsigned char));

  for (int y = 0; y < height - 1; y += 2) {
    for (int x = 0; x < width; x++) {
      int pixel = (y * (width) + x) * channels;
      int pixelBelow = ((y + 1) * (width) + x) * channels;
      int newPixel = ((y / 2) * (width) + x) * channels;

      data2[newPixel + 0] = average(data[pixel + 0], data[pixelBelow + 0]);
      data2[newPixel + 1] = average(data[pixel + 1], data[pixelBelow + 1]);
      data2[newPixel + 2] = average(data[pixel + 2], data[pixelBelow + 2]);
      if (channels == 4) {
        data2[newPixel + 3] = average(data[pixel + 3], data[pixelBelow + 3]);
      }
    }
  }
  return data2;
}

void setupConfig(struct Config *config, int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if(strcmp(argv[i],"-help") == 0){
        config->help = true;
        return;
    }
    if (strcmp(argv[i], "-i") == 0) {
      config->path = argv[i + 1];
      i++;
      continue;
    }
    if (strcmp(argv[i], "-w") == 0) {
      config->size = atoi(argv[i + 1]);
      i++;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0) {
      config->output = argv[i + 1];
      i++;
      continue;
    }
  }
}

bool isGrayShade(int r, int g, int b) {
  short offset = 5;
  if (abs(r - g) <= offset && abs(r - b) <= offset && abs(g - b) <= offset) {
    return true;
  }
  return false;
}

// converts the rgb value from 0-255 to 0-5
int clampChannel(int c) { return ceil(((double)c * 5) / 255); }

int getColor(int r, int g, int b) {
  if (isGrayShade(r, g, b)) {
    int color = round(((double)r - 8) / 10) + 232;
    if (color > 255)
      color = 255;
    if (color < 232)
      color = 232;
    return color;
  } else {
    r = clampChannel(r);
    g = clampChannel(g);
    b = clampChannel(b);
    int color = 16 + (36 * r) + (6 * g) + b;
    if (color > 231)
      color = 231;
    if (color < 16)
      color = 16;
    return color;
  }
}

// For the normal colours use the following formula to calculate the ansii
// escape code (16–231) -> color = 16 + (36 * R) + (6 * G) + B 231 to 255
// contains a linear gray scale, to check if the color is on the gray scale the
// r, g, and b should be the same +-15 To calculate the corresponding gray color
// we use the formula n = round( (X - 8) / 10 ) + 232, where X is one of the
// three channels (doesn't matter the channel)

int main(int argc, char **argv) {

  if (argc % 2 == 0) {
    printf("%s\n", "ERROR: There is an empty argument");
    return 0;
  }
  struct Config config;
  config.path = "";
  config.size = -1;
  setupConfig(&config, argc, argv);

  if(config.help){
      printf("%s\n", "\n-i -> Specifies the image to use.\n-w [optional] -> Specifies the width of the console output (default is 300 pixels). The height uses the same proportions as the original image.\n-o [optional] -> Specifies the output path of the .txt with the ascii characters of the image.");
      return 0;
  }

  if (argc == 1 || strcmp(config.path, "") == 0) {
    printf("%s\n", "ERROR: You must at least specify the path to the image");
    return 0;
  }

  int width, height, channels;
  unsigned char *data = stbi_load(config.path, &width, &height, &channels, 0);

  int newWidth;
  if (config.size == -1) {
    newWidth = 300;
  } else {
    newWidth = config.size;
  }

  float ratio = (float)width / (float)newWidth;
  int newHeight = 0;
  if (ratio < 1) {
    newHeight = (int)ceil((float)height / ((float)newWidth / (float)width));
  } else {
    newHeight = (int)ceil((float)height / ratio);
  }

  unsigned char *dataResized = malloc(newWidth * newHeight * channels);
  stbir_pixel_layout layout = (channels == 4) ? STBIR_RGBA : STBIR_RGB;
  stbir_resize_uint8_linear(data, width, height, 0, dataResized, newWidth,
                            newHeight, 0, layout);

  width = newWidth;
  height = newHeight;
  unsigned char *temp = halfHeight(dataResized, width, height, channels);
  free(dataResized);
  dataResized = temp;
  height = floor((float)height / 2);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int pixel = (y * width + x) * channels;

      unsigned char r = dataResized[pixel + 0];
      unsigned char g = dataResized[pixel + 1];
      unsigned char b = dataResized[pixel + 2];

      float brightness = (0.299 * r + 0.587 * g + 0.114 * b);

      unsigned char index = floor((brightness / 255) * (arrayLength - 1));

      int color = getColor(r, g, b);

      printf("\033[48;5;0m\033[38;5;%dm%c\033[0m", color, asciiLetters[index]);
      // printf("%d\n",color);
    }
    printf("\n");
  }

  if(config.output != NULL ){
      FILE *test = fopen(config.output, "w");
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          int pixel = (y * width + x) * channels;
          unsigned char r = dataResized[pixel + 0];
          unsigned char g = dataResized[pixel + 1];
          unsigned char b = dataResized[pixel + 2];
          float brightness = (0.299 * r + 0.587 * g + 0.114 * b);
          unsigned char index = floor((brightness / 255) * (arrayLength - 1));
          fprintf(test, "%c", asciiLetters[index]);
        }

        fprintf(test, "\n");
      }
  }
  free(dataResized);
  return 0;
}
