#ifndef GRAPHCALCULATOR_H
#define GRAPHCALCULATOR_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QVector>
#include "2d/point.h"
#include "igraph.h"
#include "kfunction.h"

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
};


class GraphCalculator : public QObject, public QRunnable
{
    Q_OBJECT
public:
    GraphCalculator(IGraph *graph,
                    QVector<GraphGeometry::D2::Point> &positions,
                    QMutex &lock,
                    GraphCalculatorConfig config);

    Q_INVOKABLE
    void run() override;

signals:
    void updated();
    void finished();

private:
    static const double kMinimalTemperature;
    static const double kTemperatureDecreasingFactor;
    static const int kDelayUs;
    static const int kMaxDegrees;
    static const int kRightAngleDeg;

    IGraph *graph;
    QMutex &mutex;
    QVector<GraphGeometry::D2::Point> &positions;
    GraphCalculatorConfig config;
};

#endif // GRAPHCALCULATOR_H
