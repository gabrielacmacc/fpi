#include <stdio.h>
#include <iostream>

using namespace std;

#ifndef files_h
#define files_h

string getRootPath();
string openFileDialog();
string saveFileDialog(const string& defaultFileName);

#endif