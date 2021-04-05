#include "graphlayout.h"
#include "graphnodesproxymodel.h"
#include "graphlineproxymodel.h"
#ifndef DISABLE_THREADS
#include <QThreadPool>
#endif // DISABLE_THREADS
#include <QDebug>

QVariant fromPolymorphicVariant(const PolymorphicTypes::Variant &data)
{
    switch (data.type())
    {
        case PolymorphicTypes::BasicType::Double: return QVariant(data.toDouble());
        case PolymorphicTypes::BasicType::Float: return QVariant(data.toFloat());
        case PolymorphicTypes::BasicType::Integer: return QVariant(data.toInt());
        case PolymorphicTypes::BasicType::String: return QVariant(QString::fromStdString(data.toString()));
        default: return {};
    }
}

GraphLayout::GraphLayout(PositionedGraph *graph)
    : graph(graph)
    , nodesModel(nullptr)
    , edgesModel(nullptr)
{}

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
    bool indexOkay = index > -1 && index < nodesCount();
    if (!indexOkay) return {};

    auto data = graph->node(index);
    return fromPolymorphicVariant(data);
}

QVariant GraphLayout::edgeProperties(int index) const
{
    bool indexOkay = index > -1 && index < nodesCount();
    if (!indexOkay) return {};

    auto data = graph->edgeProperties(index);
    return fromPolymorphicVariant(data);
}

QPair<int, int> GraphLayout::edge(int index) const
{
    auto edge = graph->edge(index);
    return qMakePair(edge.first, edge.second);
}

double GraphLayout::getNodeXPosition(int index) const
{
    bool indexOkay = index > -1 && index < nodesCount();
    return indexOkay ? graph->nodePosition(index).x() : -1;
}

double GraphLayout::getNodeYPosition(int index) const
{
    bool indexOkay = index > -1 && index < nodesCount();
    return indexOkay ? graph->nodePosition(index).y() : -1;
}

void GraphLayout::setNodeXPosition(int index, double x)
{
    if (!qFuzzyCompare(getNodeXPosition(index), x))
    {
        graph->setNodePosition(index, GraphGeometry::Point(x, getNodeYPosition(index)));
        emit positionUpdated(index);
    }
}

void GraphLayout::setNodeYPosition(int index, double y)
{
    if (!qFuzzyCompare(getNodeYPosition(index), y))
    {
        graph->setNodePosition(index, GraphGeometry::Point(getNodeXPosition(index), y));
        emit positionUpdated(index);
    }
}

void GraphLayout::recalculatePositions()
{ 
    calculator->calculate(graph);
    emit positionsUpdated();
//    auto *calc = new GraphCalculator(graph, positions, positionsLock, config);
//#ifndef DISABLE_THREADS
//    connect(calc, &GraphCalculator::finished, this, &GraphLayout::positionsUpdated);
//    connect(calc, &GraphCalculator::updated,  this, &GraphLayout::positionsUpdated);
//    calc->setAutoDelete(true);
//    QThreadPool::globalInstance()->start(calc);
//#else
//    calc->run();
//    delete calc;
//    emit positionsUpdated();
//#endif // DISABLE_THREADS
}

void GraphLayout::setRandomPositions()
{
    auto area = 1000;   // TODO: fix
    for (int i = 0; i < graph->nodesCount(); ++i)
    {
        double x = rand() % int(area);
        double y = rand() % int(area);
        graph->setNodePosition(i, GraphGeometry::Point(x, y));
    }
    emit positionsUpdated();
}

void GraphLayout::setGraphCalculator(GraphCalculator *calculator)
{
    this->calculator = calculator;
}

void GraphLayout::setNodeSize(int width, int height)
{
    this->nodeWidth = width;
    this->nodeHeight = height;
}

void GraphLayout::setFrameSize(int width, int height)
{
    this->frameWidth = width;
    this->frameHeight = height;
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

void GraphLayout::setGraph(PositionedGraph *graph)
{
    this->graph = graph;
    emit modelUpdated();
}

void GraphLayout::onGraphChanged()
{
    setRandomPositions();
}
