#include <stdio.h>
#include <iostream>

#ifndef filesOperations_h
#define filesOperations_h

std::string getRootPath();
std::string openFileDialog();
std::string saveFileDialog(const std::string& defaultFileName);

#endif