#include "TextEditor.h"
#include "FileManager.h"
#include "EditorStatus.h"
#include "Line.h"

using namespace std;

TextEditor::TextEditor() {
    currentLine = 0;
    insertMode = false;
    commandMode = false;
    matchLine = 0;
    matchColumn = 0;
    cursorColumn = 0;
    lines.emplace_back();
}

void TextEditor::toggleMode() {
    commandMode = !commandMode;
}

bool TextEditor::isCommand() const {
    return commandMode;
}

void TextEditor::refreshDisplay() {
    system("cls");
    for (size_t i = 0; i < lines.size(); ++i) {
        cout << (i + 1) << (i < 9 ? "  | " : " | ");
        if (i == currentLine) {
            cout << "> ";
        }
        lines[i].display(i == currentLine);
        cout << endl;
    }
    updateStatus();
}

void TextEditor::setLastCmd(const string& cmd) {
    status.previousCommand = cmd;
    status.logCommand(cmd);
}

bool TextEditor::isWordChar(char c) {
    if (isalnum(c)) {
        return true;
    }
    else {
        return c == '_';
    }
}

bool TextEditor::isPunct(char c) {
    if (ispunct(c)) {
        return true;
    }
    else {
        return (c == ' ' || isspace(c));
    }
}

void TextEditor::updateStatus() {
    adjustCursor();
    status.lineCount = static_cast<int>(lines.size());

    cout << "==========================================" << endl;

    if (isInsert()) {
        cout << "[INSERT MODE] ";
    }
    else {
        cout << "[NORMAL MODE] ";
    }

    string filename = fileManager.retrieveCurrentFileName();
    cout << "| File: ";
    if (filename.empty()) {
        cout << "Untitled";
    }
    else {
        cout << filename;
    }

    if (fileManager.hasPendingChanges()) {
        cout << " [Unsaved]";
    }
    cout << endl;

    cout << "Line: " << currentLine + 1
        << " | Column: " << cursorColumn + 1
        << " | Total Lines: " << status.lineCount << endl;

    cout << "Last Command: ";
    if (status.previousCommand.empty()) {
        cout << "None";
    }
    else {
        cout << status.previousCommand;
    }
    cout << endl;

    cout << "Command Status: " << status.statusMessage << endl;

    status.showCommandLog();

    cout << "==========================================" << endl;
}


void TextEditor::insertChar(char ch) {
    lines[currentLine].insertChar(ch);
    fileManager.markDocumentAsChanged();
    setLastCmd("Insert Char: " + string(1, ch));
}

void TextEditor::moveRight() {
    bool moved = lines[currentLine].moveRight();
    if (moved) {
        if (currentLine + 1 < lines.size()) {
            ++currentLine;
            lines[currentLine].resetCursor();
        }
    }
}

void TextEditor::moveLeft() {
    if (lines[currentLine].moveLeft() && currentLine > 0) {
        --currentLine;
        lines[currentLine].cursorAtEnd();
    }
}

void TextEditor::moveUp() {
    if (currentLine > 0) {
        currentLine--;
    }
}

void TextEditor::moveDown() {
    if (currentLine < lines.size() - 1) {
        currentLine++;
    }
}

void TextEditor::addLine() {
    status.statusMessage = "Successfully Added New Line!";
    fileManager.markDocumentAsChanged();

    Line newLine;
    Node* cursor = lines[currentLine].getCursor();

    if (cursor) {
        while (cursor != nullptr) {
            newLine.insertChar(cursor->data);
            cursor = cursor->next;
        }
    }

    lines.insert(lines.begin() + currentLine + 1, newLine);
    currentLine++;
    lines[currentLine].resetCursor();
}

void TextEditor::startInsert() {
    insertMode = true;
}

void TextEditor::stopInsert() {
    insertMode = false;
}

bool TextEditor::isInsert() const {
    return insertMode;
}

