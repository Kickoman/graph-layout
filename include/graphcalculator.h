#ifndef GRAPHCALCULATOR_H
#define GRAPHCALCULATOR_H

#include <functional>
#include <vector>

#include "igraph.h"
#include "point.h"

struct GraphCalculatorConfig
{
    std::function<double(double)> repulsiveForce;
    std::function<double(double)> attractiveForce;

    double nodeWidth = 100;
    double nodeHeight = 100;

    double frameWidth = 1000;
    double frameHeight = 1000;
};

class GraphCalculator
{
public:
    GraphCalculator() = default;
    GraphCalculator(const GraphCalculatorConfig &config);

    void setConfig(const GraphCalculatorConfig &config);
    virtual void calculate(PositionedGraph *) = 0;

    bool isRunning() const;

protected:
    bool validateConfig();

    volatile bool running = false;
    GraphCalculatorConfig config;
};

#endif // GRAPHCALCULATOR_H
