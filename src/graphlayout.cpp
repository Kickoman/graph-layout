#include "graphlayout.h"
#include "graphnodesproxymodel.h"
#include "graphlineproxymodel.h"
#ifndef DISABLE_THREADS
#include <QThreadPool>
#endif // DISABLE_THREADS
#include <QDebug>

GraphLayout::GraphLayout(IGraph *graph)
    : graph(graph)
    , nodesModel(nullptr)
    , edgesModel(nullptr)
{
    positions.resize(graph->nodesCount());
}

int GraphLayout::nodesCount() const
{
    return graph->nodesCount();
}

int GraphLayout::edgesCount() const
{
    return graph->edgesCount();
}

QVariant GraphLayout::node(int index) const
{
    return graph->node(index);
}

QPair<int, int> GraphLayout::edge(int index) const
{
    return graph->edge(index);
}

QVariant GraphLayout::edgeProperties(int index) const
{
    return graph->edgeProperties(index);
}

double GraphLayout::getNodeXPosition(int index) const
{
    bool indexOkay = index > -1 && index < nodesCount();
    return indexOkay ? positions[index].x() : -1;
}

double GraphLayout::getNodeYPosition(int index) const
{
    bool indexOkay = index > -1 && index < nodesCount();
    return indexOkay ? positions[index].y() : -1;
}

void GraphLayout::setNodeXPosition(int index, double x)
{
    if (!qFuzzyCompare(getNodeXPosition(index), x))
    {
        positions[index] = GraphGeometry::Point(x, getNodeYPosition(index));
        emit positionUpdated(index);
    }
}

void GraphLayout::setNodeYPosition(int index, double y)
{
    if (!qFuzzyCompare(getNodeYPosition(index), y))
    {
        positions[index] = GraphGeometry::Point(getNodeXPosition(index), y);
        emit positionUpdated(index);
    }
}

void GraphLayout::recalculatePositions()
{ 
    auto *calc = new GraphCalculator(graph, positions, positionsLock, config);
#ifndef DISABLE_THREADS
    connect(calc, &GraphCalculator::finished, this, &GraphLayout::positionsUpdated);
    connect(calc, &GraphCalculator::updated,  this, &GraphLayout::positionsUpdated);
    calc->setAutoDelete(true);
    QThreadPool::globalInstance()->start(calc);
#else
    calc->run();
    delete calc;
    emit positionsUpdated();
#endif // DISABLE_THREADS
}

void GraphLayout::setRandomPositions()
{
    positions.resize(nodesCount());
    auto area = std::max(config.nodeHeight * nodesCount(), config.nodeWidth * nodesCount());
    for (auto &pos : positions)
    {
        double x = rand() % int(area);
        double y = rand() % int(area);
        pos = GraphGeometry::Point(x, y);
    }
    emit positionsUpdated();
}

void GraphLayout::setGraphCalculatorConfig(const GraphCalculatorConfig &config)
{
    this->config = config;
}

void GraphLayout::setRepulsiveForce(const QString &formula)
{
    this->config.repulsiveForce = formula.toStdString();
}

void GraphLayout::setAttractiveForce(const QString &formula)
{
    this->config.attractiveForce = formula.toStdString();
}

void GraphLayout::setNodeSize(int width, int height)
{
    this->config.nodeWidth = width;
    this->config.nodeHeight = height;
}

void GraphLayout::setFrameSize(int width, int height)
{
    this->config.frameWidth = width;
    this->config.frameHeight = height;
}

QVariant GraphLayout::getNodesModel()
{
    if (!nodesModel)
    {
        nodesModel = new GraphNodesProxyModel(this);
    }
    return QVariant::fromValue(nodesModel);
}

QVariant GraphLayout::getEdgesModel()
{
    if (!edgesModel)
    {
        edgesModel = new GraphLineProxyModel(this);
    }
    return QVariant::fromValue(edgesModel);
}

void GraphLayout::setGraph(IGraph *graph)
{
    this->graph = graph;
    emit modelUpdated();
}

void GraphLayout::onGraphChanged()
{
    setRandomPositions();
}
