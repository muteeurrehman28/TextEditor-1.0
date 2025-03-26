#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <string>
#include <vector>
#include "Line.h"

using namespace std;

class FileManager {
private:
    string currentFileName;
    bool modified;

public:
    FileManager();
    bool loadDocument(const string& fileName, vector<Line>& lines);
    bool saveDocument(const string& fileName, vector<Line>& lines);
    bool hasPendingChanges() const;
    void markDocumentAsChanged();
    string retrieveCurrentFileName() const;
};

#endif
