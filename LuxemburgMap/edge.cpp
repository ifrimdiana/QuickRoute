#include "edge.h"

Edge::Edge()
{

}

Edge::Edge(Node* f, Node* s,int length): m_first(f), m_second(s),m_length(length) {}

Node* Edge::getFirst() const
{
    return m_first;
}

void Edge::setFirst(Node* f)
{
    m_first = f;
}

Node* Edge::getSecond() const
{
    return m_second;
}

void Edge::setSecond(Node* s)
{
    m_second = s;
}

int Edge::getLength()const
{
    return m_length;
}

void Edge::setLength(int length)
{
    m_length = length;
}
