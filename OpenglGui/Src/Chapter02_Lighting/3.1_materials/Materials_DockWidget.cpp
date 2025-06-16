#include "Materials_DockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include "Materials.h"
Materials_DockWidget::Materials_DockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

Materials_DockWidget::~Materials_DockWidget()
{
}
void Materials_DockWidget::initDockWidget()
{
    DockWidgetName = "Materials_DockWidget";
    button_list = {"pb_Materials"};
    button_name_list = {"Materials"};
}
void Materials_DockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void Materials_DockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void Materials_DockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &Materials_DockWidget::pb_Materials_clicked);
}
void Materials_DockWidget::pb_Materials_clicked()
{
    std::cout << "pb_Materials_clicked" << std::endl;
    pMaterials = new Materials(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pMaterials);
    // refresh_tool_control(pMaterials);//如果有控件,刷新控件
}
