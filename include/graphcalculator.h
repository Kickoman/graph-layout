#ifndef GRAPHCALCULATOR_H
#define GRAPHCALCULATOR_H

#include "2d/point.h"
#include "igraph.h"
#include "kfunction.h"
#include <atomic>
#include <memory>

struct GraphCalculatorConfig
{
    Function<double> repulsiveForce;
    Function<double> attractiveForce;
    Function<double> edgesRepulsiveForce;
    Function<double> linesRepulsiveForce;

    double nodeWidth = 100;
    double nodeHeight = 100;

    double frameWidth = 1000;
    double frameHeight = 1000;

    bool enableThrottling = true;
};


class GraphCalculator
{
public:
    using TPoint = GraphGeometry::D2::Point;
    using TGraph = IGraph<TPoint>;
    using TGraphPtr = std::shared_ptr<TGraph>;

    GraphCalculator() = default;
    GraphCalculator(const TGraphPtr &graph, GraphCalculatorConfig config);
    void setGraph(const TGraphPtr &graph);
    void setConfig(GraphCalculatorConfig config);
    void run();
    void requestStop();

private:
    static const double kMinimalTemperature;
    static const double kTemperatureDecreasingFactor;
    static const int kDelayUs;
    static const int kMaxDegrees;
    static const int kRightAngleDeg;

    TGraphPtr graph;
    GraphCalculatorConfig config;
    std::atomic<bool> stopRequested;
};

#endif // GRAPHCALCULATOR_H
