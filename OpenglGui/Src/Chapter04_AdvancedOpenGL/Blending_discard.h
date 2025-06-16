
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
class Blending_discard : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Blending_discard(QWidget *parent = nullptr);
    ~Blending_discard();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;
    void onSwitchShader_slot(QAbstractButton *button);
signals:
    void initialized();

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO, m_transparentVAO;
    QOpenGLBuffer m_cubeVBO, m_planeVBO, m_transparentVBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[3];

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_vegetation;

    CameraUtil m_camera;
};
