#include "ReviewDockWidget.h"

#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>
#include "Review_glsl.h"
#include "Review_camera.h"

ReviewDockWidget::ReviewDockWidget(QWidget *parent)
    : glDockWidget(parent)
{
    setupWidget(this);
}

ReviewDockWidget::~ReviewDockWidget()
{
}
void ReviewDockWidget::initDockWidget()
{
    DockWidgetName = "ReviewDockWidget";
    button_list = {"pb_Review_GLSL", "pb_Review_Camera"};
    button_name_list = {"Review_GLSL", "Review_Camera"};
}
void ReviewDockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void ReviewDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void ReviewDockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &ReviewDockWidget::pb_Review_GLSL_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &ReviewDockWidget::pb_Review_Camera_clicked);
}
void ReviewDockWidget::pb_Review_GLSL_clicked()
{
    std::cout << "pb_Review_GLSL_clicked" << std::endl;
    pReview_GLSL = new Review_GLSL(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pReview_GLSL);
    // refresh_tool_control(pReview_GLSL);//如果有控件,刷新控件
}
void ReviewDockWidget::pb_Review_Camera_clicked()
{
    std::cout << "pb_Review_Camera_clicked" << std::endl;
    pReview_Camera = new Review_Camera(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pReview_Camera);
    // refresh_tool_control(pReview_GLSL);//如果有控件,刷新控件
}
