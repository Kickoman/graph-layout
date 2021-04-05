#include "graphcalculator.h"

GraphCalculator::GraphCalculator(const GraphCalculatorConfig &config)
{
    setConfig(config);
}

void GraphCalculator::setConfig(const GraphCalculatorConfig &config)
{
    this->config = config;
}

bool GraphCalculator::isRunning() const
{
    return running;
}

bool GraphCalculator::validateConfig()
{
    if (config.frameHeight < 0) return false;
    if (config.frameWidth < 0) return false;
    if (config.nodeHeight < 0) return false;
    if (config.nodeWidth < 0) return false;
    if (!config.attractiveForce) return false;
    if (!config.repulsiveForce) return false;
    return true;
}
