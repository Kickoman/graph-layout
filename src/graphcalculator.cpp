#include "graphcalculator.h"
#include <array>
#define _USE_MATH_DEFINES
#include <math.h>
#include <QDebug>
#include <thread>
#include <QThread>
#include <chrono>
#include <utility>

#include "twodvector.h"
#include "section.h"
#include "line.h"

Q_DECLARE_TYPEINFO(GraphGeometry::TwoDVector, Q_MOVABLE_TYPE);

const double GraphCalculator::kMinimalTemperature(10);
const double GraphCalculator::kTemperatureDecreasingFactor(1.01);
const int GraphCalculator::kDelayUs(50000);
const int GraphCalculator::kMaxDegrees(360);
const int GraphCalculator::kRightAngleDeg(90);

GraphCalculator::GraphCalculator(IGraph *graph,
                                 QVector<GraphGeometry::Point> &positions,
                                 QMutex &lock,
                                 GraphCalculatorConfig config)
    : graph(graph)
    , mutex(lock)
    , positions(positions)
    , config(std::move(config))
{
}

void GraphCalculator::run()
{
    double temperature = sqrt(config.frameHeight * config.frameHeight
                              + config.frameWidth * config.frameWidth);

    while (temperature > kMinimalTemperature)
    {
        QMutexLocker lock(&mutex);

        const int NODES_COUNT = graph->nodesCount();
        QVector<GraphGeometry::TwoDVector> forces(NODES_COUNT, {0, 0});

        // Repulsive forces between each pair of vertices
        for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
        {
            for (int otherNode = 0; otherNode < NODES_COUNT; ++otherNode)
            {
                if (targetNode == otherNode) continue;

                auto myPosition = positions.at(targetNode);
                auto itPosition = positions.at(otherNode);
                // Direction vector should be directed FROM other node,
                // because this is the repulsive force, so it should
                // move target node onto a larger distance from other node.
                auto directionVector = GraphGeometry::TwoDVector(itPosition, myPosition);
                auto distance = directionVector.magnitude();
                if (qFuzzyCompare(distance, 0))
                    distance = static_cast<double>(INT_MAX);
                auto forceScalar = config.repulsiveForce(distance);
                if (qIsNaN(forceScalar) || qIsInf(forceScalar))
                    forceScalar = static_cast<double>(INT_MAX); // some big value
                auto forceVector = directionVector / distance * forceScalar;
                forces[targetNode] += forceVector;
            }
        }

        // Attractive forces between connected vertices
        const int EDGES_COUNT = graph->edgesCount();
        for (int targetEdge = 0; targetEdge < EDGES_COUNT; ++targetEdge)
        {
            auto currentEdge = graph->edge(targetEdge);
            auto position1 = positions.at(currentEdge.first);
            auto position2 = positions.at(currentEdge.second);

            auto directionVector1 = GraphGeometry::TwoDVector(position1, position2);
            auto directionVector2 = GraphGeometry::TwoDVector(position2, position1);
            auto distance = directionVector1.magnitude();

            if (qFuzzyCompare(distance, 0))
            {
                distance = static_cast<double>(INT_MAX);
                directionVector1 = GraphGeometry::TwoDVector(1, 0).rotateDeg(rand() % kMaxDegrees);
                directionVector2 = GraphGeometry::TwoDVector(1, 0).rotateDeg(rand() % kMaxDegrees);
            }
            else
            {
                directionVector1 = directionVector1 / distance;
                directionVector2 = directionVector2 / distance;
            }

            auto forceScalar = config.attractiveForce(distance);
            if (qIsNaN(forceScalar) || qIsInf(forceScalar))
                forceScalar = static_cast<double>(INT_MAX);

            auto forceVector1 = directionVector1 * forceScalar;
            auto forceVector2 = directionVector2 * forceScalar;

            forces[currentEdge.first] += forceVector1;
            forces[currentEdge.second] += forceVector2;
        }

        // Compute repulsive forces from edges
        auto nearestPoint = [](const GraphGeometry::Section &section, GraphGeometry::Point point) -> GraphGeometry::Point {
            GraphGeometry::TwoDVector direction(section.start(), section.end());
            direction = direction.rotateDeg(kRightAngleDeg);
            GraphGeometry::Line perpendicular(point, direction);
            GraphGeometry::Line mainLIne(section.start(), section.end());

            bool intersects = false;
            auto intersection = perpendicular.intersection(mainLIne, &intersects);

            if (!intersects)
                return point;

            if (section.has(intersection))
                return intersection;

            GraphGeometry::TwoDVector checkStart(point, section.start());
            GraphGeometry::TwoDVector checkEnd(point, section.end());

            if (checkStart.magnitude() < checkEnd.magnitude())
                return section.start();
            return section.end();
        };

        for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
        {
            auto nodePosition = positions.at(targetNode);
            for (int targetEdge = 0; targetEdge < EDGES_COUNT; ++targetEdge)
            {
                auto edge = graph->edge(targetEdge);
                if (targetNode == edge.first || targetNode == edge.second) continue;

                auto sectionStart = positions.at(edge.first);
                auto sectionEnd = positions.at(edge.second);
                GraphGeometry::Section section(sectionStart, sectionEnd);
                auto point = nearestPoint(section, nodePosition);

                GraphGeometry::TwoDVector forceDirectionVector(point, nodePosition);
                auto distance = forceDirectionVector.magnitude();
                if (qFuzzyCompare(distance, 0))
                {
                    distance = static_cast<double>(0);
                    forceDirectionVector = GraphGeometry::TwoDVector(1, 0).rotateDeg(rand() % kMaxDegrees);
                }
                else
                    forceDirectionVector = forceDirectionVector / forceDirectionVector.magnitude();

                auto forceScalar = config.linesRepulsiveForce(distance);
                if (qIsNaN(forceScalar) || qIsInf(forceScalar))
                    forceScalar = static_cast<double>(INT_MAX);
                auto forceVector = forceDirectionVector * forceScalar;
                forces[targetNode] += forceVector;
            }
        }

        // Compute repulsive forces from frame edges`
        auto computeForce
            = [this](double distance, GraphGeometry::TwoDVector direction) -> GraphGeometry::TwoDVector {
            if (qFuzzyCompare(distance, 0))
                distance = static_cast<double>(INT_MAX);
            auto forceScalar = config.edgesRepulsiveForce(distance);
            if (qIsNaN(forceScalar) || qIsInf(forceScalar))
                forceScalar = static_cast<double>(INT_MAX);
            return direction / direction.magnitude() * forceScalar;
        };
        for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
        {
            auto pos = positions.at(targetNode);
            forces[targetNode] += computeForce(pos.y(), {0, 1});                        // from top
            forces[targetNode] += computeForce(config.frameHeight - pos.y(), {0, -1});  // from bottom
            forces[targetNode] += computeForce(pos.x(), {1, 0});                        // from left
            forces[targetNode] += computeForce(config.frameWidth - pos.x(), {-1, 0});
        }

        // Apply forces to nodes
        for (int node = 0; node < NODES_COUNT; ++node)
        {
            double x = positions.at(node).x();
            double y = positions.at(node).y();

            x += std::min(temperature, forces.at(node).x());
            y += std::min(temperature, forces.at(node).y());

            x = std::max(config.nodeWidth / 2, x);
            x = std::min(config.frameWidth - config.nodeWidth / 2, x);
            y = std::max(config.nodeHeight / 2, y);
            y = std::min(config.frameHeight - config.nodeHeight / 2, y);

            positions[node] = GraphGeometry::Point(x, y);
        }

        lock.unlock();
#ifndef WASM_BUILD
        emit updated();
        QThread::usleep(kDelayUs);
#endif // WASM_BUILD
        temperature /= kTemperatureDecreasingFactor;
    }
    emit finished();
}
