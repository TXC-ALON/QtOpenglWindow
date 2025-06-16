
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Geometry_Shader_houses : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Geometry_Shader_houses(QWidget *parent = nullptr);
    ~Geometry_Shader_houses();

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
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    std::unique_ptr<ShaderUtil> m_shader;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};
