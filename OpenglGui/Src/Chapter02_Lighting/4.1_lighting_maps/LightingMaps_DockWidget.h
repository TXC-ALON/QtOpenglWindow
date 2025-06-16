#pragma once
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
// 这里是变换加入的地方

class LightingMapsDiffuseMap;
class LightingMapsSpecularMap;
class LightingMapsExercise2;
class LightingMapsExercise4;

class LightingMaps_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    LightingMaps_DockWidget(QWidget *parent = nullptr);
    ~LightingMaps_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    LightingMapsDiffuseMap *pLightingMapsDiffuseMap = nullptr;
    LightingMapsSpecularMap *pLightingMapsSpecularMap = nullptr;
    LightingMapsExercise2 *pLightingMapsExercise2 = nullptr;
    LightingMapsExercise4 *pLightingMapsExercise4 = nullptr;

    void pb_LightingMapsDiffuseMap_clicked();
    void pb_LightingMapsSpecularMap_clicked();
    void pb_LightingMapsExercise2_clicked();
    void pb_LightingMapsExercise4_clicked();
};
