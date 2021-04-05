#include "forcedgraphcalculator.h"
#include "section.h"
#include "line.h"

#include <algorithm>
#include <cmath>

bool fuzzyCompare(double a, double b)
{
    return std::fabs(a - b) < 1e-5;
}

ForcedGraphCalculator::ForcedGraphCalculator(const GraphCalculatorConfig &config)
    : GraphCalculator(config)
{
}

void ForcedGraphCalculator::setForces(const ForcedGraphCalculator::ForcedGraphConfig &forces)
{
    forcesConfig = forces;
}

void ForcedGraphCalculator::calculate(PositionedGraph *graph)
{
    if (!prepareConfigs())
        return;

    running = true;
    const double minimalTemperature = forcesConfig.minimalTemperature(config);
    double temperature = forcesConfig.initialTemperature(config);

    while (temperature > minimalTemperature)
    {
        iteration(temperature, graph);
        temperature /= forcesConfig.temperatureDecreasingFactor(config);
    }
    running = false;
}

void ForcedGraphCalculator::iteration(double temperature, PositionedGraph *graph)
{
    auto attractiveForces = calculateAttractiveForces(graph);
    auto repulsiveForces = calculateRepulsiveForces(graph);
    auto frameRepulsiveForces = calculateFrameRepulsiveForces(graph);
    auto linesRepulsiveForces = calculateLinesRepulsiveForces(graph);

    const auto nodesCount = graph->nodesCount();
    for (int node = 0; node < nodesCount; ++node)
    {
        double x = graph->nodePosition(node).x();
        double y = graph->nodePosition(node).y();

        GraphGeometry::TwoDVector totalForce
                = attractiveForces.at(node)
                + repulsiveForces.at(node)
                + frameRepulsiveForces.at(node)
                + linesRepulsiveForces.at(node);

        x += std::min(temperature, totalForce.x());
        y += std::min(temperature, totalForce.y());

        x = std::max(config.nodeWidth / 2, x);
        x = std::min(config.frameWidth - config.nodeWidth / 2, x);
        y = std::max(config.nodeHeight / 2, y);
        y = std::min(config.frameHeight - config.nodeHeight / 2, y);

        graph->setNodePosition(node, GraphGeometry::Point(x, y));
    }
}

bool ForcedGraphCalculator::prepareConfigs()
{
    if (!GraphCalculator::validateConfig()) return false;
    if (!forcesConfig.linesRepulsiveForce)
    {
        forcesConfig.linesRepulsiveForce = [this](double distance) -> double
        {
            static const double kLinesRepulsiveDecreasingFactor = 10;
            return this->config.repulsiveForce(distance) / kLinesRepulsiveDecreasingFactor;
        };
    }
    if (!forcesConfig.frameEdgesRepulsiveForce)
    {
        forcesConfig.frameEdgesRepulsiveForce = [this](double distance) -> double
        {
            static const double kFramesRepulsiveDecreasingFactor = 7;
            return this->config.repulsiveForce(distance) / kFramesRepulsiveDecreasingFactor;
        };
    }
    return true;
}

std::vector<GraphGeometry::TwoDVector> ForcedGraphCalculator::calculateRepulsiveForces(PositionedGraph *graph)
{
    const int NODES_COUNT = graph->nodesCount();
    std::vector<GraphGeometry::TwoDVector> forces(graph->nodesCount());

    for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
    {
        for (int otherNode = 0; otherNode < NODES_COUNT; ++otherNode)
        {
            if (targetNode == otherNode) continue;

            auto myPosition = graph->nodePosition(targetNode);
            auto itPosition = graph->nodePosition(otherNode);

            // Direction vector should be directed FROM other node,
            // because this is the repulsive force, so it should
            // move target node onto a larger distance from other node.
            auto directionVector = GraphGeometry::TwoDVector(itPosition, myPosition);
            auto distance = directionVector.magnitude();
            if (fuzzyCompare(distance, 0))
                distance = static_cast<double>(INT_MAX);
            auto forceScalar = config.repulsiveForce(distance);
            if (isnan(forceScalar) || isinf(forceScalar))
                forceScalar = static_cast<double>(INT_MAX); // some big value
            auto forceVector = directionVector / distance * forceScalar;
            forces[targetNode] += forceVector;
        }
    }

    return forces;
}

