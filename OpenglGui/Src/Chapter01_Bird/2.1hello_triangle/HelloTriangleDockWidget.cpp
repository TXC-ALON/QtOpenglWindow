#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include "HelloTriangleDockWidget.h"
#include "HelloTriangle.h"
#include "HelloTriangleIndexed.h"
#include "HelloTriangleExercise1.h"
#include "HelloTriangleExercise2.h"
#include "HelloTriangleExercise3.h"
HelloTriangleDockWidget::HelloTriangleDockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

HelloTriangleDockWidget::~HelloTriangleDockWidget()
{
}
void HelloTriangleDockWidget::initDockWidget()
{
    DockWidgetName = "HelloTriangleDockWidget";
    button_list = {"pb_HelloTriangle", "pb_HelloTriangleIndexed", "pb_HelloTriangleExercise1", "pb_HelloTriangleExercise2", "pb_HelloTriangleExercise3"};
    button_name_list = {"HelloTriangle", "HelloTriangleIndexed", "HelloTriangleExercise1", "HelloTriangleExercise2", "HelloTriangleExercise3"};
}
void HelloTriangleDockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void HelloTriangleDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void HelloTriangleDockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &HelloTriangleDockWidget::pb_HelloTriangle_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &HelloTriangleDockWidget::pb_HelloTriangleIndexed_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &HelloTriangleDockWidget::pb_HelloTriangleExercise1_clicked);
    connect(example_buttoms[3], &QPushButton::clicked, this, &HelloTriangleDockWidget::pb_HelloTriangleExercise2_clicked);
    connect(example_buttoms[4], &QPushButton::clicked, this, &HelloTriangleDockWidget::pb_HelloTriangleExercise3_clicked);
}

void HelloTriangleDockWidget::pb_HelloTriangle_clicked()
{
    std::cout << "pb_HelloTriangle_clicked" << std::endl;
    pHelloTriangle = new HelloTriangle(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pHelloTriangle);
}

void HelloTriangleDockWidget::pb_HelloTriangleIndexed_clicked()
{
    std::cout << "pb_HelloTriangleIndexed_clicked" << std::endl;
    pHelloTriangleIndexed = new HelloTriangleIndexed(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pHelloTriangleIndexed);
}
void HelloTriangleDockWidget::pb_HelloTriangleExercise1_clicked()
{
    std::cout << "pb_HelloTriangleExercise1_clicked" << std::endl;
    pHelloTriangleExercise1 = new HelloTriangleExercise1(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pHelloTriangleExercise1);
}
void HelloTriangleDockWidget::pb_HelloTriangleExercise2_clicked()
{
    std::cout << "pb_HelloTriangleExercise2_clicked" << std::endl;
    pHelloTriangleExercise2 = new HelloTriangleExercise2(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pHelloTriangleExercise2);
}
void HelloTriangleDockWidget::pb_HelloTriangleExercise3_clicked()
{
    std::cout << "pb_HelloTriangleExercise3_clicked" << std::endl;
    pHelloTriangleExercise3 = new HelloTriangleExercise3(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pHelloTriangleExercise3);
}
