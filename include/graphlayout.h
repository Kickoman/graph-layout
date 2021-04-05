#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QObject>
#include <QVector>
#include "point.h"
#include <QMutex>
#include <QAbstractListModel>
#include "igraph.h"
#include "graphcalculator.h"

///
/// \brief The model for the GraphView QML object, which
///        stores the positions of the nodes in graph
///
class GraphLayout : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int nodesCount READ nodesCount NOTIFY modelUpdated)
    Q_PROPERTY(int edgesCount READ edgesCount NOTIFY modelUpdated)
    Q_PROPERTY(QVariant nodesModel READ getNodesModel NOTIFY modelUpdated)
    Q_PROPERTY(QVariant edgesModel READ getEdgesModel NOTIFY modelUpdated)
public:
    GraphLayout(PositionedGraph *graph);

    ///
    /// \brief Getter for the number of nodes in graph
    /// \return Number of nodes
    ///
    Q_INVOKABLE
    int nodesCount() const;

    ///
    /// \brief Getter for the number of edges in graph
    /// \return Number of edges
    ///
    Q_INVOKABLE
    int edgesCount() const;

    QVariant node(int index) const;
    QVariant edgeProperties(int index) const;

    ///
    /// \brief Getter for the edge by its index
    /// \param index Index of the edge
    /// \return QPair with the start node index and finish node index
    ///
    Q_INVOKABLE
    QPair<int, int> edge(int index) const;

    ///
    /// \brief Getter for the X coordinate of the node by its index
    /// \param index Index of the node
    /// \return X coordinate of the node
    ///
    Q_INVOKABLE
    double getNodeXPosition(int index) const;

    ///
    /// \brief Getter for the Y coordinate of the node by its index
    /// \param index Index of the node
    /// \return Y coordinate of the node
    ///
    Q_INVOKABLE
    double getNodeYPosition(int index) const;

    ///
    /// \brief Setter for the X coordinate of the node
    /// \param index Index of the node
    /// \param x coordinate of the node
    ///
    Q_INVOKABLE
    void setNodeXPosition(int index, double x);

    ///
    /// \brief Setter for the Y coordinate of the node
    /// \param index Index of the node
    /// \param y coordinate of the node
    ///
    Q_INVOKABLE
    void setNodeYPosition(int index, double y);

    ///
    /// \brief Starts recalculating the positions of the nodes
    ///        using force-based algorithm
    ///
    Q_INVOKABLE
    void recalculatePositions();

    ///
    /// \brief Sets random positions for nodes
    ///
    Q_INVOKABLE
    void setRandomPositions();

    void setGraphCalculator(GraphCalculator *calculator);

    ///
    /// \brief Setter for the actual node size (used in calculation algorithm)
    /// \param width Node width
    /// \param height Node height
    ///
    Q_INVOKABLE
    void setNodeSize(int width, int height);

    ///
    /// \brief Setter for the size of the field where the graph should be drawn
    /// \param width Field width
    /// \param height Field height
    ///
    Q_INVOKABLE
    void setFrameSize(int width, int height);

    ///
    /// \brief Getter for the nodes list model
    /// \return QVariant with the nodes list model
    ///
    Q_INVOKABLE
    QVariant getNodesModel();

    ///
    /// \brief Getter for the edges list model
    /// \return QVariant with the edges list model
    ///
    Q_INVOKABLE
    QVariant getEdgesModel();

    ///
    /// \brief Setter for the graph to be used
    /// \param graph a pointer to a new graph
    ///
    void setGraph(PositionedGraph *graph);
signals:
    void positionsUpdated();
    void positionUpdated(int nodeIndex);
    void modelUpdated();

private slots:
    void onGraphChanged();

private:
    GraphCalculator *calculator;
    PositionedGraph *graph;

    int nodeWidth = 100;
    int nodeHeight = 100;
    int frameWidth = 100;
    int frameHeight = 100;

    QAbstractListModel *nodesModel;
    QAbstractListModel *edgesModel;
};

#endif // GRAPHMODEL_H
