#include "LightCasters_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>

#include "LightCastersDirectional.h"
#include "LightCastersPoint.h"
#include "LightCastersSpot.h"
LightCasters_DockWidget::LightCasters_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

LightCasters_DockWidget::~LightCasters_DockWidget()
{
}
void LightCasters_DockWidget::initDockWidget()
{
    DockWidgetName = "LightCasters_DockWidget";
    button_list = {"pb_light_casters_directional", "pb_light_casters_point", "pb_light_casters_spot"};
    button_name_list = {"light_casters_directional", "light_casters_point", "light_casters_spot"};
}
void LightCasters_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void LightCasters_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void LightCasters_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &LightCasters_DockWidget::pb_LightCastersDirectional_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &LightCasters_DockWidget::pb_LightCastersPoint_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &LightCasters_DockWidget::pb_LightCastersSpot_clicked);
}
void LightCasters_DockWidget::pb_LightCastersDirectional_clicked()
{
    std::cout << "pb_LightCastersDirectional_clicked" << std::endl;
    pLightCastersDirectional = new LightCastersDirectional(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightCastersDirectional);
    refresh_tool_control(pLightCastersDirectional); // 如果有控件,刷新控件
}
void LightCasters_DockWidget::pb_LightCastersPoint_clicked()
{

    std::cout << "pb_LightCastersPoint_clicked" << std::endl;
    pLightCastersPoint = new LightCastersPoint(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightCastersPoint);
    refresh_tool_control(pLightCastersPoint); // 如果有控件,刷新控件
}
void LightCasters_DockWidget::pb_LightCastersSpot_clicked()
{
    std::cout << "pb_LightCastersSpot_clicked" << std::endl;
    pLightCastersSpot = new LightCastersSpot(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightCastersSpot);
    refresh_tool_control(pLightCastersSpot); // 如果有控件,刷新控件
}
