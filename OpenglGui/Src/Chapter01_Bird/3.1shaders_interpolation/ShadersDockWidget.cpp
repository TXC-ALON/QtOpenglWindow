
#include <iostream>
#include <Manager.h>
#include "ShadersDockWidget.h"
#include "ShadersInterpolation.h"

ShadersDockWidget::ShadersDockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

ShadersDockWidget::~ShadersDockWidget()
{
}
void ShadersDockWidget::initDockWidget()
{
    DockWidgetName = "ShadersDockWidget";
    button_list = {"pb_shaders_interpolation"};
    button_name_list = {"shaders_interpolation"};
}
void ShadersDockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void ShadersDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void ShadersDockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &ShadersDockWidget::pb_shaders_interpolation_clicked);
}
void ShadersDockWidget::pb_shaders_interpolation_clicked()
{
    std::cout << "pb_shaders_interpolation_clicked" << std::endl;
    pShadersInterpolation = new ShadersInterpolation(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pShadersInterpolation);
}
