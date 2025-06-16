
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Advanced_GLSL : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Advanced_GLSL(QWidget *parent = nullptr);
    ~Advanced_GLSL();

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
    QOpenGLVertexArrayObject m_cubeVAO;
    QOpenGLBuffer m_cubeVBO;

    std::unique_ptr<OpenGLUniformbufferObject> m_UBO;

    std::unique_ptr<ShaderUtil> m_shaderRed;
    std::unique_ptr<ShaderUtil> m_shaderGreen;
    std::unique_ptr<ShaderUtil> m_shaderBlue;
    std::unique_ptr<ShaderUtil> m_shaderYellow;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};
