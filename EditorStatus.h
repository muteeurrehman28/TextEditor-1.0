#ifndef EDITORSTATUS_H
#define EDITORSTATUS_H

#include <iostream>
#include <string>
#include <deque>

using namespace std;

struct EditorStatus {
public:
    int cursorXPosition;
    int lineCount;
    int currentIndex;
    deque<string> commandLog;
    string statusMessage;
    string previousCommand;

    EditorStatus();

    void logCommand(const string& command);
    void showCommandLog() const;
    void goToNextCommand();
    void goToPreviousCommand();
};

#endif
