#include "EditorStatus.h"
using namespace std;

EditorStatus::EditorStatus() {
    cursorXPosition = 0;
    lineCount = 1;
    currentIndex = 0;
    previousCommand = "";
    statusMessage = "";
}

void EditorStatus::logCommand(const string& command) {
    if (!command.empty()) {
        commandLog.push_back(command);
        previousCommand = command;
    }
}

void EditorStatus::showCommandLog() const {
    if (commandLog.size() == 0) {
        cout << "No commands logged yet." << endl;
        return;
    }

    cout << "Command Log:" << endl;
    for (size_t i = 0; i < commandLog.size(); ++i) {
        string status = (i == currentIndex) ? "[Current] " : "";
        //cout << status << commandLog[i] << endl;
    }
}

void EditorStatus::goToNextCommand() {
    if (commandLog.size() == 0) {
        statusMessage = "There are no commands to move through.";
        return;
    }

    if (currentIndex < commandLog.size() - 1) {
        ++currentIndex;
    }

    else {
        statusMessage = "You have reached the most recent command.";
    }
}

void EditorStatus::goToPreviousCommand() {
    if (commandLog.size() == 0) {
        statusMessage = "The command log is empty, so no previous command exists.";
        return;
    }

    if (currentIndex > 0) {
        --currentIndex;
    }
    else {
        statusMessage = "You are already at the beginning of the command history.";
    }
}