std::vector<GraphGeometry::TwoDVector> ForcedGraphCalculator::calculateAttractiveForces(PositionedGraph *graph)
{
    static const int kMaxDegrees = 360;

    const int EDGES_COUNT = graph->edgesCount();
    std::vector<GraphGeometry::TwoDVector> forces(graph->nodesCount());
    for (int targetEdge = 0; targetEdge < EDGES_COUNT; ++targetEdge)
    {
        auto currentEdge = graph->edge(targetEdge);
        auto position1 = graph->nodePosition(currentEdge.first);
        auto position2 = graph->nodePosition(currentEdge.second);

        auto directionVector1 = GraphGeometry::TwoDVector(position1, position2);
        auto directionVector2 = GraphGeometry::TwoDVector(position2, position1);
        auto distance = directionVector1.magnitude();

        if (fuzzyCompare(distance, 0))
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
        if (isnan(forceScalar) || isinf(forceScalar))
            forceScalar = static_cast<double>(INT_MAX);

        auto forceVector1 = directionVector1 * forceScalar;
        auto forceVector2 = directionVector2 * forceScalar;

        forces[currentEdge.first] += forceVector1;
        forces[currentEdge.second] += forceVector2;
    }
    return forces;
}

std::vector<GraphGeometry::TwoDVector> ForcedGraphCalculator::calculateFrameRepulsiveForces(PositionedGraph *graph)
{
    const auto NODES_COUNT = graph->nodesCount();
    std::vector<GraphGeometry::TwoDVector> forces(graph->nodesCount());

    auto computeForce
        = [this](double distance, GraphGeometry::TwoDVector direction) -> GraphGeometry::TwoDVector {
        if (fuzzyCompare(distance, 0))
            distance = static_cast<double>(INT_MAX);
        auto forceScalar = forcesConfig.frameEdgesRepulsiveForce(distance);
        if (isnan(forceScalar) || isinf(forceScalar))
            forceScalar = static_cast<double>(INT_MAX);
        return direction / direction.magnitude() * forceScalar;
    };
    for (int targetNode = 0; targetNode < NODES_COUNT; ++targetNode)
    {
        auto pos = graph->nodePosition(targetNode);
        forces[targetNode] += computeForce(pos.y(), {0, 1});                        // from top
        forces[targetNode] += computeForce(config.frameHeight - pos.y(), {0, -1});  // from bottom
        forces[targetNode] += computeForce(pos.x(), {1, 0});                        // from left
        forces[targetNode] += computeForce(config.frameWidth - pos.x(), {-1, 0});
    }

    return forces;
}

std::vector<GraphGeometry::TwoDVector> ForcedGraphCalculator::calculateLinesRepulsiveForces(PositionedGraph *graph)
{
    static const int kMaxDegrees = 360;
    const auto NODES_COUNT = graph->nodesCount();
    const auto EDGES_COUNT = graph->edgesCount();
    std::vector<GraphGeometry::TwoDVector> forces(graph->nodesCount());

    auto nearestPoint = [](const GraphGeometry::Section &section, GraphGeometry::Point point) -> GraphGeometry::Point {
        static const int kRightAngleDeg = 90;
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
        auto nodePosition = graph->nodePosition(targetNode);
        for (int targetEdge = 0; targetEdge < EDGES_COUNT; ++targetEdge)
        {
            auto edge = graph->edge(targetEdge);
            if (targetNode == edge.first || targetNode == edge.second) continue;

            auto sectionStart = graph->nodePosition(edge.first);
            auto sectionEnd = graph->nodePosition(edge.second);
            GraphGeometry::Section section(sectionStart, sectionEnd);
            auto point = nearestPoint(section, nodePosition);

            GraphGeometry::TwoDVector forceDirectionVector(point, nodePosition);
            auto distance = forceDirectionVector.magnitude();
            if (fuzzyCompare(distance, 0))
            {
                distance = static_cast<double>(INT_MAX);
                forceDirectionVector = GraphGeometry::TwoDVector(1, 0).rotateDeg(rand() % kMaxDegrees);
            }
            else
                forceDirectionVector = forceDirectionVector / forceDirectionVector.magnitude();

            auto forceScalar = forcesConfig.frameEdgesRepulsiveForce(distance);
            if (isnan(forceScalar) || isinf(forceScalar))
                forceScalar = static_cast<double>(INT_MAX);
            auto forceVector = forceDirectionVector * forceScalar;
            forces[targetNode] += forceVector;
        }
    }

    return forces;
}
