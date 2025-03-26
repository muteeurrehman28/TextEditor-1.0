#include <iostream>
#include <vector>
#include <cstdlib>
#include <conio.h>
#include <string>

#include "EditorStatus.h"
#include "Node.h" 
#include "Line.h"
#include "TextEditor.h"

using namespace std;

int getChar() {
    int key = _getch();
    if (key == 0 || key == 224) {
        int secondKey = _getch();
        if (secondKey == 72) {
            return 'A';
        }
        else if (secondKey == 80) {
            return 'B';
        }
        else if (secondKey == 77) {
            return 'C';
        }
        else if (secondKey == 75) {
            return 'D';
        }
    }
    return key;
}

void printCommandPrompt(const string& command) {
    system("cls");
    cout << "****************************************\n";
    cout << "*         Text Editor Command          *\n";
    cout << "****************************************\n";
    cout << "* Command: " << command << "\n";
    cout << "*                                      *\n";
    cout << "* Press ESC to cancel, ENTER to execute.*\n";
    cout << "****************************************\n";
}

int main() {
    TextEditor editor;
    int input;
    bool pasteMode = false;
    int count = 0;
    string searchPattern;
    bool commandMode = false;
    EditorStatus& status = editor.getStatus();

    while (true) {
        if (!commandMode) {
            editor.refreshDisplay();
        }
        input = getChar();

        if (input == ':') {
            string command;
            command += static_cast<char>(input);
            printCommandPrompt(command);
            commandMode = true;

            while (commandMode) {
                input = getChar();
                if (input == 27) {
                    status.statusMessage = "Command canceled. Returning to editor.";
                    commandMode = false;
                    break;
                }

                if (input == 13) {
                    status.statusMessage = "Executing command: " + command;
                    editor.handleFileCmd(command);
                    commandMode = false;
                    break;
                }
                command += static_cast<char>(input);
                printCommandPrompt(command);
            }

            if (command[1] == 'h' && input == 13) {
                commandMode = false;
                while (true) {
                    system("cls");
                    editor.refreshDisplay();
                    input = getChar();
                    if (input == 27) {
                        status.statusMessage = "Exiting history navigation. Returning to editor.";
                        editor.setLastCmd("Exiting From Command History....");
                        break;
                    }
                    if (input == 66) {
                        status.goToPreviousCommand();
                        status.previousCommand = "Moving To Previous Command.";
                    }
                    else if (input == 65) {
                        status.goToNextCommand();
                        status.previousCommand = "Moving To Next Command.";
                    }
                }
            }

            if (input == 13) {
                if (command.find(":s/") == 0) {
                    size_t firstSlash = command.find('/', 2);
                    size_t secondSlash = command.find('/', firstSlash + 1);

                    if (firstSlash != string::npos && secondSlash != string::npos) {
                        size_t thirdSlash = command.find('/', secondSlash + 1);

                        string oldStr = command.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                        string newStr;
                        bool global = false;

                        if (thirdSlash != string::npos) {
                            newStr = command.substr(secondSlash + 1, thirdSlash - secondSlash - 1);
                            string remainder = command.substr(thirdSlash + 1);
                            global = (remainder.find("g") != string::npos);
                        }
                        else {
                            newStr = command.substr(secondSlash + 1);
                            size_t gFlagPos = newStr.find("/g");
                            if (gFlagPos != string::npos) {
                                newStr = newStr.substr(0, gFlagPos);
                                global = true;
                            }
                        }

                        if (!oldStr.empty()) {
                            editor.setLastCmd("Replacing " + oldStr + " with " + newStr);
                            bool success = editor.replace(oldStr, newStr, global);
                            if (success) {
                                status.statusMessage = "Replacement successful.";
                            }
                            else {
                                status.statusMessage = "Word not found. No replacements made.";
                            }
                        }
                        else {
                            status.statusMessage = "Invalid substitution: old string is empty.";
                        }
                    }
                    else {
                        status.statusMessage = "Invalid substitution command syntax. Correct format: :s/old/new/[g]";
                    }
                }
                editor.handleFileCmd(command);
                commandMode = false;
                continue;
            }
        }

        if (input == 27) {
            editor.setLastCmd("Exiting Insert Mode....");
            editor.stopInsert();
            commandMode = false;
        }

        while (input >= '1' && input <= '9') {
            count = count * 10 + (input - '0');
            input = getChar();
        }

        if (count == 0) {
            count = 1;
        }

        if (editor.isInsert() && !commandMode) {
            if (input == 65) {
                editor.moveUp();
                editor.setLastCmd("Move Up.");
            }
            else if (input == 66) {
                editor.moveDown();
                editor.setLastCmd("Move Down.");
            }
            else if (input == 67) {
                editor.moveRight();
                editor.setLastCmd("Move Right.");
            }
            else if (input == 68) {
                editor.moveLeft();
                editor.setLastCmd("Move Left.");
            }
            else if (input == 13) {
                editor.addLine();
                editor.setLastCmd("New Line.");
            }
            else if (input == 8) {
                editor.backspace();
                editor.setLastCmd("Backspace.");
            }
            else {
                editor.insertChar(static_cast<char>(input));
            }
        }
        else {
            if (input == '/') {
                string pattern;
                input = getChar();
                while (input != '\n' && input != 27) {
                    pattern += static_cast<char>(input);
                    input = getChar();
                }
                if (!pattern.empty()) {
                    searchPattern = pattern;
                    editor.searchPattern(pattern);
                }
            }
            else if (input == 'n') {
                editor.setLastCmd("Find Next Occurrence.");
                if (!searchPattern.empty()) {
                    editor.nextMatch();
                }
            }
            else if (input == 'N') {
                editor.setLastCmd("Find Previous Occurrence.");
                if (!searchPattern.empty()) {
                    editor.prevMatch();
                }
            }
            else if (input == 'd') {
                input = getChar();
                if (input == 'd') {
                    if (count == 2) {
                        editor.setLastCmd("Deleting 2 Lines.");
                    }
                    else {
                        editor.setLastCmd("Deleting Line.");
                    }
                    for (int i = 0; i < count; ++i) {
                        editor.deleteLine();
                    }
                }
            }
            else if (input == 'y') {
                input = getChar();
                if (input == 'y') {
                    if (count == 2) {
                        pasteMode = true;
                        editor.copyLine();
                        editor.copyLine(true);
                        editor.setLastCmd("Copying 2 Lines.");
                    }
                    else {
                        editor.copyLine();
                        pasteMode = false;
                        editor.setLastCmd("Copy Line.");
                    }
                }
            }
            else if (input == 'j') {
                editor.setLastCmd("Moving Down 5 Times.");
                for (int i = 0; i < count - 1; ++i) {
                    editor.moveDown();
                }
            }
            else if (input == 'J') {
                editor.mergeLines();
                editor.setLastCmd("Joining Lines.");
            }
            else if (input == '>') {
                input = getChar();
                if (input == '>') {
                    for (int i = 0; i < count; ++i) editor.indent();
                    editor.setLastCmd("Indentation.");
                }
            }
            else if (input == '<') {
                input = getChar();
                if (input == '<') {
                    for (int i = 0; i < count; ++i) editor.unindent();
                    editor.setLastCmd("Unindentation.");
                }
            }
            else if (input == 'm') {
                editor.deleteToEnd();
                editor.setLastCmd("Delete To End Of Line.");
            }
            else if (input == 'x') {
                editor.deleteChar();
                editor.setLastCmd("Delete Character At Cursor.");
            }
            else if (input == 'p') {
                if (pasteMode) {
                    editor.pasteAfter();
                    editor.pasteAfter(true);
                    editor.setLastCmd("Pasting 2 lines After.");
                }
                else {
                    editor.pasteAfter();
                    editor.setLastCmd("Paste After.");
                }
            }
            else if (input == 'P') {
                if (pasteMode) {
                    editor.setLastCmd("Pasting 2 lines Before.");
                    editor.pasteBefore(true);
                    editor.pasteBefore();
                }
                else {
                    editor.pasteBefore();
                    editor.setLastCmd("Paste Before.");
                }
            }
            else if (input == '0') {
                editor.moveToStart();
                editor.setLastCmd("Move To The Start Of Line.");
            }
            else if (input == '$') {
                editor.moveToEnd();
                editor.setLastCmd("Move To The End Of Line");
            }
            else if (input == 'w') {
                editor.nextWord();
                editor.setLastCmd("Move To The Next Word.");
            }
            else if (input == 'b') {
                editor.prevWord();
                editor.setLastCmd("Move To The Previous Word.");
            }
            else if (input == 'e') {
                editor.endWord();
                editor.setLastCmd("Move To The Word End.");
            }
            else if (input == 'q') {
                editor.setLastCmd("Quitting From Editor....");
                return 0;
            }
            else if (input == 'i') {
                editor.startInsert();
                editor.setLastCmd("Entering Insert Mode....");
            }
            else if (input == 65) {
                editor.moveUp();
                editor.setLastCmd("Move Up.");
            }
            else if (input == 66) {
                editor.moveDown();
                editor.setLastCmd("Move Down.");
            }
            else if (input == 67) {
                editor.moveRight();
                editor.setLastCmd("Move Right.");
            }
            else if (input == 68) {
                editor.moveLeft();
                editor.setLastCmd("Move Left.");
            }
            else {
                editor.insertChar(static_cast<char>(input));
            }
        }
    }
}
