#include "TransformationsDockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include "Transformations.h"
TransformationsDockWidget::TransformationsDockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

TransformationsDockWidget::~TransformationsDockWidget()
{
}
void TransformationsDockWidget::initDockWidget()
{
    DockWidgetName = "TransformationsDockWidget";
    button_list = {"pb_Transformations"};
    button_name_list = {"Transformations"};
}
void TransformationsDockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void TransformationsDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void TransformationsDockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &TransformationsDockWidget::pb_Transformations_clicked);
}
void TransformationsDockWidget::pb_Transformations_clicked()
{
    std::cout << "pb_Transformations_clicked" << std::endl;
    pTransformations = new Transformations(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pTransformations);
}
