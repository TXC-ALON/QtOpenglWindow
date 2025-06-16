
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Asteroids_instanced : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Asteroids_instanced(QWidget *parent = nullptr);
    ~Asteroids_instanced();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;
signals:
    void initialized();
private slots:
    void onSliderValueChanged();

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);
    void updateModelMatrices(int planet_num, float radius, float offset);

private:
    QOpenGLBuffer m_VBO;
    std::unique_ptr<ShaderUtil> m_shader;
    std::unique_ptr<ShaderUtil> m_rockShader;

    std::unique_ptr<ModelUtil> m_modelPlanet;
    std::unique_ptr<ModelUtil> m_modelRock;

    struct mat4
    {
        float m11;
        float m12;
        float m13;
        float m14;
        float m21;
        float m22;
        float m23;
        float m24;
        float m31;
        float m32;
        float m33;
        float m34;
        float m41;
        float m42;
        float m43;
        float m44;
    };
    std::vector<mat4> m_modelMatrices;

    std::vector<SliderWithLabel *> m_sliders;
    int planet_num = 100000;
    float radius = 150;
    float offset = 25;
    CameraUtil m_camera;
};
