// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QToolButton>
#include <QMessageBox>

#include <QDir>
#include <QSettings>
#include <iostream>
#include "Chapter_headers.h"
#include <Manager.h>
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    std::cout << "start MainWindow " << std::endl;
    ui->setupUi(this);
    tabifyDockWidget(ui->dw_example, ui->dw_model);
    addDockWidget(Qt::RightDockWidgetArea, ui->dw_camera);
    // tabifyDockWidget(ui->dw_example, ui->dw_camera);

    setupWidget(this);
}

MainWindow::~MainWindow()
{
}
void MainWindow::setupWidget(QWidget *widget)
{
    setupWidgetLayoutAndStyle(widget);
    connectSignals(widget);
    Manager::get_instance()->setMainWindow(this);
}
void MainWindow::setupWidgetLayoutAndStyle(QWidget *widget)
{
}
void MainWindow::connectSignals(QWidget *widget)
{
    connectSignals_Chapter1(widget);
    connectSignals_Chapter2(widget);
    connectSignals_Chapter3(widget);
    connectSignals_Chapter4(widget);
    connectSignals_Chapter5(widget);
}

void MainWindow::connectSignals_Chapter1(QWidget *widget)
{
    connect(ui->pb_hellotriangle, &QPushButton::clicked, Manager::get_instance(), &Manager::hello_triangle_dockwidget);
    connect(ui->pb_glsl, &QPushButton::clicked, Manager::get_instance(), &Manager::hello_shader_dockwidget);
    connect(ui->pb_texture, &QPushButton::clicked, Manager::get_instance(), &Manager::texture_dockwidget);
    connect(ui->pb_transform, &QPushButton::clicked, Manager::get_instance(), &Manager::transform_dockwidget);
    connect(ui->pb_coord, &QPushButton::clicked, Manager::get_instance(), &Manager::corrdinate_dockwidget);
    connect(ui->pb_camera, &QPushButton::clicked, Manager::get_instance(), &Manager::camera_dockwidget);
    connect(ui->pb_review, &QPushButton::clicked, Manager::get_instance(), &Manager::review_dockwidget);
}

void MainWindow::connectSignals_Chapter2(QWidget *widget)
{
    connect(ui->pb_LightingBasic, &QPushButton::clicked, Manager::get_instance(), &Manager::basic_lighting_dockwidget);
    connect(ui->pb_LightMaterials, &QPushButton::clicked, Manager::get_instance(), &Manager::materials_dockwidget);
    connect(ui->pb_LightMaps, &QPushButton::clicked, Manager::get_instance(), &Manager::lighting_maps_dockwidget);
    connect(ui->pb_LightCasters, &QPushButton::clicked, Manager::get_instance(), &Manager::light_casters_dockwidget);
    connect(ui->pb_LightMulti, &QPushButton::clicked, Manager::get_instance(), &Manager::multiple_lights_dockwidget);
    connect(ui->pb_LightReview, &QPushButton::clicked, Manager::get_instance(), &Manager::review_lights_dockwidget);
}
void MainWindow::connectSignals_Chapter3(QWidget *widget)
{
    connect(ui->pb_ModelLoad1, &QPushButton::clicked, Manager::get_instance(), &Manager::load_model_dockwidget);
}
void MainWindow::connectSignals_Chapter4(QWidget *widget)
{
    connect(ui->pb_AdvancedOpenGL, &QPushButton::clicked, Manager::get_instance(), &Manager::load_advancedOpenGL_dockwidget);
}
void MainWindow::connectSignals_Chapter5(QWidget *widget)
{
    connect(ui->pb_AdvanecdLighting, &QPushButton::clicked, Manager::get_instance(), &Manager::load_advancedLighting_dockwidget);
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close(); // 关闭窗口
    }
    else
    {
        QMainWindow::keyPressEvent(event); // 调用基类的事件处理函数
    }
}
