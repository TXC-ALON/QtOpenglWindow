// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE
class Manager;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::unique_ptr<Ui::MainWindow> ui;

private:
    void keyPressEvent(QKeyEvent *event);
    void setupWidget(QWidget *widget);
    void setupWidgetLayoutAndStyle(QWidget *widget);
    void connectSignals(QWidget *widget);
    void connectSignals_Chapter1(QWidget *widget);
    void connectSignals_Chapter2(QWidget *widget);
    void connectSignals_Chapter3(QWidget *widget);
    void connectSignals_Chapter4(QWidget *widget);
    void connectSignals_Chapter5(QWidget *widget);

private slots:
};

#endif // MAINWINDOW_H
