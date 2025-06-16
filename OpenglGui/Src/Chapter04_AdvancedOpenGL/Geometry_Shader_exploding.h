
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Geometry_Shader_exploding : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Geometry_Shader_exploding(QWidget *parent = nullptr);
    ~Geometry_Shader_exploding();

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
    std::unique_ptr<ShaderUtil> m_shader;
    std::unique_ptr<ModelUtil> m_model;

    CameraUtil m_camera;
};
