#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <CameraUtil.h>
#include "glWidget.h"
#include <QElapsedTimer>
class CameraClass : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE CameraClass(QWidget *parent = nullptr);
    ~CameraClass();

protected:
    void custom_initializeGL() override;
    void custom_resizeGL(int w, int h) override;
    // void paintGL() override;
    void custom_paintGL() override;

    virtual bool event(QEvent *e) override;
    void mupdate()
    {
        this->update();
    }

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;

private:
    QElapsedTimer timer;   // 用于计算帧率
    QTimer *refreshTimer;  // 定时器，用于定期刷新画面
    int frameCount;        // 记录帧数
    double fps;            // 当前帧率
    qint64 lastUpdateTime; // 上一次计算帧率的时间戳
};

#endif // CAMERACLASS_H