void TextEditor::deleteLine() {
    if (lines.empty() || currentLine < 0 || currentLine >= lines.size()) {
        status.statusMessage = "Invalid line to delete.";
        return;
    }

    fileManager.markDocumentAsChanged();

    Line tempLine = lines[currentLine];

    lines.erase(lines.begin() + currentLine);

    if (lines.empty()) {
        currentLine = -1;
    }
    else if (currentLine >= lines.size()) {
        currentLine = lines.size() - 1;
    }

    if (currentLine >= 0) {
        lines[currentLine].resetCursor();
    }

    status.statusMessage = "Line removed successfully!";
}

void TextEditor::deleteToEnd() {
    if (lines.empty() || currentLine < 0 || currentLine >= lines.size()) {
        status.statusMessage = "No content to delete.";
        return;
    }

    fileManager.markDocumentAsChanged();
    lines[currentLine].deleteAfterCursor();
    status.statusMessage = "Deleted successfully!";
}

void TextEditor::deleteChar() {
    if (lines.empty() || currentLine < 0 || currentLine >= lines.size()) {
        status.statusMessage = "Invalid line.";
        return;
    }

    fileManager.markDocumentAsChanged();
    Node* cursor = lines[currentLine].getCursor();

    if (lines[currentLine].getHead() == nullptr) {
        deleteLine();
    }
    else if (cursor == nullptr) {
        if (currentLine > 0) {
            lines[currentLine - 1].cursorAtEnd();
            cursor = lines[currentLine].getHead();
            lines[currentLine - 1].insertChar(cursor->data);

            while (cursor && cursor->next) {
                cursor = cursor->next;
                lines[currentLine - 1].insertChar(cursor->data);
            }

            deleteLine();
        }
    }
    else {
        lines[currentLine].deleteChAtCursor();
    }

    status.statusMessage = "Character deleted successfully!";
}

void TextEditor::backspace() {
    deleteChar();
}

void TextEditor::copyLine(bool append) {
    if (append && currentLine + 1 < lines.size()) {
        clipboardAlt.clear();
        Node* temp = lines[currentLine + 1].getHead();

        while (temp != nullptr) {
            clipboardAlt += temp->data;
            temp = temp->next;
        }
        status.statusMessage = "Copied next line to clipboard!";
    }
    else if (!append && currentLine >= 0 && currentLine < lines.size()) {
        clipboard.clear();
        Node* temp = lines[currentLine].getHead();

        while (temp != nullptr) {
            clipboard += temp->data;
            temp = temp->next;
        }
        status.statusMessage = "Copied current line to clipboard!";
    }
}

void TextEditor::pasteAfter(bool append) {
    fileManager.markDocumentAsChanged();
    const string& buffer = append ? clipboardAlt : clipboard;

    if (buffer.empty()) {
        addLine();
    }
    else {
        Line line;
        for (char ch : buffer) {
            line.insertChar(ch);
        }

        if (currentLine + 1 < lines.size()) {
            lines.insert(lines.begin() + currentLine + 1, line);
        }
        else {
            lines.push_back(line);
        }
        currentLine++;
    }
    status.statusMessage = "Content pasted after successfully!";
}

void TextEditor::pasteBefore(bool append) {
    fileManager.markDocumentAsChanged();
    const string& buffer = append ? clipboard : clipboardAlt;

    if (buffer.empty()) {
        addLine();
        moveUp();
    }
    else {
        Line line;
        for (char ch : buffer) {
            line.insertChar(ch);
        }
        lines.insert(lines.begin() + currentLine, line);
        currentLine++;
        lines[currentLine].cursorAtEnd();
    }
    status.statusMessage = "Content pasted before successfully!";
}

void TextEditor::moveToStart() {
    lines[currentLine].resetCursor();
    status.statusMessage = "Moved to the start of the line.";
}

void TextEditor::moveToEnd() {
    lines[currentLine].cursorAtEnd();
    status.statusMessage = "Moved to the end of the line.";
}

