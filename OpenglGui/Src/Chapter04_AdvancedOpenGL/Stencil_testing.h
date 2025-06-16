
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "SliderWithLabel.h"
class Stencil_testing : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Stencil_testing(QWidget *parent = nullptr);
    ~Stencil_testing();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;
signals:
    void initialized();
public slots:
    void onSliderValueChanged();

private:
    double border_weight = 0.1;
    SliderWithLabel *border_weight_slider = nullptr;
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO;
    QOpenGLBuffer m_cubeVBO, m_planeVBO;

    QOpenGLShaderProgram m_normalShader;
    QOpenGLShaderProgram m_singleColorShader;

    std::unique_ptr<QOpenGLTexture> m_texture[2];

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};