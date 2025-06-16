#include "MultiLights_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>

#include "MultipleLights.h"

MultiLights_DockWidget::MultiLights_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

MultiLights_DockWidget::~MultiLights_DockWidget()
{
}
void MultiLights_DockWidget::initDockWidget()
{
    DockWidgetName = "MultiLights_DockWidget";
    button_list = {"pb_multiple_lights"};
    button_name_list = {"multiple_lights"};
}
void MultiLights_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void MultiLights_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void MultiLights_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &MultiLights_DockWidget::pb_MultipleLights_clicked);
}
void MultiLights_DockWidget::pb_MultipleLights_clicked()
{
    std::cout << "pb_MultipleLights_clicked" << std::endl;
    pMultipleLights = new MultipleLights(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pMultipleLights);
    // refresh_tool_control(pMultipleLights);//如果有控件,刷新控件
}
