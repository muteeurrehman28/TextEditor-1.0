#pragma once

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "Node.h"
#include "Line.h"
#include "EditorStatus.h"
#include "FileManager.h"
#include <vector>
#include <string>

using namespace std;

class TextEditor {
private:
    vector<Line> lines;
    int currentLine;
    bool insertMode;
    bool commandMode;
    string clipboard;
    string clipboardAlt;
    EditorStatus status;
    string searchQuery;
    int matchLine;
    int matchColumn;
    int cursorColumn;
public:
    TextEditor();

    FileManager fileManager;

    void toggleMode();
    bool isCommand() const;
    void refreshDisplay();
    void setLastCmd(const string& cmd);
    bool isWordChar(char c);
    bool isPunct(char c);
    void updateStatus();
    void insertChar(char ch);
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void addLine();
    void startInsert();
    void stopInsert();
    bool isInsert() const;
    void deleteLine();
    void deleteToEnd();
    void deleteChar();
    void backspace();
    void copyLine(bool append = false);
    void pasteAfter(bool append = false);
    void pasteBefore(bool append = false);
    void moveToStart();
    void moveToEnd();
    void nextWord();
    void prevWord();
    void endWord();
    void adjustCursor();
    void handleFileError(const string& operation, const string& filename);
    bool handleFileCmd(const string& cmd);
    void mergeLines();
    void indent();
    void unindent();
    void deleteLineNum(int lineNum);
    bool search(const string& query);
    void searchPattern(const string& query);
    void nextMatch();
    void prevMatch();
    bool replace(const string& oldStr, const string& newStr, bool global = false);
    EditorStatus& getStatus();
};

#endif