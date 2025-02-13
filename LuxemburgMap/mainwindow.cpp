#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_graph(new Graph())
    , m_selectedNode1(nullptr)
    , m_selectedNode2(nullptr)
    , bufferInitialized(false)
{
    ui->setupUi(this);

    if (!m_graph->loadFromXml("Harta_Luxemburg.xml")) {
        qDebug() << "Failed to load graph from file. Exiting.";
        return;
    }

    m_graph->scaleGraph(this->width(), this->height(), 50);
    m_graph->rotateGraph90Right();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (!m_graph || m_graph->getNodes().empty()) {
        qDebug() << "Graph is empty or not initialized.";
        return;
    }

    QPoint clickPos = event->pos();
    Node* closestNode = nullptr;
    double minDistance = 50.0;

    for (Node* node : m_graph->getNodes()) {
        QPoint pos = node->getPos();
        double distance = std::sqrt(std::pow(clickPos.x() - pos.x(), 2) +
                                    std::pow(clickPos.y() - pos.y(), 2));
        if (distance < minDistance) {
            minDistance = distance;
            closestNode = node;
        }
    }

    if (closestNode) {
        if (!m_selectedNode1) {
            m_selectedNode1 = closestNode;
        } else if (!m_selectedNode2) {
            m_selectedNode2 = closestNode;
        } else {
            m_selectedNode1 = closestNode;
            m_selectedNode2 = nullptr;
        }

        update();
    } else {
        qDebug() << "No node selected.";
    }
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    if (!m_graph || m_graph->getNodes().empty())
    {
        qDebug() << "Nothing to draw. Graph is empty.";
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor blue(81, 173, 218);
    QColor red(234, 20, 29);

    if (!bufferInitialized)
    {
        graphBuffer = QPixmap(this->size());
        graphBuffer.fill(Qt::transparent);
        QPainter bufferPainter(&graphBuffer);

        bufferPainter.setPen(blue);

        for (Edge* edge : m_graph->getEdges())
        {
            if (!edge) continue;

            Node* first = edge->getFirst();
            Node* second = edge->getSecond();

            if (!first || !second)
            {
                qDebug() << "Invalid edge with null nodes.";
                continue;
            }

            QPoint pos1 = first->getPos();
            QPoint pos2 = second->getPos();

            bufferPainter.drawLine(pos1, pos2);
        }

        bufferPainter.setPen(blue);
        bufferPainter.setBrush(blue);

        for (Node* node : m_graph->getNodes())
        {
            if (!node) continue;

            QPoint pos = node->getPos();
            bufferPainter.drawEllipse(pos.x() - 1, pos.y() - 1, 2, 2);
        }

        bufferInitialized = true;
    }

    painter.drawPixmap(0, 0, graphBuffer);

    std::vector<Edge*> shortestPath;
    std::unordered_set<Node*> shortestPathNodes;

    if (m_selectedNode1 && m_selectedNode2)
    {
        shortestPath = m_graph->dijkstra(m_selectedNode1, m_selectedNode2);
        for (const Edge* edge : shortestPath)
        {
            shortestPathNodes.insert(edge->getFirst());
            shortestPathNodes.insert(edge->getSecond());
        }
    }

    painter.setPen(red);

    for (const Edge* edge : shortestPath)
    {
        Node* first = edge->getFirst();
        Node* second = edge->getSecond();

        if (!first || !second) continue;

        QPoint pos1 = first->getPos();
        QPoint pos2 = second->getPos();

        painter.drawLine(pos1, pos2);
    }

    painter.setPen(red);
    painter.setBrush(red);

    for (Node* node : shortestPathNodes)
    {
        if (!node) continue;

        QPoint pos = node->getPos();
        painter.drawEllipse(pos.x() - 1, pos.y() - 1, 2, 2);
    }

    if (m_selectedNode1)
    {
        QPoint pos = m_selectedNode1->getPos();
        painter.setPen(red);
        painter.setBrush(red);
        painter.drawEllipse(pos.x() - 1, pos.y() - 1, 2, 2);
    }

    if (m_selectedNode2)
    {
        QPoint pos = m_selectedNode2->getPos();
        painter.setPen(red);
        painter.setBrush(red);
        painter.drawEllipse(pos.x() - 1, pos.y() - 1, 2, 2);
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    if (!m_graph || m_graph->getNodes().empty()) {
        return;
    }

    bufferInitialized = false;

    m_graph->scaleGraph(this->width(), this->height(), 50);

    update();

    QMainWindow::resizeEvent(event);
}





