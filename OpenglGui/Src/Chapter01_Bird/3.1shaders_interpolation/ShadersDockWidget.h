#pragma once
#include "glDockWidget.h"
class ShadersInterpolation;
namespace Ui
{
    class helloshaders;
}
class ShadersDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    ShadersDockWidget(QWidget *parent = nullptr);
    ~ShadersDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    ShadersInterpolation *pShadersInterpolation = nullptr;
public slots:
    void pb_shaders_interpolation_clicked();
};
