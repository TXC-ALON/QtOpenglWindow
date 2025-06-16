#include "CameraDockWidget.h"
#include <QToolBox>
#include <iostream>
#include <QLabel>
#include <Manager.h>

#include "CameraCircle.h"
#include "CameraInteract.h"
#include "CameraClass.h"

CameraDockWidget::CameraDockWidget(QWidget *parent)
    : glDockWidget(parent)
{

    setupWidget(this);
}

CameraDockWidget::~CameraDockWidget()
{
}
void CameraDockWidget::initDockWidget()
{
    DockWidgetName = "CameraDockWidget";
    button_list = {"pb_CameraCircle", "pb_CameraInteract", "pb_CameraClass"};
    button_name_list = {"CameraCircle", "CameraInteract", "CameraClass"};
}
void CameraDockWidget::setupWidget(QWidget *widget)
{
    initDockWidget();
    create_tool_example(widget);
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
}
void CameraDockWidget::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void CameraDockWidget::connectSignals(QWidget *widget)
{
    connect(example_buttoms[0], &QPushButton::clicked, this, &CameraDockWidget::pb_CameraCircle_clicked);
    connect(example_buttoms[1], &QPushButton::clicked, this, &CameraDockWidget::pb_CameraInteract_clicked);
    connect(example_buttoms[2], &QPushButton::clicked, this, &CameraDockWidget::pb_CameraClass_clicked);
}
void CameraDockWidget::pb_CameraCircle_clicked()
{
    std::cout << "pb_CameraCircle_clicked" << std::endl;
    pCameraCircle = new CameraCircle(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCameraCircle);
}
void CameraDockWidget::pb_CameraInteract_clicked()
{
    std::cout << "pb_CameraInteract_clicked" << std::endl;
    pCameraInteract = new CameraInteract(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCameraInteract);
}
void CameraDockWidget::pb_CameraClass_clicked()
{
    std::cout << "pb_CameraClass_clicked" << std::endl;
    pCameraClass = new CameraClass(Manager::get_instance()->mainWindow);
    Manager::get_instance()->setGLwidget(pCameraClass);
}