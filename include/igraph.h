#ifndef IGRAPH_H
#define IGRAPH_H

#include <utility>

#include "variant.h"
#include "point.h"

///
/// \brief Interface for the graph to be used in GraphCalculator
///
class PositionedGraph
{
public:
    virtual ~PositionedGraph() = default;
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
    /// \brief Getter for the edge by its index
    /// \param index Index of the edge
    /// \return QPair with the start node index and finish node index
    ///
    virtual std::pair<int, int> edge(int index) const = 0;
    virtual PolymorphicTypes::Variant node(int index) const = 0;
    virtual PolymorphicTypes::Variant edgeProperties(int index) const = 0;
    ///
    /// \brief Getter for the node position
    /// \param index Index of the node
    /// \return Point structure with current node position
    ///
    virtual GraphGeometry::Point nodePosition(int index) const = 0;
    ///
    /// \brief Setter for the node position
    /// \param index Index of the node
    /// \param position Point structure with new node position
    ///
    virtual void setNodePosition(int index, GraphGeometry::Point position) = 0;
    ///
    /// \brief Checks if two nodes are connected with each other
    /// \param a first node index
    /// \param b second node index
    /// \return true if nodes are connected, false otherwise
    ///
    virtual bool isAdjacent(int a, int b) const = 0;
};

#endif // IGRAPH_H
