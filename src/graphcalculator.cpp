#include "graphcalculator.h"
#include "2d/point.h"
#include <cmath>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>
#include <chrono>
#include <utility>
#include <limits>

#include "2d/vector.h"
#include "2d/section.h"
#include "2d/line.h"

const double GraphCalculator::kMinimalTemperature(10);
const double GraphCalculator::kTemperatureDecreasingFactor(1.01);
const int GraphCalculator::kDelayUs(50000);
const int GraphCalculator::kMaxDegrees(360);
const int GraphCalculator::kRightAngleDeg(90);

template<class X>
bool fuzzyCompare(const X a, const X b) {
    constexpr auto relativeDifferenceFactor = 0.0001;
    const auto greaterMagnitude = std::max(std::abs(a), std::abs(b));
    return std::abs(a - b) <= relativeDifferenceFactor * greaterMagnitude;
}

GraphCalculator::GraphCalculator(TGraph *graph, GraphCalculatorConfig config)
    : graph(graph)
    , config(std::move(config))
    , stopRequested(false)
{}

void GraphCalculator::setGraph(TGraph *graph) {
    this->graph = graph;
}

void GraphCalculator::setConfig(GraphCalculatorConfig config) {
    this->config = std::move(config);
}

void GraphCalculator::requestStop() {
    stopRequested.store(true);
}