void TextEditor::nextWord() {
    if (currentLine >= lines.size() || lines[currentLine].getCursor() == nullptr) {
        status.statusMessage = "No cursor or line to move to the next word.";
        return;
    }

    Node* cursor = lines[currentLine].getCursor();
    if (cursor->next == nullptr && currentLine == lines.size() - 1) {
        status.statusMessage = "No next word exists.";
        return;
    }

    if (cursor->next == nullptr) {
        if (currentLine < lines.size() - 1) {
            moveDown();
            lines[currentLine].resetCursor();
        }
    }

    lines[currentLine].moveToNextWord();
    status.statusMessage = "Moved to the next word.";
}

void TextEditor::prevWord() {
    if (currentLine <= 0 && lines[currentLine].getCursor() == nullptr) {
        status.statusMessage = "No previous word exists.";
        return;
    }

    if (lines[currentLine].getCursor() == nullptr) {
        moveUp();
        lines[currentLine].cursorAtEnd();
        status.statusMessage = "Moved to the previous word.";
    }
    else {
        lines[currentLine].moveToPrevWord();
        status.statusMessage = "Moved to the previous word.";
    }
}

void TextEditor::endWord() {
    if (lines[currentLine].getCursor() == nullptr || lines[currentLine].getHead() == nullptr) {
        status.statusMessage = "No word to end.";
        return;
    }

    Node* cursor = lines[currentLine].getCursor();

    if (cursor->next == nullptr && currentLine == lines.size() - 1) {
        status.statusMessage = "No next word exists.";
        return;
    }

    if (cursor->next == nullptr) {
        if (currentLine < lines.size() - 1) {
            moveDown();
            lines[currentLine].resetCursor();
            status.statusMessage = "Moved to the end of the word in next line.";
        }
    }
    else {
        lines[currentLine].moveToNextWordEnd();
        status.statusMessage = "Moved to the end of the word.";
    }
}

void TextEditor::adjustCursor() {
    Node* cursor = lines[currentLine].getCursor();
    if (cursor == nullptr || cursor == lines[currentLine].getHead()) {
        status.cursorXPosition = 1;
        return;
    }

    lines[currentLine].resetCursor();
    int position = 1;

    while (lines[currentLine].getCursor() != cursor) {
        lines[currentLine].moveRight();
        position++;
    }

    status.cursorXPosition = position;
    lines[currentLine].setCursor(cursor);
}

void TextEditor::handleFileError(const string& operation, const string& filename) {
    std::string errorMessage = "Error with file: " + filename;

    if (operation == "saveFile") {
        if (errno == ENOENT) {
            status.statusMessage = "Error: File not found: " + filename;
        }
        else if (errno == EACCES) {
            status.statusMessage = "Error: Permission denied: " + filename;
        }
        else if (errno == ENOSPC) {
            status.statusMessage = "Error: Disk full. Cannot save file: " + filename;
        }
        else if (errno == EEXIST) {
            status.statusMessage = "Error: File already exists: " + filename;
        }
        else {
            status.statusMessage = errorMessage + " during save operation.";
        }
    }
    else if (operation == "loadFile") {
        if (errno == ENOENT) {
            status.statusMessage = "Error: File not found: " + filename;
        }
        else if (errno == EACCES) {
            status.statusMessage = "Error: Permission denied: " + filename;
        }
        else {
            status.statusMessage = errorMessage + " during load operation.";
        }
    }
}

