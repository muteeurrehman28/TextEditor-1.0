#include "FileManager.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

FileManager::FileManager() {
    currentFileName = "";
    modified = false;
}

bool FileManager::hasPendingChanges() const {
    return modified;
}

string FileManager::retrieveCurrentFileName() const {
    return currentFileName;
}

void FileManager::markDocumentAsChanged() {
    modified = true;
}

bool FileManager::loadDocument(const string& filename, vector<Line>& lines) {
    ifstream fileStream(filename);
    if (fileStream.fail()) {
        return false;
    }

    lines.clear();

    string current;
    while (fileStream >> ws && getline(fileStream, current)) {
        Line lineData;
        for (char c : current) {
            lineData.insertChar(c);
        }
        lines.push_back(lineData);
    }

    fileStream.close();
    return true;

}

bool FileManager::saveDocument(const string& fileName, vector<Line>& lines) {
    ofstream outputStream;
    outputStream.open(fileName);
    if (outputStream.fail()) {
        return false;
    }

    for (const auto& line : lines) {
        for (Node* currentNode = line.getHead(); currentNode != nullptr; currentNode = currentNode->next) {
            outputStream << currentNode->data;
        }
        outputStream.put('\n');
    }

    outputStream.close();
    currentFileName = fileName;
    modified = false;
    return true;
}