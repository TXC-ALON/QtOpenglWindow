#pragma once

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QToolButton>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <iostream>

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QSurfaceFormat>
#include "Chapter_headers.h"
class MainWindow;
class Manager : public QObject
{
    Q_OBJECT

private:
    // 私有构造函数
    Manager(QObject *parent = nullptr)
        : QObject(parent)
    {
        // 初始化代码
    }

    // 删除拷贝构造函数和赋值操作符
    Manager(const Manager &) = delete;
    Manager &operator=(const Manager &) = delete;
    // 存储QMainWindow的指针

public:
    MainWindow *mainWindow;
    // 静态方法来获取单例实例
    static Manager *get_instance()
    {
        static QMutex mutex;                  // 用于线程安全
        static Manager *m_instance = nullptr; // 单例实例

        QMutexLocker locker(&mutex); // 自动锁定和解锁
        if (m_instance == nullptr)
        {
            m_instance = new Manager();
        }
        return m_instance;
    }
    void setMainWindow(MainWindow *window)
    {
        mainWindow = window;
    }
    void ClearGL();
    void UpdateGL();
    void ClearDockWidget();
    void UpdateDockWidget();
    void onInitialized();
    void setGLwidget(GLWidget *widget);
    void setupSurfaceFormat(int samples);
    void UpdateCameraDockWidget(QWidget *camera_widget);
    template <typename T>
    void addOpenGLWidgetToLayout(T *&widget)
    {
        ClearGL();
        widget = new T(mainWindow);
        widget->setMinimumSize(QSize(800, 600));
        mainWindow->ui->openGLWidget = widget;
        mainWindow->ui->gridLayout_main->addWidget(mainWindow->ui->openGLWidget, 1, 1, 1, 1);
        UpdateGL();
    }

    template <typename T>
    void addDockWidgetToLayout(T *&widget)
    {
        // 创建一个新的GLWidget，这里假设addOpenGLWidgetToLayout是一个已经定义好的函数
        GLWidget *temp_gl = new GLWidget(mainWindow);
        addOpenGLWidgetToLayout(temp_gl);

        // 清除之前的DockWidget，这里假设ClearDockWidget是一个已经定义好的函数
        ClearDockWidget();

        // 创建一个新的T类型的widget
        widget = new T(mainWindow);

        // 设置新widget的最小尺寸
        widget->setMinimumSize(QSize(200, 170));

        // 将新widget的dw_example与mainWindow中的dw_example关联
        mainWindow->ui->dw_example = widget->ui->dw_example;

        // 将新widget添加到QMainWindow的右侧
        mainWindow->addDockWidget(Qt::RightDockWidgetArea, mainWindow->ui->dw_example);

        // 确保dw_example是当前显示的标签
        mainWindow->ui->dw_example->setVisible(true);
        // 合并dw_example和dw_menu为一个标签页
        mainWindow->tabifyDockWidget(mainWindow->ui->dw_example, mainWindow->ui->dw_model);
        // tabifyDockWidget(ui->dw_example, ui->dw_camera);
        mainWindow->ui->dw_example->raise();
        mainWindow->addDockWidget(Qt::RightDockWidgetArea, mainWindow->ui->dw_camera);
        mainWindow->adjustSize();
    }

public:
    HelloTriangleDockWidget *pHelloTriangleDockWidget;
    ShadersDockWidget *pShadersDockWidget;
    TextureDockWidget *pTextureDockWidget;
    TransformationsDockWidget *pTransformationsDockWidget;
    CoordinateSystemsDockWidget *pCoordinateSystemsDockWidget;
    CameraDockWidget *pCameraDockWidget;
    ReviewDockWidget *pReviewDockWidget;
    BasicLighting_DockWidget *pBasicLighting_DockWidget;
    Materials_DockWidget *pMaterials_DockWidget;
    LightingMaps_DockWidget *pLightingMaps_DockWidget;
    LightCasters_DockWidget *pLightCasters_DockWidget;
    MultiLights_DockWidget *pMultiLights_DockWidget;
    ReviewLighting_DockWidget *pReviewLighting_DockWidget;
    LoadModel_DockWidget *pLoadModel_DockWidget;
    AdvancedOpenGL_DockWidget *pAdvancedOpenGL_DockWidget;
    AdvancedLighting_DockWidget *pAdvancedLighting_DockWidget;

public slots:
    // 定义槽函数来处理接收到的信号
    // dockwidget
    // chapter1 Bird
    void hello_triangle_dockwidget();
    void hello_shader_dockwidget();
    void texture_dockwidget();
    void transform_dockwidget();
    void corrdinate_dockwidget();
    void camera_dockwidget();
    void review_dockwidget();

    // chapter2 Lighting
    void basic_lighting_dockwidget();
    void materials_dockwidget();
    void lighting_maps_dockwidget();
    void light_casters_dockwidget();
    void multiple_lights_dockwidget();
    void review_lights_dockwidget();

    // chapter3 Model
    void load_model_dockwidget();

    // chapter4 Advanced
    void load_advancedOpenGL_dockwidget();

    // chapter5 Advanced Lighting
    void load_advancedLighting_dockwidget();

public slots:
    // opengl
public:
signals:
    // 如果需要，可以定义自己的信号
    void dataProcessed(const QString &processedData);
};
