#include "Line.h"
#include <iostream>
#include <cctype>

using namespace std;

Line::Line() {
    head = nullptr;
    cursor = nullptr;
    cursorAtHead = true;
}

bool Line::isWordCharacter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

bool Line::isPunctuation(char c) {
    return (c == ' ') || (c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

void Line::insertChar(char ch) {
    Node* newNode = new Node(ch);

    if (head == nullptr) {
        head = newNode;
        cursor = newNode;
        cursorAtHead = false;
        return;
    }

    if (cursorAtHead) {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
        cursor = newNode;
        cursorAtHead = false;
    }

    else {
        Node* nextNode;
        if (cursor) {
            nextNode = cursor->next;
        }
        else {
            nextNode = nullptr;
        }

        if (nextNode) {
            nextNode->prev = newNode;
        }

        newNode->next = nextNode;
        newNode->prev = cursor;
        cursor->next = newNode;
        cursor = newNode;
    }
}

bool Line::moveRight() {
    if (head == nullptr) {
        return true;
    }

    if (cursor == nullptr) {
        cursor = head;
        cursorAtHead = false;
        return false;
    }

    if (cursor->next != nullptr) {
        cursor = cursor->next;
        return false;
    }

    cursorAtHead = false;
    return true;
}

bool Line::moveLeft() {
    if (head == nullptr) {
        return true;
    }

    if (cursor == nullptr || cursor == head) {
        cursor = nullptr;
        cursorAtHead = true;
        return true;
    }

    cursor = cursor->prev;
    return false;
}

void Line::display(bool displayCursor) const {
    Node* temp = head;
    bool cursorDisplayed = false;

    while (temp != nullptr) {
        if (displayCursor && temp == cursor && !cursorDisplayed) {
            cout << "|";
            cursorDisplayed = true;
        }
        cout << temp->data;
        temp = temp->next;
    }

    if (displayCursor && cursor == nullptr && !cursorAtHead && !cursorDisplayed) {
        cout << "|";
    }
}

void Line::resetCursor() {
    cursor = nullptr;
    cursorAtHead = true;
}

Node* Line::getCursor() const {
    return cursor;
}

Node* Line::getHead() const {
    return head;
}

void Line::deleteAfterCursor() {
    if (cursor == nullptr) {
        while (head != nullptr) {
            Node* toDelete = head;
            head = head->next;
            delete toDelete;
        }
    }
    else {
        Node* temp = cursor->next;
        cursor->next = nullptr;
        while (temp != nullptr) {
            Node* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
}

void Line::cursorAtEnd() {
    if (head == nullptr) {
        cursor = nullptr;
        cursorAtHead = true;
        return;
    }

    Node* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }

    cursor = temp;
    cursorAtHead = false;
}

void Line::deleteChAtCursor() {
    if (cursor == nullptr) {
        return;
    }

    Node* toDelete = cursor;
    if (cursor == head) {
        head = cursor->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        cursor = nullptr;
        cursorAtHead = true;
    }
    else {
        if (cursor->prev != nullptr) {
            cursor->prev->next = cursor->next;
        }
        if (cursor->next != nullptr) {
            cursor->next->prev = cursor->prev;
            cursor = cursor->next;
        }
        else {
            cursor = cursor->prev;
        }
    }
    delete toDelete;
}

void Line::setCursor(Node* c) {
    cursor = c;
}

void Line::sethead(bool s) {
    cursorAtHead = (s != 0);
}

void Line::moveToNextWord() {
    if (!head) return;

    if (!cursor) {
        cursor = head;
        cursorAtHead = false;
    }

    while (cursor && cursor->next && isPunctuation(cursor->next->data)) {
        cursor = cursor->next;
    }

    while (cursor && cursor->next && !isPunctuation(cursor->next->data)) {
        cursor = cursor->next;
    }
}

void Line::moveToPrevWord() {
    if (!head) return;

    if (!cursor || cursor == head) {
        cursorAtHead = true;
        cursor = nullptr;
        return;
    }

    while (cursor && cursor->prev && isWordCharacter(cursor->prev->data)) {
        cursor = cursor->prev;
    }

    while (cursor && cursor->prev && isPunctuation(cursor->prev->data)) {
        cursor = cursor->prev;
    }

    while (cursor && cursor->prev && isWordCharacter(cursor->prev->data)) {
        cursor = cursor->prev;
    }

    if (cursor == head) {
        cursorAtHead = true;
        cursor = nullptr;
    }
}

void Line::moveToNextWordEnd() {
    if (!head) return;

    if (!cursor) {
        cursor = head;
        cursorAtHead = false;
    }

    while (cursor && cursor->next && isPunctuation(cursor->next->data)) {
        cursor = cursor->next;
    }

    while (cursor && cursor->next && !isPunctuation(cursor->next->data)) {
        cursor = cursor->next;
    }
}

Node* Line::getTail() {
    Node* originalCursor = cursor;
    cursorAtEnd();
    Node* tailNode = cursor;
    cursor = originalCursor; 
    return tailNode;
}

int Line::getLength() {
    int length = 0;
    for (Node* current = head; current != nullptr; current = current->next) {
        length++;
    }
    return length;
}
