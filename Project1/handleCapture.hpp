#ifndef _handleCapture_hpp
#define _handleCatptue_hpp

#include "generatePattern.hpp"


using namespace pat;

int capture();
void showPattern(int index, GRAY_STR* gr, IplImage* pattern);
int capturePattern(int index, VideoCapture cap);

#endif