bool TextEditor::handleFileCmd(const std::string& cmd) {
    if (cmd == ":q" || cmd == ":q!") {
        if (cmd == ":q") {
            if (fileManager.hasPendingChanges()) {
                status.statusMessage = "Unsaved changes. Use :q! to quit without saving.";
                return false;
            }
        }
        status.statusMessage = (cmd == ":q!") ? "Force quit without saving." : status.statusMessage;
        exit(0);
    }
    else if (cmd.find(":w ") == 0 || cmd == ":w" || cmd == ":wq") {
        std::string filename = (cmd == ":w") ? fileManager.retrieveCurrentFileName() : cmd.substr(3);

        if (filename.empty()) {
            status.statusMessage = "No file name specified. Use :w <filename> to save.";
        }
        else if (fileManager.saveDocument(filename, lines)) {
            status.statusMessage = (cmd == ":wq") ? "File saved. Exiting..." : "File saved to " + filename;
            if (cmd == ":wq") {
                exit(0);
            }
        }
        else {
            handleFileError("saveFile", filename);
        }
    }
    else if (cmd.find(":e ") == 0) {
        std::string filename = cmd.substr(3);
        if (fileManager.loadDocument(filename, lines)) {
            status.statusMessage = "File loaded successfully.";
        }
        else {
            handleFileError("loadFile", filename);
        }
    }
    else {
        status.statusMessage = "Unknown command: " + cmd;
        return false;
    }
    return true;
}

void TextEditor::mergeLines() {
    if (currentLine < 0 || currentLine >= lines.size() - 1) {
        status.statusMessage = "No more lines to join.";
        return;
    }

    fileManager.markDocumentAsChanged();

    Line& current = lines[currentLine];
    Line& next = lines[currentLine + 1];
    Node* cursor = next.getHead();

    if (cursor) {
        do {
            current.insertChar(cursor->data);
            cursor = cursor->next;
        } while (cursor);

        deleteLine();
        status.statusMessage = "Lines joined successfully.";
    }
}

void TextEditor::indent() {
    if (currentLine < 0 || currentLine >= lines.size()) return;

    fileManager.markDocumentAsChanged();
    Line& current = lines[currentLine];

    current.insertChar(' ');
    current.insertChar(' ');

    status.statusMessage = "Line indented successfully.";
}

void TextEditor::unindent() {
    if (currentLine < 0 || currentLine >= lines.size()) return;

    Node* cursor = lines[currentLine].getHead();
    if (!cursor) return;

    lines[currentLine].setCursor(cursor);

    if (cursor->data == ' ') {
        deleteChar();
        cursor = lines[currentLine].getCursor();
        deleteChar();
        status.statusMessage = "Unindented successfully.";
    }
    else {
        status.statusMessage = "No indentation to remove.";
    }
}

void TextEditor::deleteLineNum(int lineNum) {
    if (lineNum <= 0 || lineNum > lines.size()) {
        status.statusMessage = "Invalid line number: " + std::to_string(lineNum);
        return;
    }

    currentLine = lineNum - 1;
    fileManager.markDocumentAsChanged();

    auto lineIterator = lines.begin() + currentLine;
    lines.erase(lineIterator);

    status.statusMessage = "Deleted line " + std::to_string(lineNum) + " successfully.";
}

bool TextEditor::search(const std::string& pattern) {
    if (pattern.empty()) return false;

    for (int lineIndex = 0; lineIndex < lines.size(); ++lineIndex) {
        Node* cursor = lines[lineIndex].getHead();
        int position = 0;

        while (cursor) {
            if (cursor->data == pattern[0]) {
                Node* matchStart = cursor;
                int matchIndex = 0;
                Node* tempCursor = cursor->next;

                while (tempCursor && matchIndex < pattern.size() && tempCursor->data == pattern[matchIndex]) {
                    matchIndex++;
                    tempCursor = tempCursor->next;
                }

                if (matchIndex == pattern.size()) {
                    lines[lineIndex].setCursor(matchStart);
                    adjustCursor();

                    currentLine = lineIndex;
                    matchLine = lineIndex + 1;
                    cursorColumn = position;
                    matchColumn = status.cursorXPosition;

                    return true;
                }
            }
            cursor = cursor->next;
            position++;
        }
    }

    return false;
}

