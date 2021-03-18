#ifndef IGRAPH_H
#define IGRAPH_H

#include <QVariant>

///
/// \brief Interface for the graph to be used in GraphCalculator
///
class IGraph
{
public:
    virtual ~IGraph() = default;
    ///
    /// \brief Getter for the number of available nodes
    /// \return The number of the available nodes
    ///
    virtual int nodesCount() const = 0;
    ///
    /// \brief Getter for the number of the available edges
    /// \return The number of the available edges
    ///
    virtual int edgesCount() const = 0;
    ///
    /// \brief Getter for the information about the node
    /// \param index Index of the node
    /// \return QVariant with the information about the node
    ///
    virtual QVariant node(int index) const = 0;
    ///
    /// \brief Getter for the edge by its index
    /// \param index Index of the edge
    /// \return QPair with the start node index and finish node index
    ///
    virtual QPair<int, int> edge(int index) const = 0;
    ///
    /// \brief Getter for the information about the edge
    /// \param index Index of the edge
    /// \return QVariant with the information about the edge
    ///
    virtual QVariant edgeProperties(int index) const = 0;

    ///
    /// \brief Checks if two nodes are connected with each other
    /// \param a first node index
    /// \param b second node index
    /// \return true if nodes are connected, false otherwise
    ///
    virtual bool isAdjacent(int a, int b) const = 0;
};

#endif // IGRAPH_H