void GraphCalculator::run()
{
    stopRequested.store(false);
    double temperature = sqrt(config.frameHeight * config.frameHeight
                              + config.frameWidth * config.frameWidth);

    const int nodesCount = graph->nodesCount();
    std::vector<GraphGeometry::D2::Vector> forces(nodesCount);
    while (temperature > kMinimalTemperature && !stopRequested.load())
    {
        std::fill(forces.begin(), forces.end(), GraphGeometry::D2::Vector(0, 0));

        // Repulsive forces between each pair of vertices
        const int NODES_COUNT = graph->nodesCount();
        for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
        {
            for (int otherNode = 0; otherNode < NODES_COUNT; ++otherNode)
            {
                if (targetNode == otherNode) continue;

                const auto myPosition = graph->nodePosition(targetNode);
                const auto itPosition = graph->nodePosition(otherNode);

                // Direction vector should be directed FROM other node,
                // because this is the repulsive force, so it should
                // move target node onto a larger distance from other node.
                auto directionVector = GraphGeometry::D2::Vector(itPosition, myPosition);
                auto distance = directionVector.magnitude();
                if (fuzzyCompare(distance, 0.)) {
                    distance = std::numeric_limits<int>::max();
                }
                auto forceScalar = config.repulsiveForce(distance);
                if (std::isnan(forceScalar) || std::isinf(forceScalar)) {
                    forceScalar = std::numeric_limits<int>::max();
                }
                auto forceVector = directionVector / distance * forceScalar;
                forces[targetNode] += forceVector;
            }
        }

        // Attractive forces between connected vertices
        const int EDGES_COUNT = graph->edgesCount();
        for (int targetEdge = 0; targetEdge < EDGES_COUNT; ++targetEdge)
        {
            auto currentEdge = graph->edge(targetEdge);

            const auto position1 = graph->nodePosition(currentEdge.first);
            const auto position2 = graph->nodePosition(currentEdge.second);

            auto directionVector1 = GraphGeometry::D2::Vector(position1, position2);
            auto directionVector2 = GraphGeometry::D2::Vector(position2, position1);
            auto distance = directionVector1.magnitude();

            if (fuzzyCompare(distance, 0.))
            {
                distance = std::numeric_limits<int>::max();
                directionVector1 = GraphGeometry::D2::Vector(1, 0).rotateDeg(rand() % kMaxDegrees);
                directionVector2 = GraphGeometry::D2::Vector(1, 0).rotateDeg(rand() % kMaxDegrees);
            }
            else
            {
                directionVector1 = directionVector1 / distance;
                directionVector2 = directionVector2 / distance;
            }

            auto forceScalar = config.attractiveForce(distance);
            if (std::isnan(forceScalar) || std::isinf(forceScalar))
                forceScalar = std::numeric_limits<int>::max();

            const auto forceVector1 = directionVector1 * forceScalar;
            const auto forceVector2 = directionVector2 * forceScalar;

            forces[currentEdge.first] += forceVector1;
            forces[currentEdge.second] += forceVector2;
        }

        // Compute repulsive forces from edges
        auto nearestPoint = [](const GraphGeometry::D2::Section &section, GraphGeometry::D2::Point point) -> GraphGeometry::D2::Point {
            GraphGeometry::D2::Vector direction(section.start(), section.end());
            direction = direction.rotateDeg(kRightAngleDeg);
            GraphGeometry::D2::Line perpendicular(point, direction);
            GraphGeometry::D2::Line mainLIne(section.start(), section.end());

            bool intersects = false;
            auto intersection = perpendicular.intersection(mainLIne, &intersects);

            if (!intersects)
                return point;

            if (section.has(intersection))
                return intersection;

            GraphGeometry::D2::Vector checkStart(point, section.start());
            GraphGeometry::D2::Vector checkEnd(point, section.end());

            if (checkStart.magnitude() < checkEnd.magnitude())
                return section.start();
            return section.end();
        };

        for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
        {
            const auto nodePosition = graph->nodePosition(targetNode);
            for (int targetEdge = 0; targetEdge < EDGES_COUNT; ++targetEdge)
            {
                auto edge = graph->edge(targetEdge);
                if (targetNode == edge.first || targetNode == edge.second) continue;

                const auto sectionStart = graph->nodePosition(edge.first);
                const auto sectionEnd = graph->nodePosition(edge.second);
                GraphGeometry::D2::Section section(sectionStart, sectionEnd);
                auto point = nearestPoint(section, nodePosition);

                GraphGeometry::D2::Vector forceDirectionVector(point, nodePosition);
                auto distance = forceDirectionVector.magnitude();
                if (fuzzyCompare(distance, 0.))
                {
                    distance = 0.0;
                    forceDirectionVector = GraphGeometry::D2::Vector(1, 0).rotateDeg(rand() % kMaxDegrees);
                }
                else
                    forceDirectionVector = forceDirectionVector / forceDirectionVector.magnitude();

                auto forceScalar = config.linesRepulsiveForce(distance);
                if (std::isnan(forceScalar) || std::isinf(forceScalar))
                    forceScalar = std::numeric_limits<int>::max();
                auto forceVector = forceDirectionVector * forceScalar;
                forces[targetNode] += forceVector;
            }
        }

        // Compute repulsive forces from frame edges`
        auto computeForce
            = [this](double distance, GraphGeometry::D2::Vector direction) -> GraphGeometry::D2::Vector {
            if (fuzzyCompare(distance, 0.))
                distance = std::numeric_limits<int>::max();
            auto forceScalar = config.edgesRepulsiveForce(distance);
            if (std::isnan(forceScalar) || std::isinf(forceScalar))
                forceScalar = std::numeric_limits<int>::max();
            return direction / direction.magnitude() * forceScalar;
        };
        for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
        {
            const auto pos = graph->nodePosition(targetNode);
            forces[targetNode] += computeForce(pos.y(), {0, 1});                        // from top
            forces[targetNode] += computeForce(config.frameHeight - pos.y(), {0, -1});  // from bottom
            forces[targetNode] += computeForce(pos.x(), {1, 0});                        // from left
            forces[targetNode] += computeForce(config.frameWidth - pos.x(), {-1, 0});
        }

        // Apply forces to nodes
        for (int node = 0; node < NODES_COUNT; ++node)
        {
            double x = graph->nodePosition(node).x();
            double y = graph->nodePosition(node).y();

            x += std::min(temperature, forces.at(node).x());
            y += std::min(temperature, forces.at(node).y());

            x = std::max(config.nodeWidth / 2, x);
            x = std::min(config.frameWidth - config.nodeWidth / 2, x);
            y = std::max(config.nodeHeight / 2, y);
            y = std::min(config.frameHeight - config.nodeHeight / 2, y);

            graph->setNodePosition(node, GraphGeometry::D2::Point(x, y));
        }

        graph->flush();
        if (config.enableThrottling) {
            std::this_thread::sleep_for(std::chrono::microseconds(kDelayUs));
        }

        temperature /= kTemperatureDecreasingFactor;
    }
}
