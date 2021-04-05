#ifndef FORCEDGRAPHCALCULATOR_H
#define FORCEDGRAPHCALCULATOR_H

#include "graphcalculator.h"
#include "twodvector.h"

class ForcedGraphCalculator : public GraphCalculator
{
public:
    struct ForcedGraphConfig
    {
        std::function<double(const GraphCalculatorConfig&)> initialTemperature
            = [](const GraphCalculatorConfig &config) -> double
        {
            return sqrt(config.frameHeight * config.frameHeight
                        + config.frameWidth * config.frameWidth);
        };

        std::function<double(const GraphCalculatorConfig&)> minimalTemperature
            = [](const GraphCalculatorConfig &) -> double
        {
            return 10;
        };

        std::function<double(const GraphCalculatorConfig&)> temperatureDecreasingFactor
            = [](const GraphCalculatorConfig &) -> double
        {
            return 1.01;
        };
        std::function<double(double)> frameEdgesRepulsiveForce;
        std::function<double(double)> linesRepulsiveForce;
    };

    ForcedGraphCalculator() = default;
    ForcedGraphCalculator(const GraphCalculatorConfig &config);

    void setForces(const ForcedGraphConfig &forces);

    void calculate(PositionedGraph *graph) override;

    virtual void iteration(double temperature, PositionedGraph *graph);

private:
    bool prepareConfigs();

    std::vector<GraphGeometry::TwoDVector> calculateRepulsiveForces(PositionedGraph *graph);
    std::vector<GraphGeometry::TwoDVector> calculateAttractiveForces(PositionedGraph *graph);
    std::vector<GraphGeometry::TwoDVector> calculateFrameRepulsiveForces(PositionedGraph *graph);
    std::vector<GraphGeometry::TwoDVector> calculateLinesRepulsiveForces(PositionedGraph *graph);

    ForcedGraphConfig forcesConfig;
};

#endif // FORCEDGRAPHCALCULATOR_H
