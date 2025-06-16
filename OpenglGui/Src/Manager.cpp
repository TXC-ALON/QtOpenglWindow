#include "Manager.h"
#include "mainwindow.h"
void Manager::ClearDockWidget()
{
    if (mainWindow->ui->dw_example)
    {
        // std::cout << "ui->dw_example" << std::endl;
        mainWindow->removeDockWidget(mainWindow->ui->dw_example);
        delete mainWindow->ui->dw_example;
        mainWindow->ui->dw_example = nullptr;
    }
}
void Manager::UpdateDockWidget()
{
    mainWindow->update();
}
void Manager::onInitialized()
{
    qDebug() << "Initialized signal received";
    GLWidget *oldWidget = mainWindow->ui->openGLWidget;
    if (oldWidget != nullptr)
    {
        mainWindow->ui->gridLayout_main->removeWidget(oldWidget);
        oldWidget->hide(); // 隐藏旧的 widget
    }
}
void Manager::setGLwidget(GLWidget *newWidget)
{
    if (newWidget == nullptr)
    {
        std::cout << "newWidget == nullptr" << std::endl;
        return;
    }

    // 检查 newWidget 是否具有 initialized 信号
    const QMetaObject *metaObj = newWidget->metaObject();
    int signalIndex = metaObj->indexOfSignal("initialized()");
    // qDebug() << "signalIndex is" << signalIndex;

    if (signalIndex >= 0)
    {
        // 如果已经有 openGLWidget, 那么先隐藏它而不是立即删除
        GLWidget *oldWidget = mainWindow->ui->openGLWidget;
        connect(newWidget, SIGNAL(initialized()), this, SLOT(onInitialized()));
        newWidget->setMinimumSize(QSize(800, 600));
        mainWindow->ui->openGLWidget = newWidget;
        mainWindow->ui->gridLayout_main->addWidget(mainWindow->ui->openGLWidget, 1, 1, 1, 1);
        delete oldWidget;
        // mainWindow->ui->openGLWidget->hide( );
        // todo 设置这个不知为何会崩溃,后续研究一下
        // qDebug( ) << "mainWindow->ui->openGLWidget->hide( );";
        // mainWindow->ui->openGLWidget->show( );
        // qDebug( ) << "mainWindow->ui->openGLWidget->show( );";
    }
    else
    {
        mainWindow->ui->gridLayout_main->removeWidget(mainWindow->ui->openGLWidget);
        delete mainWindow->ui->openGLWidget;
        mainWindow->ui->openGLWidget = nullptr;
        newWidget->setMinimumSize(QSize(800, 600));
        mainWindow->ui->openGLWidget = newWidget;
        mainWindow->ui->gridLayout_main->addWidget(mainWindow->ui->openGLWidget, 1, 1, 1, 1);
    }

    // mainWindow->ui->gridLayout_main->removeWidget(mainWindow->ui->openGLWidget);
    // delete mainWindow->ui->openGLWidget;
    // mainWindow->ui->openGLWidget = nullptr;
    // newWidget->setMinimumSize(QSize(800, 600));
    // mainWindow->ui->openGLWidget = newWidget;
    // mainWindow->ui->gridLayout_main->addWidget(mainWindow->ui->openGLWidget, 1, 1, 1, 1);
}

