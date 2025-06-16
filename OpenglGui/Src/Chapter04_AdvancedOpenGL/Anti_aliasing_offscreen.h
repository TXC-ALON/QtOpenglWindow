
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include <QSurfaceFormat>
#include <QOpenGLFramebufferObject>
class Anti_aliasing_offscreen : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Anti_aliasing_offscreen(QWidget *parent = nullptr);
    ~Anti_aliasing_offscreen();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    // QWidget *createControlPanel() override;
signals:
    void initialized();

private:
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    QOpenGLVertexArrayObject cubeVAO;
    QOpenGLBuffer cubeVBO;
    std::unique_ptr<ShaderUtil> m_shader;

    QOpenGLVertexArrayObject quadVAO;
    QOpenGLBuffer quadVBO;
    std::unique_ptr<ShaderUtil> m_screenshader;

    CameraUtil m_camera;

    std::unique_ptr<QOpenGLFramebufferObject> m_multisampleFBO;
    std::unique_ptr<QOpenGLFramebufferObject> m_intermediateFBO;
};
