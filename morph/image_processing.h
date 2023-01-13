#pragma once

#include "image.h"


using namespace std;

int readImageHeader(char[], int&, int&, int&, bool&);
int readImage(char[], Image&);
int writeImage(char[], Image&);
int** resimOku(char* resimadi);
void resimYaz(char* resimadi, short* resim, int N, int M, int Q);
int readImage(char fname[], Image& image);
int readImageHeader(char fname[], int& N, int& M, int& Q, bool& type);
int writeImage(char fname[], Image& image);
