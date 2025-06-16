#include "LightingMaps_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>

#include "LightingMapsDiffuseMap.h"
#include "LightingMapsSpecularMap.h"
#include "LightingMapsExercise2.h"
#include "LightingMapsExercise4.h"

LightingMaps_DockWidget::LightingMaps_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

LightingMaps_DockWidget::~LightingMaps_DockWidget()
{
}
void LightingMaps_DockWidget::initDockWidget()
{
    DockWidgetName = "LightingMaps_DockWidget";
    button_list = {"pb_lighting_maps_diffuse_map", "pb_lighting_maps_specular_map", "pb_lighting_maps_exercise2", "pb_lighting_maps_exercise4"};
    button_name_list = {"lighting_maps_diffuse_map", "lighting_maps_specular_map", "lighting_maps_exercise2", "lighting_maps_exercise4"};
}
void LightingMaps_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void LightingMaps_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void LightingMaps_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &LightingMaps_DockWidget::pb_LightingMapsDiffuseMap_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &LightingMaps_DockWidget::pb_LightingMapsSpecularMap_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &LightingMaps_DockWidget::pb_LightingMapsExercise2_clicked);
    connect(example_buttoms[3], &QPushButton::clicked, this, &LightingMaps_DockWidget::pb_LightingMapsExercise4_clicked);
}

void LightingMaps_DockWidget::pb_LightingMapsDiffuseMap_clicked()
{
    std::cout << "pb_LightingMapsDiffuseMap_clicked" << std::endl;
    pLightingMapsDiffuseMap = new LightingMapsDiffuseMap(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightingMapsDiffuseMap);
    // refresh_tool_control(pLightingMapsDiffuseMap);//如果有控件,刷新控件
}
void LightingMaps_DockWidget::pb_LightingMapsSpecularMap_clicked()
{
    std::cout << "pb_LightingMapsSpecularMap_clicked" << std::endl;
    pLightingMapsSpecularMap = new LightingMapsSpecularMap(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightingMapsSpecularMap);
    // refresh_tool_control(pLightingMapsSpecularMap);//如果有控件,刷新控件
}
void LightingMaps_DockWidget::pb_LightingMapsExercise2_clicked()
{
    std::cout << "pb_LightingMapsExercise2_clicked" << std::endl;
    pLightingMapsExercise2 = new LightingMapsExercise2(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightingMapsExercise2);
    // refresh_tool_control(pLightingMapsExercise2);//如果有控件,刷新控件
}
void LightingMaps_DockWidget::pb_LightingMapsExercise4_clicked()
{
    std::cout << "pb_LightingMapsExercise4_clicked" << std::endl;
    pLightingMapsExercise4 = new LightingMapsExercise4(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pLightingMapsExercise4);
    // refresh_tool_control(pLightingMapsExercise4);//如果有控件,刷新控件
}