#pragma once
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
// 这里是变换加入的地方

class Colors;
class BasicLightingDiffuse;
class BasicLightingSpecular;
class BasicLightingExercise1;
class BasicLightingExercise3;

class BasicLighting_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    BasicLighting_DockWidget(QWidget *parent = nullptr);
    ~BasicLighting_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Colors *pColors = nullptr;
    BasicLightingDiffuse *pBasicLightingDiffuse = nullptr;
    BasicLightingSpecular *pBasicLightingSpecular = nullptr;
    BasicLightingExercise1 *pBasicLightingExercise1 = nullptr;
    BasicLightingExercise3 *pBasicLightingExercise3 = nullptr;
private slots:
    void pb_Colors_clicked();
    void pb_BasicLightingDiffuse_clicked();
    void pb_BasicLightingSpecular_clicked();
    void pb_BasicLightingExercise1_clicked();
    void pb_BasicLightingExercise3_clicked();
};
