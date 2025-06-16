
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Instancing_quads : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Instancing_quads(QWidget *parent = nullptr);
    ~Instancing_quads();

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
    QOpenGLVertexArrayObject m_quadVAO;
    QOpenGLBuffer m_quadVBO, m_instanceVBO;
    std::unique_ptr<ShaderUtil> m_shader;

    CameraUtil m_camera;
};
