#include "GLWidget.h"
#include <QtWidgets>
#include "Utils_Widget.h"
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      currentDrawMode(FillMode),
      frameCount(0),
      fps(0.0),
      lastUpdateTime(0),
      show_Fps(false),
      real_time_render(true)

{
    timer.start(); // 启动计时器
    setFocusPolicy(Qt::StrongFocus);
    initFpslabel();
}

GLWidget::~GLWidget()
{
    // qDebug() << "~GLWidget() start";
    delete m_labelFps;
    // qDebug() << "~GLWidget() end";
}

void GLWidget::applyDrawMode()
{
    switch (currentDrawMode)
    {
    case PointMode:
        // 设置点的大小为5.0像素
        glPointSize(5.0f);         // 这里的值可以根据需要调整
        glEnable(GL_POINT_SMOOTH); // 启用点平滑
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case WireframeMode:
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case FillMode:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    }
}
void GLWidget::setDrawMode(DrawMode mode)
{
    if (currentDrawMode != mode)
    {
        qDebug() << "setDrawMode is " << mode;
        currentDrawMode = mode;
        update(); // Trigger repaint
    }
}
void GLWidget::createDrawModeControl()
{
    QStringList button_list = {"pb_Point Mode", "pb_Wireframe Mode", "pb_Fill Mode"};
    QStringList button_name_list = {"Point Mode", "Wireframe Mode", "Fill Mode"};
    QList<QPushButton *> DrawMode_buttons;
    QWidget *control_draw = createButtomsControl(this, button_list, button_name_list, DrawMode_buttons);
    connect(DrawMode_buttons[0], &QPushButton::clicked, this, [this]()
            { this->setDrawMode(DrawMode::PointMode); });
    connect(DrawMode_buttons[1], &QPushButton::clicked, this, [this]()
            { this->setDrawMode(DrawMode::WireframeMode); });
    connect(DrawMode_buttons[2], &QPushButton::clicked, this, [this]()
            { this->setDrawMode(DrawMode::FillMode); });
}

void GLWidget::initializeGL()
{
    custom_initializeGL();
}

void GLWidget::resizeGL(int w, int h)
{
    custom_resizeGL(w, h);
}
void GLWidget::paintGL()
{
    custom_paintGL();
    if (show_Fps)
    {
        updateFps();
    }
    if (real_time_render)
    {
        QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
    }
}
void GLWidget::custom_initializeGL()
{
}
void GLWidget::custom_resizeGL(int w, int h)
{
}
void GLWidget::custom_paintGL()
{
}
void GLWidget::init_paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    applyDrawMode();
}

void GLWidget::end_paintGL()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLWidget::set_orth_projection(QOpenGLShaderProgram &m_program)
{
    QMatrix4x4 m_projection;
    m_projection.setToIdentity();
    // const float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    const float aspectRatio = static_cast<float>(width()) / static_cast<float>(height());
    const float orthoSize = 1.0f; // 可以根据需要调整这个值来改变三角形的大小
    m_projection.ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, -1.0f, 1.0f);
    m_program.setUniformValue("projection", m_projection);
}

void GLWidget::FPS_Show(bool close, bool real_time_render_close)
{
    show_Fps = !close;
    real_time_render = !real_time_render_close;
    if (show_Fps)
    {
        real_time_render = true;
    }
}

void GLWidget::initFpslabel()
{
    if (m_labelFps != nullptr)
    {
        delete m_labelFps;
        m_labelFps = nullptr;
    }
    m_labelFps = new QLabel(this);
    m_labelFps->setGeometry(10, 10, 100, 20);              // x, y, width, height
    m_labelFps->setFont(QFont("Arial", 12));               // 设置字体和大小
    m_labelFps->setStyleSheet("QLabel { color: white; }"); // 设置样式表
}

void GLWidget::updateFps()
{
    // 累计帧数
    frameCount++;
    // 获取当前时间
    qint64 currentTime = timer.elapsed();
    // 每 0.1 秒计算一次帧率
    if (currentTime - lastUpdateTime >= 100)
    { // 100 毫秒
        qint64 elapsedTime = currentTime - lastUpdateTime;
        fps = frameCount * 1000.0 / elapsedTime; // 计算帧率
        frameCount = 0;                          // 重置帧计数
        lastUpdateTime = currentTime;            // 更新时间戳
        // 输出帧率到控制台（可选）
        // qDebug() << "FPS:" << fps;
    }
    m_labelFps->setText(QString::asprintf("FPS: %.2f", fps));
}
