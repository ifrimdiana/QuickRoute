#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QColor>
#include <QMouseEvent>
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawNode(QPainter& painter, Node* node);
    void drawEdge(QPainter& painter, Edge* edge, const std::vector<Edge*>& shortestPath);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::MainWindow *ui;
    Graph* m_graph;
    Node* m_selectedNode1;
    Node* m_selectedNode2;

    QPixmap graphBuffer;
    bool bufferInitialized;
};
#endif // MAINWINDOW_H
