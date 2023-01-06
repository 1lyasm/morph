#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include "image.h"

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], Image&);
int writeImage(char[], Image&);
int** resimOku(char* resimadi);
void resimYaz(char* resimadi, short* resim, int N, int M, int Q);
int readImage(char fname[], Image& image);
int readImageHeader(char fname[], int& N, int& M, int& Q, bool& type);
int writeImage(char fname[], Image& image);

#endif