void TextEditor::searchPattern(const std::string& pattern) {
    searchQuery = pattern;
    bool found = search(pattern);

    if (!found) {
        status.statusMessage = "Pattern '" + pattern + "' not found.";
    }
}

void TextEditor::nextMatch() {
    if (searchQuery.empty()) {
        status.statusMessage = "No previous search pattern.";
        return;
    }

    bool matchFound = false;
    std::string resultMsg;

    for (size_t i = currentLine + 1; i < lines.size(); ++i) {
        Node* cursor = lines[i].getHead();
        int column = 0;
        Node* matchCursor = nullptr;

        while (cursor) {
            if (cursor->data == searchQuery[0]) {
                Node* tempCursor = cursor;
                int matchIndex = 0;

                while (tempCursor && matchIndex < searchQuery.size() && tempCursor->data == searchQuery[matchIndex]) {
                    tempCursor = tempCursor->next;
                    matchIndex++;
                }

                if (matchIndex == searchQuery.size()) {
                    matchCursor = cursor;
                    break;
                }
            }
            cursor = cursor->next;
            column++;
        }

        if (matchCursor) {
            currentLine = i;
            lines[currentLine].setCursor(matchCursor);
            adjustCursor();
            matchLine = currentLine + 1;
            matchColumn = status.cursorXPosition;
            resultMsg = "Pattern found at line: " + std::to_string(matchLine) + ", column: " + std::to_string(matchColumn);
            status.statusMessage = resultMsg;
            matchFound = true;
            break;
        }
    }

    if (!matchFound) {
        resultMsg = searchQuery + " not found";
        status.statusMessage = resultMsg;
    }
}

void TextEditor::prevMatch() {
    if (searchQuery.empty()) {
        status.statusMessage = searchQuery + " not found";
        return;
    }

    if (matchLine < 0 || matchColumn < 0) {
        status.statusMessage = "Invalid col and line";
        return;
    }

    std::string resultMsg;
    bool matchFound = false;

    for (int i = currentLine; i >= 0; --i) {
        Node* cursor = (i == currentLine) ? lines[i].getCursor() : lines[i].getTail();
        int col = (i == currentLine) ? cursorColumn - 1 : lines[i].getLength() - 1;

        while (cursor && col >= 0) {
            if (cursor->data == searchQuery.back()) {
                Node* tempCursor = cursor;
                int matchIndex = searchQuery.length() - 1;

                while (matchIndex >= 0 && tempCursor && tempCursor->data == searchQuery[matchIndex]) {
                    tempCursor = tempCursor->prev;
                    matchIndex--;
                }

                if (matchIndex < 0) {
                    currentLine = i;
                    matchLine = currentLine + 1;
                    lines[currentLine].setCursor(cursor);
                    adjustCursor();
                    matchColumn = status.cursorXPosition;

                    resultMsg = "line: " + std::to_string(matchLine) + " column: " + std::to_string(matchColumn);
                    status.statusMessage = resultMsg;
                    matchFound = true;
                    break;
                }
            }
            cursor = cursor ? cursor->prev : nullptr;
            col--;
        }

        if (matchFound) break;
    }

    if (!matchFound) {
        resultMsg = searchQuery + " not found";
        status.statusMessage = resultMsg;
    }
}

bool TextEditor::replace(const string& oldStr, const string& newStr, bool global) {
    if (oldStr.empty()) {
        return false;
    }

    bool replaced = false;

    while (true) {
        if (!search(oldStr)) {
            break;
        }

        replaced = true;

        for (int i = 0; i < oldStr.length(); ++i) {
            moveRight();
        }

        for (int i = 0; i < oldStr.length(); ++i) {
            lines[currentLine].deleteChAtCursor();
        }

        for (char ch : newStr) {
            lines[currentLine].insertChar(ch);
        }

        if (!global) {
            return true;
        }
    }

    return replaced;
}

EditorStatus& TextEditor::getStatus() {
    return status;
}