#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "SliderWithLabel.h"
class Frame_buffers : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Frame_buffers(QWidget *parent = nullptr);
    ~Frame_buffers();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;

public:
    void handlePostProcessingChange(QAbstractButton *button);
public slots:
    void onSliderValueChanged();
signals:
    void initialized();

private:
    void handleLineMode(bool flag);

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO, m_quadVAO;
    QOpenGLBuffer m_cubeVBO, m_planeVBO, m_quadVBO;

    QOpenGLShaderProgram m_shader;
    QOpenGLShaderProgram m_screenShader;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    std::unique_ptr<QOpenGLFramebufferObject> m_FBO;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
    bool m_isPixelMode = false;
    bool m_isLineMode;
    int m_postProcessingId;

    // 新增像素化相关的成员变量
    float m_buf_scale;
    qreal device_ratio;
    SliderWithLabel *pixel_weight_slider = nullptr;
};