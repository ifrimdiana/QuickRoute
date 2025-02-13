#ifndef NODE_H
#define NODE_H

#include <QPoint>
class Node
{
public:
    Node();
    int getValue();
    QPoint getPos()const;
    void setValue(int n);
    void setPos(QPoint p);
    bool operator==(const Node& other) const;
private:
    int m_value;
    QPoint m_pos;
};

#endif // NODE_H

