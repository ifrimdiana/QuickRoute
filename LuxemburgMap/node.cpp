#include "node.h"

Node::Node()
{

}

int Node::getValue()
{
    return m_value;
}

QPoint Node::getPos() const
{
    return m_pos;
}

void Node::setValue(int n)
{
    m_value = n;
}

void Node::setPos(QPoint p)
{
    m_pos = p;
}

bool Node::operator==(const Node& other) const
{
    return this->getPos() == other.getPos();
}
