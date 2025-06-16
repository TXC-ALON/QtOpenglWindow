
#include "LoadModel_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include <QVBoxLayout>

#include "LoadModel1.h"
// #include "LoadModel2.h"

LoadModel_DockWidget::LoadModel_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

LoadModel_DockWidget::~LoadModel_DockWidget()
{
}
void LoadModel_DockWidget::initDockWidget()
{
    DockWidgetName = "LoadModel_DockWidget";
    button_list = {"pb_LoadModel1"};
    button_name_list = {"LoadModel1"};
    // Initialize your dock widget here
}
void LoadModel_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void LoadModel_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
    // Set up your widget layout and style here
}
void LoadModel_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &LoadModel_DockWidget::pb_LoadModel1_clicked);
    // connect(example_buttoms[1], &QPushButton::clicked, this, &LoadModel_DockWidget::pb_LoadModel2_clicked);
}

void LoadModel_DockWidget::pb_LoadModel1_clicked()
{
    std::cout << "pb_LoadModel1_clicked" << std::endl;
    pLoadModel1 = new LoadModel1(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLoadModel1);
    refresh_tool_control(pLoadModel1);
}

// void LoadModel_DockWidget::pb_LoadModel2_clicked()
// {
//     std::cout << "pb_LoadModel2_clicked" << std::endl;
//     pLoadModel2 = new LoadModel2(Manager::get_instance()->mainWindow);
//     Manager::get_instance()->setGLwidget(pLoadModel2);
//     // refresh_tool_control(pLoadModel2);
// }
