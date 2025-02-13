#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <limits>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include "edge.h"

class Graph
{
public:
    Graph();
    ~Graph();
    std::vector<Node*>& getNodes();
    std::vector<Edge*>& getEdges();
    std::unordered_map<Node*, std::vector<std::pair<Node*, Edge*>>> getAdjacencyList();
    Node* addNode(int id, QPoint pos);
    void addEdge(Node* from, Node* to,int length);
    bool loadFromXml(const QString& filePath);
    void scaleGraph(int width, int height, int margin);
    void rotateGraph90Right();
    std::vector<Edge*> dijkstra(Node* startNode, Node* endNode) const;
private:
    std::vector<Node*> m_nodes;
    std::vector<Edge*> m_edges;
    std::unordered_map<Node*, std::vector<std::pair<Node*, Edge*>>> adjacencyList;
};

#endif // GRAPH_H
