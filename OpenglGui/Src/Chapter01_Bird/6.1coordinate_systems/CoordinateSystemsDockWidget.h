#pragma once

#include "glDockWidget.h"
class CoordinateSystems;
class CoordinateSystemsDepth;
class CoordinateSystemsExercise;

class CoordinateSystemsDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    CoordinateSystemsDockWidget(QWidget *parent = nullptr);
    ~CoordinateSystemsDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    void create_tool_example(QWidget *widget);
    CoordinateSystems *pCoordinateSystems = nullptr;
    CoordinateSystemsDepth *pCoordinateSystemsDepth = nullptr;
    CoordinateSystemsExercise *pCoordinateSystemsExercise = nullptr;

private slots:
    void pb_CoordinateSystems_clicked();
    void pb_CoordinateSystemsDepth_clicked();
    void pb_CoordinateSystemsExercise_clicked();

public:
};
