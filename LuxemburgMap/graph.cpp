#include "graph.h"

Graph::Graph() {
}

Graph::~Graph() {
    for (Node* node : m_nodes)
        delete node;
    m_nodes.clear();

    for (Edge* edge : m_edges)
        delete edge;
    m_edges.clear();
}

std::vector<Node*>& Graph::getNodes() {
    return m_nodes;
}

std::vector<Edge*>& Graph::getEdges() {
    return m_edges;
}

std::unordered_map<Node*, std::vector<std::pair<Node*, Edge*>>> Graph::getAdjacencyList() {
    return adjacencyList;
}

Node* Graph::addNode(int id, QPoint pos)
{
    for (Node* currentNode : m_nodes) {
        if (currentNode->getValue() == id) {
            qDebug() << "Node with ID" << id << "already exists.";
            return currentNode;
        }
    }

    Node* newNode = new Node();
    newNode->setValue(id);
    newNode->setPos(pos);

    m_nodes.push_back(newNode);
    adjacencyList[newNode] = {};

    return newNode;
}

void Graph::addEdge(Node* from, Node* to, int length)
{
    if (from == nullptr || to == nullptr) {
        qDebug() << "Error: Cannot add edge. One of the nodes is null.";
        return;
    }

    for (Edge* edge : m_edges) {
        if ((edge->getFirst() == from && edge->getSecond() == to) ||
            (edge->getFirst() == to && edge->getSecond() == from)) {
            qDebug() << "Edge between" << from->getValue() << "and" << to->getValue() << "already exists.";
            return;
        }
    }

    Edge* newEdge = new Edge(from, to, length);
    m_edges.push_back(newEdge);

    adjacencyList[from].push_back({to,newEdge});
    adjacencyList[to].push_back({from,newEdge});

    qDebug() << "Edge added between" << from->getValue() << "and" << to->getValue() << "with length" << length;
}

bool Graph::loadFromXml(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filePath;
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse XML content";
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if (root.tagName() != "map") {
        qDebug() << "Invalid root element. Expected 'map'";
        return false;
    }

    QDomElement nodesElement = root.firstChildElement("nodes");
    if (nodesElement.isNull()) {
        qDebug() << "Error: Missing 'nodes' element in XML.";
        return false;
    }

    QMap<int, Node*> nodeMap;

    QDomElement nodeElement = nodesElement.firstChildElement("node");
    while (!nodeElement.isNull()) {
        int id = nodeElement.attribute("id").toInt();
        int latitude = nodeElement.attribute("latitude").toInt();
        int longitude = nodeElement.attribute("longitude").toInt();

        Node* newNode = addNode(id, QPoint(latitude, longitude));
        nodeMap[id] = newNode;

        qDebug() << "Parsed node with ID" << id << "at position (" << latitude << "," << longitude << ")";
        nodeElement = nodeElement.nextSiblingElement("node");
    }

    QDomElement arcsElement = root.firstChildElement("arcs");
    if (arcsElement.isNull()) {
        qDebug() << "Error: Missing 'arcs' element in XML.";
        return false;
    }

    QDomElement arcElement = arcsElement.firstChildElement("arc");
    while (!arcElement.isNull()) {
        int fromId = arcElement.attribute("from").toInt();
        int toId = arcElement.attribute("to").toInt();
        int length = arcElement.attribute("length").toInt();

        if (nodeMap.contains(fromId) && nodeMap.contains(toId)) {
            Node* fromNode = nodeMap[fromId];
            Node* toNode = nodeMap[toId];

            if (fromNode == nullptr || toNode == nullptr) {
                qDebug() << "Invalid edge: From ID" << fromId << ", To ID" << toId;
                arcElement = arcElement.nextSiblingElement("arc");
                continue;
            }

            addEdge(fromNode, toNode, length);
        } else {
            qDebug() << "Edge references non-existent nodes: From ID" << fromId << ", To ID" << toId;
        }

        arcElement = arcElement.nextSiblingElement("arc");
    }

    return true;
}

void Graph::scaleGraph(int width, int height, int margin)
{
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = INT_MIN, maxY = INT_MIN;

    for (Node* node : m_nodes) {
        QPoint pos = node->getPos();
        minX = std::min(minX, pos.x());
        minY = std::min(minY, pos.y());
        maxX = std::max(maxX, pos.x());
        maxY = std::max(maxY, pos.y());
    }

    double widthScale = (width - 2 * margin) / static_cast<double>(maxX - minX);
    double heightScale = (height - 2 * margin) / static_cast<double>(maxY - minY);
    double scale = std::min(widthScale, heightScale);

    int offsetX = (width - (maxX - minX) * scale) / 2;
    int offsetY = (height - (maxY - minY) * scale) / 2;

    for (Node* node : m_nodes) {
        QPoint pos = node->getPos();
        int scaledX = offsetX + static_cast<int>((pos.x() - minX) * scale);
        int scaledY = offsetY + static_cast<int>((pos.y() - minY) * scale);
        node->setPos(QPoint(scaledX, scaledY));
    }

    qDebug() << "Graph scaled and centered. Scale factor:" << scale;
}

void Graph::rotateGraph90Right()
{
    if (m_nodes.empty()) return;

    int minX = INT_MAX, minY = INT_MAX;
    int maxX = INT_MIN, maxY = INT_MIN;

    for (Node* node : m_nodes) {
        QPoint pos = node->getPos();
        minX = std::min(minX, pos.x());
        minY = std::min(minY, pos.y());
        maxX = std::max(maxX, pos.x());
        maxY = std::max(maxY, pos.y());
    }

    int centerX = (minX + maxX) / 2;
    int centerY = (minY + maxY) / 2;

    for (Node* node : m_nodes) {
        QPoint pos = node->getPos();

        int translatedX = pos.x() - centerX;
        int translatedY = pos.y() - centerY;

        int rotatedX = translatedY;
        int rotatedY = -translatedX;

        int finalX = rotatedX + centerX;
        int finalY = rotatedY + centerY;

        node->setPos(QPoint(finalX, finalY));
    }
}

std::vector<Edge*> Graph::dijkstra(Node* startNode, Node* endNode) const
{
    if (!startNode || !endNode)
    {
        qDebug() << "Start or end node is null.";
        return {};
    }

    using NodeDist = std::pair<int, Node*>;
    std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<>> pq;

    std::unordered_map<Node*, int> distances;
    std::unordered_map<Node*, Node*> predecessors;
    std::unordered_set<Node*> visited;

    for (Node* node : m_nodes)
    {
        distances[node] = std::numeric_limits<int>::max();
        predecessors[node] = nullptr;
    }

    distances[startNode] = 0;
    pq.push({0, startNode});

    while (!pq.empty())
    {
        Node* current = pq.top().second;
        pq.pop();

        if (visited.find(current) != visited.end()) {
            continue;
        }

        visited.insert(current);

        if (current == endNode) {
            break;
        }

        for (const auto& neighborPair : adjacencyList.at(current))
        {
            Node* neighbor = neighborPair.first;
            Edge* edge = neighborPair.second;

            int edgeLength = edge->getLength();
            int newDist = distances[current] + edgeLength;

            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                predecessors[neighbor] = current;
                pq.push({newDist, neighbor});
            }
        }
    }

    std::vector<Edge*> path;
    for (Node* at = endNode; at != nullptr && predecessors[at] != nullptr; at = predecessors[at]) {
        Node* prev = predecessors[at];

        for (const auto& neighborPair : adjacencyList.at(prev)) {
            if (neighborPair.first == at)
            {
                path.push_back(neighborPair.second);
                break;
            }
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}
