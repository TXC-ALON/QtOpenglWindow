#pragma once
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
// 这里是变换加入的地方

class MultipleLights;
class MultiLights_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    MultiLights_DockWidget(QWidget *parent = nullptr);
    ~MultiLights_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    MultipleLights *pMultipleLights = nullptr;
    void pb_MultipleLights_clicked();
};
