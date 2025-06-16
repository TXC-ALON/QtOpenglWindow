#pragma once
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
// 这里是变换加入的地方
// #include "Transformations.h"
class Materials;
class Materials_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    Materials_DockWidget(QWidget *parent = nullptr);
    ~Materials_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Materials *pMaterials = nullptr;
    void pb_Materials_clicked();
};
