
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Asteroids : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Asteroids(QWidget *parent = nullptr);
    ~Asteroids();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

public:
    // QWidget *createControlPanel() override;
signals:
    void initialized();

private:
    std::unique_ptr<ShaderUtil> m_shader;

    std::unique_ptr<ModelUtil> m_modelPlanet;
    std::unique_ptr<ModelUtil> m_modelRock;

    std::vector<QMatrix4x4> m_modelMatrices;

    CameraUtil m_camera;
};
