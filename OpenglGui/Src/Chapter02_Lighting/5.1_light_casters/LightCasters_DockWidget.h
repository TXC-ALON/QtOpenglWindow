#pragma once
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
// 这里是变换加入的地方

class LightCastersDirectional;
class LightCastersPoint;
class LightCastersSpot;

class LightCasters_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    LightCasters_DockWidget(QWidget *parent = nullptr);
    ~LightCasters_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    LightCastersDirectional *pLightCastersDirectional = nullptr;
    LightCastersPoint *pLightCastersPoint = nullptr;
    LightCastersSpot *pLightCastersSpot = nullptr;

    void pb_LightCastersDirectional_clicked();
    void pb_LightCastersPoint_clicked();
    void pb_LightCastersSpot_clicked();
};
