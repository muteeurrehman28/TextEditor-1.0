#ifndef NODE_H
#define NODE_H

struct Node {
    char data;
    Node* next;
    Node* prev;

    Node(char ch) {
        data = ch;
        next = nullptr;
        prev = nullptr;
    }
};

#endif
