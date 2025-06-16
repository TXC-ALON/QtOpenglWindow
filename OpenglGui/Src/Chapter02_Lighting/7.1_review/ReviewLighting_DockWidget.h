
#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
class Basic_Colors;
class Light_Materials;
class Light_Multiple;
class ReviewLighting_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    ReviewLighting_DockWidget(QWidget *parent = nullptr);
    ~ReviewLighting_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Basic_Colors *pBasic_Colors = nullptr;
    Light_Materials *pLight_Materials = nullptr;
    Light_Multiple *pLight_Multiple = nullptr;
private slots:
    void pb_Basic_Colors_clicked();
    void pb_Light_Materials_clicked();
    void pb_Light_Multiple_clicked();
};
