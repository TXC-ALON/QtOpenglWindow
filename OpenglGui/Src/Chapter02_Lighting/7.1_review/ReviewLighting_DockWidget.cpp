
#include "ReviewLighting_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include <QVBoxLayout>

#include "Basic_Colors.h"
#include "Light_Materials.h"
#include "Light_Multiple.h"

ReviewLighting_DockWidget::ReviewLighting_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

ReviewLighting_DockWidget::~ReviewLighting_DockWidget()
{
}
void ReviewLighting_DockWidget::initDockWidget()
{
    DockWidgetName = "ReviewLighting_DockWidget";
    button_list = {"pb_Basic_Colors", "pb_Light_Materials", "pb_Light_Multiple"};
    button_name_list = {"Basic_Colors", "Light_Materials", "Light_Multiple"};
    // Initialize your dock widget here
}
void ReviewLighting_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void ReviewLighting_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
    // Set up your widget layout and style here
}
void ReviewLighting_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &ReviewLighting_DockWidget::pb_Basic_Colors_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &ReviewLighting_DockWidget::pb_Light_Materials_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &ReviewLighting_DockWidget::pb_Light_Multiple_clicked);
}

void ReviewLighting_DockWidget::pb_Basic_Colors_clicked()
{
    std::cout << "pb_Basic_Colors_clicked" << std::endl;
    pBasic_Colors = new Basic_Colors(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pBasic_Colors);
    refresh_tool_control(pBasic_Colors);
}
void ReviewLighting_DockWidget::pb_Light_Materials_clicked()
{
    std::cout << "pb_Light_Materials_clicked" << std::endl;
    pLight_Materials = new Light_Materials(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLight_Materials);
    refresh_tool_control(pLight_Materials);
}
void ReviewLighting_DockWidget::pb_Light_Multiple_clicked()
{
    std::cout << "pb_Light_Multiple_clicked" << std::endl;
    pLight_Multiple = new Light_Multiple(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLight_Multiple);
    refresh_tool_control(pLight_Multiple);
}
