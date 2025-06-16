#pragma once
#include <QObject>
#include <QWidget>
#include <QToolBox>
#include "glWidget.h"
#include "ui_glDockWidget.h"
#include <iostream>
namespace Ui
{
    class glDockWidget;
}
class glDockWidget : public QWidget
{
    Q_OBJECT
public:
    glDockWidget(QWidget *parent = nullptr);
    virtual ~glDockWidget();

public:
    virtual void create_tool_example(QWidget *widget);
    virtual void refresh_tool_control(GLWidget *widget);

private:
    virtual void initDockWidget() = 0;
    virtual void setupWidget(QWidget *widget) = 0;
    virtual void setupWidgetLayoutAndStyle(QWidget *widget) = 0;
    virtual void connectSignals(QWidget *widget) = 0;
public slots:
public:
    Ui::glDockWidget *ui;
    QString DockWidgetName = "-Default-";
    std::vector<QPushButton *> example_buttoms;
    QStringList button_list;
    QStringList button_name_list;
};