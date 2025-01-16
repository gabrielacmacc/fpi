#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <iostream>

using namespace std;

string getRootPath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string path(buffer);

    string::size_type pos = path.find_last_of("\\/");
	path = path.substr(0, pos);
    pos = path.find_last_of("\\/");
	path = path.substr(0, pos);
	pos = path.find_last_of("\\/");

    if (pos != std::string::npos) {
        return path.substr(0, pos);
    }
    return "";
}

string openFileDialog() {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); 
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Imagens\0*.bmp;*.jpg;*.jpeg;*.png\0Todos os arquivos\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = "Select an image";

    if (GetOpenFileName(&ofn)) {
        return string(fileName);
    }

    return string();
}

string saveFileDialog(const std::string& defaultFileName) {
    OPENFILENAME ofn;       
    char fileName[MAX_PATH] = ""; 
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); 
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Imagens\0*.bmp;*.jpg;*.jpeg;*.png\0Todos os arquivos\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrTitle = "Save image as";
    strcpy_s(fileName, defaultFileName.c_str());

    if (GetSaveFileName(&ofn)) {
        return string(fileName);
    }

    return string();
}