void Manager::setupSurfaceFormat(int samples)
{
    QSurfaceFormat format;
    format.setSamples(samples);                     // 设置多采样级别
    format.setDepthBufferSize(24);                  // 设置深度缓冲区大小
    format.setStencilBufferSize(8);                 // 设置模板缓冲区大小
    format.setVersion(3, 3);                        // 设置OpenGL版本
    format.setProfile(QSurfaceFormat::CoreProfile); // 设置OpenGL核心模式
    QSurfaceFormat::setDefaultFormat(format);
}
void Manager::UpdateCameraDockWidget(QWidget *camera_widget)
{
    // qDebug() << "start UpdateCameraDockWidget";
    //  qDebug() << "dw_camera address:" << static_cast<void *>(mainWindow->ui->dw_camera);
    //  qDebug() << "camera_widget address:" << static_cast<void *>(mainWindow->ui->camera_widget);
    if (mainWindow->ui->dw_camera && mainWindow->ui->camera_widget)
    {
        mainWindow->ui->gridLayout_9->removeWidget(mainWindow->ui->camera_widget);
        delete mainWindow->ui->camera_widget;
        mainWindow->ui->camera_widget = nullptr;
        mainWindow->ui->camera_widget = camera_widget;
        mainWindow->ui->gridLayout_9->addWidget(mainWindow->ui->camera_widget);
    }
    else
    {
        qDebug() << "UpdateCameraDockWidget failed";
    }
}
void Manager::ClearGL()
{
    qDebug() << "Manager::ClearGL() start";

    if (mainWindow->ui->openGLWidget)
    {
        mainWindow->ui->gridLayout_main->removeWidget(mainWindow->ui->openGLWidget);
        delete mainWindow->ui->openGLWidget;
        mainWindow->ui->openGLWidget = nullptr;
    }
    if (mainWindow->ui->dw_camera && mainWindow->ui->camera_widget)
    {
        mainWindow->ui->gridLayout_9->removeWidget(mainWindow->ui->camera_widget);
        delete mainWindow->ui->camera_widget;
        mainWindow->ui->camera_widget = nullptr;
        mainWindow->ui->camera_widget = new QWidget(mainWindow->ui->dw_camera);
        mainWindow->ui->gridLayout_9->addWidget(mainWindow->ui->camera_widget);
    }
    qDebug() << "Manager::ClearGL() end";
}
void Manager::UpdateGL()
{
    qDebug() << "Manager::UpdateGL() start";
    mainWindow->update();
    qDebug() << "Manager::UpdateGL() end";
}

////Dockwidget
void Manager::hello_triangle_dockwidget()
{
    addDockWidgetToLayout(pHelloTriangleDockWidget);
}
void Manager::hello_shader_dockwidget()
{
    addDockWidgetToLayout(pShadersDockWidget);
}
void Manager::texture_dockwidget()
{
    addDockWidgetToLayout(pTextureDockWidget);
}
void Manager::transform_dockwidget()
{
    addDockWidgetToLayout(pTransformationsDockWidget);
}
void Manager::corrdinate_dockwidget()
{
    addDockWidgetToLayout(pCoordinateSystemsDockWidget);
}
void Manager::camera_dockwidget()
{
    addDockWidgetToLayout(pCameraDockWidget);
}
void Manager::review_dockwidget()
{
    addDockWidgetToLayout(pReviewDockWidget);
}

void Manager::basic_lighting_dockwidget()
{
    addDockWidgetToLayout(pBasicLighting_DockWidget);
}

void Manager::materials_dockwidget()
{
    addDockWidgetToLayout(pMaterials_DockWidget);
}

void Manager::lighting_maps_dockwidget()
{
    addDockWidgetToLayout(pLightingMaps_DockWidget);
}

void Manager::light_casters_dockwidget()
{
    addDockWidgetToLayout(pLightCasters_DockWidget);
}

void Manager::multiple_lights_dockwidget()
{
    addDockWidgetToLayout(pMultiLights_DockWidget);
}

void Manager::review_lights_dockwidget()
{
    addDockWidgetToLayout(pReviewLighting_DockWidget);
}

void Manager::load_model_dockwidget()
{
    addDockWidgetToLayout(pLoadModel_DockWidget);
}

void Manager::load_advancedOpenGL_dockwidget()
{
    qDebug() << "load_advancedOpenGL_dockwidget";
    addDockWidgetToLayout(pAdvancedOpenGL_DockWidget);
}

void Manager::load_advancedLighting_dockwidget()
{
    qDebug() << "load_advancedLighting_dockwidget";
    addDockWidgetToLayout(pAdvancedLighting_DockWidget);
}

// --OpenglWidget
