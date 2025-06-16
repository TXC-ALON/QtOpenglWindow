
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
class Face_culling : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Face_culling(QWidget *parent = nullptr);
    ~Face_culling( );

protected:
    virtual void initializeGL( ) override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL( ) override;
    virtual bool event(QEvent *e) override;

public:
    // QWidget *createControlPanel() override;
signals:
    void initialized( );

private:
    QOpenGLVertexArrayObject m_cubeVAO, m_planeVAO;
    QOpenGLBuffer            m_cubeVBO, m_planeVBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr< QOpenGLTexture > m_texture[2];

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};
