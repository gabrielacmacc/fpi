#include <stdio.h>
#include <iostream>

#ifndef files_h
#define files_h

std::string getRootPath();
std::string openFileDialog();
std::string saveFileDialog(const std::string& defaultFileName);

#endif