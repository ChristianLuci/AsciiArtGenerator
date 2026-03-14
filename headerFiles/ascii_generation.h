#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <stdbool.h>
#include <stddef.h>

struct Config {
    char *path;
    int size;
    bool help;
};

unsigned char *halfHeight(unsigned char *data, int width, int height, int channels);

unsigned char* resizeImage(unsigned char *data, int width, int height, int newWidth, int newHeight, int channels);

// converts the rgb color into an ansii value
int getAnsiiColor(int r, int g, int b);

bool isGrayShade(int r, int g, int b);

char *generateAsciiArt(unsigned char *imageData, int width, int height, int channels);

void setupConfig(struct Config *config, int argc, char **argv);

int average(int a, int b);

// converts the rgb value from 0-255 to 0-5
int clampChannel(int c);

#endif
