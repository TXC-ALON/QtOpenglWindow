#pragma once
#include "glDockWidget.h"

class Transformations;
class TransformationsDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    TransformationsDockWidget(QWidget *parent = nullptr);
    ~TransformationsDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Transformations *pTransformations = nullptr;
    void pb_Transformations_clicked();
};
