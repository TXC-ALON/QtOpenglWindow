#include "BasicLighting_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>

#include "Colors.h"
#include "BasicLightingDiffuse.h"
#include "BasicLightingSpecular.h"
#include "BasicLightingExercise1.h"
#include "BasicLightingExercise3.h"

BasicLighting_DockWidget::BasicLighting_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

BasicLighting_DockWidget::~BasicLighting_DockWidget()
{
}
void BasicLighting_DockWidget::initDockWidget()
{
    DockWidgetName = "BasicLighting_DockWidget";
    button_list = {"pb_colors", "pb_basic_lighting_diffuse", "pb_basic_lighting_specular", "pb_basic_lighting_exercise1", "pb_basic_lighting_exercise3"};
    button_name_list = {"colors", "basic_lighting_diffuse", "basic_lighting_specular", "basic_lighting_exercise1", "basic_lighting_exercise3"};
}
void BasicLighting_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void BasicLighting_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void BasicLighting_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &BasicLighting_DockWidget::pb_Colors_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &BasicLighting_DockWidget::pb_BasicLightingDiffuse_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &BasicLighting_DockWidget::pb_BasicLightingSpecular_clicked);
    connect(example_buttoms[3], &QPushButton::clicked, this, &BasicLighting_DockWidget::pb_BasicLightingExercise1_clicked);
    connect(example_buttoms[4], &QPushButton::clicked, this, &BasicLighting_DockWidget::pb_BasicLightingExercise3_clicked);
}
void BasicLighting_DockWidget::pb_Colors_clicked()
{
    std::cout << "pb_Colors_clicked" << std::endl;
    pColors = new Colors(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pColors);
    refresh_tool_control(pColors); // 如果有控件,刷新控件
}

void BasicLighting_DockWidget::pb_BasicLightingDiffuse_clicked()
{
    std::cout << "pb_Colors_clicked" << std::endl;
    pBasicLightingDiffuse = new BasicLightingDiffuse(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBasicLightingDiffuse);
    // refresh_tool_control(pBasicLightingDiffuse); // 如果有控件,刷新控件
}

void BasicLighting_DockWidget::pb_BasicLightingSpecular_clicked()
{
    std::cout << "pb_Colors_clicked" << std::endl;
    pBasicLightingSpecular = new BasicLightingSpecular(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBasicLightingSpecular);
    // refresh_tool_control(pColors); // 如果有控件,刷新控件
}

void BasicLighting_DockWidget::pb_BasicLightingExercise1_clicked()
{
    std::cout << "pb_Colors_clicked" << std::endl;
    pBasicLightingExercise1 = new BasicLightingExercise1(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBasicLightingExercise1);
    // refresh_tool_control(pBasicLightingExercise1); // 如果有控件,刷新控件
}

void BasicLighting_DockWidget::pb_BasicLightingExercise3_clicked()
{
    std::cout << "pb_Colors_clicked" << std::endl;
    pBasicLightingExercise3 = new BasicLightingExercise3(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBasicLightingExercise3);
    // refresh_tool_control(pBasicLightingExercise3); // 如果有控件,刷新控件
}
