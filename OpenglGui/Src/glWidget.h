#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QMatrix4x4>

#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLDebugLogger>
// 布局和控件
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QRadioButton>
#include <QMessageBox>
#include <QButtonGroup>

// 标准库
#include <memory>
enum DrawMode
{
    PointMode,
    WireframeMode,
    FillMode
};
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = nullptr);
    virtual ~GLWidget();

public:
    virtual QWidget *createControlPanel() { return nullptr; };
    void applyDrawMode();
    void createDrawModeControl();
private slots:
    void setDrawMode(DrawMode mode);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    virtual void custom_initializeGL();
    virtual void custom_resizeGL(int w, int h);
    virtual void custom_paintGL();
    void init_paintGL();
    void end_paintGL();

    void set_orth_projection(QOpenGLShaderProgram &m_program);

private:
    bool real_time_render = false;
    bool show_Fps = false;
    void FPS_Show(bool fps_close = false, bool real_time_render_close = false);
    QElapsedTimer timer; // 用于计算帧率
    // QTimer *refreshTimer;  // 定时器，用于定期刷新画面
    int frameCount;        // 记录帧数
    double fps;            // 当前帧率
    qint64 lastUpdateTime; // 上一次计算帧率的时间戳
    QLabel *m_labelFps = nullptr;
    void initFpslabel();
    void updateFps();

private:
    DrawMode currentDrawMode;
};

#endif // GLWIDGET_H