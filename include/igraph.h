#ifndef IGRAPH_H
#define IGRAPH_H

#include <utility>

///
/// \brief Interface for the graph to be used in GraphCalculator
///
template<class TPointType>
class IGraph
{
public:
    using PointType = TPointType;

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
    /// \brief Getter for the edge by its index
    /// \param index Index of the edge
    /// \return QPair with the start node index and finish node index
    ///
    virtual std::pair<int, int> edge(int index) const = 0;
    ///
    /// \brief Retrieves node position by node index
    /// \param index Index of the node
    /// \return TPointType with the position of the node
    ///
    virtual TPointType nodePosition(int index) const = 0;
    ///
    /// \brief Setter for the node position by its index
    /// \param index Index of the node
    /// \param point TPointType with position of the node
    ///
    virtual void setNodePosition(int index, TPointType point) = 0;
    ///
    /// \brief Some callback for finishing epoch
    ///
    virtual void flush() = 0;
};

#endif // IGRAPH_H
