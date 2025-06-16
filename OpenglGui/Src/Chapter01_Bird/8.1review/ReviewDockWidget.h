#pragma once
#include <QDockWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QObject>
#include "glDockWidget.h"
// 这里是变换加入的地方

class Review_GLSL;
class Review_Camera;

class ReviewDockWidget : public glDockWidget
{
    Q_OBJECT
public:
    ReviewDockWidget(QWidget *parent = nullptr);
    ~ReviewDockWidget();

private:
    void initDockWidget();
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);

public:
    Review_GLSL *pReview_GLSL = nullptr;
    Review_Camera *pReview_Camera = nullptr;

    void pb_Review_GLSL_clicked();
    void pb_Review_Camera_clicked();
};
