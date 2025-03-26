#ifndef LINE_H
#define LINE_H

#include "Node.h"

class Line {
private:
    Node* head;
    Node* cursor;
    bool cursorAtHead;

    bool isWordCharacter(char c);
    bool isPunctuation(char c);

public:
    Line();
    void insertChar(char ch);
    bool moveRight();
    bool moveLeft();
    void display(bool displayCursor) const;
    void resetCursor();
    Node* getCursor() const;
    Node* getHead() const;
    void deleteAfterCursor();
    void cursorAtEnd();
    void deleteChAtCursor();
    void setCursor(Node* c);
    void sethead(bool s);
    void moveToNextWord();
    void moveToPrevWord();
    void moveToNextWordEnd();
    Node* getTail();
    int getLength();
};

#endif