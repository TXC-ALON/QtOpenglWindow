
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "OpenGLUniformbufferObject.h"
class Geometry_Shader_normals : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Geometry_Shader_normals(QWidget *parent = nullptr);
    ~Geometry_Shader_normals();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;
public slots:
    void onSliderValueChanged();
signals:
    void initialized();

private:
    void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

private:
    std::unique_ptr<ShaderUtil> m_shader;
    std::unique_ptr<ShaderUtil> m_normalShader;
    std::unique_ptr<ShaderUtil> m_middle_normal_Shader;
    std::unique_ptr<ShaderUtil> m_wire_Shader;

    std::unique_ptr<ModelUtil> m_model;

    bool m_normal = true;
    bool m_middle_normal = false;
    bool m_wire = false;
    float m_Radio = 0.0001;
    SliderWithLabel *radio_slider = nullptr;
    CameraUtil m_camera;
};
