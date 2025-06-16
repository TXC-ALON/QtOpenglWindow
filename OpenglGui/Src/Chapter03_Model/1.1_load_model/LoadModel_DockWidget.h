#pragma once

#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"

class LoadModel1;
// class LoadModel2;

class LoadModel_DockWidget : public glDockWidget
{
    Q_OBJECT
public:
    LoadModel_DockWidget(QWidget *parent = nullptr);
    ~LoadModel_DockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    LoadModel1 *pLoadModel1 = nullptr;
    // LoadModel2 *pLoadModel2 = nullptr;
private slots:
    void pb_LoadModel1_clicked();
    // void pb_LoadModel2_clicked();
};
