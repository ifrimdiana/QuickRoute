#ifndef EDGE_H
#define EDGE_H

#include "node.h"
class Edge
{
public:
    Edge();
    Edge(Node* f, Node* s, int length);
    Node* getFirst()const;
    void setFirst(Node* f);
    Node* getSecond()const;
    void setSecond(Node* s);
    int getLength()const;
    void setLength(int length);
private:
    Node* m_first;
    Node* m_second;
    int m_length;

};

#endif // EDGE_H
