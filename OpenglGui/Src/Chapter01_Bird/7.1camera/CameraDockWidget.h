#pragma once

#include "glDockWidget.h"
class CameraCircle;
class CameraInteract;
class CameraClass;

class CameraDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    CameraDockWidget(QWidget *parent = nullptr);
    ~CameraDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    CameraCircle *pCameraCircle = nullptr;
    CameraInteract *pCameraInteract = nullptr;
    CameraClass *pCameraClass = nullptr;
public slots:
    void pb_CameraCircle_clicked();
    void pb_CameraInteract_clicked();
    void pb_CameraClass_clicked();
};
