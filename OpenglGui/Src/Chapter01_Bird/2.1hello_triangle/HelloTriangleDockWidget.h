#pragma once
#include "glDockWidget.h"

class HelloTriangle;
class HelloTriangleIndexed;
class HelloTriangleExercise1;
class HelloTriangleExercise2;
class HelloTriangleExercise3;

class HelloTriangleDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    HelloTriangleDockWidget(QWidget *parent = nullptr);
    ~HelloTriangleDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

private:
public:
    // void create_tool_example(QWidget *widget);
    void refresh_tool_control(GLWidget *widget) {};
    HelloTriangle *pHelloTriangle = nullptr;
    HelloTriangleIndexed *pHelloTriangleIndexed = nullptr;
    HelloTriangleExercise1 *pHelloTriangleExercise1 = nullptr;
    HelloTriangleExercise2 *pHelloTriangleExercise2 = nullptr;
    HelloTriangleExercise3 *pHelloTriangleExercise3 = nullptr;

public slots:
    void pb_HelloTriangle_clicked();
    void pb_HelloTriangleIndexed_clicked();
    void pb_HelloTriangleExercise1_clicked();
    void pb_HelloTriangleExercise2_clicked();
    void pb_HelloTriangleExercise3_clicked();
};
