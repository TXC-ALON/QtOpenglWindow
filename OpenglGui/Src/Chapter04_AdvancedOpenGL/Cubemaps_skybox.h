
#pragma once
#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"
#include "ShaderUtil.h"
#include "CubemapTextureUtil.h"
class Cubemaps_skybox : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Cubemaps_skybox(QWidget *parent = nullptr);
    ~Cubemaps_skybox();

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
    QOpenGLVertexArrayObject m_cubeVAO, m_skyboxVAO;
    QOpenGLBuffer m_cubeVBO, m_skyboxVBO;

    std::unique_ptr<ShaderUtil> m_shader;
    std::unique_ptr<ShaderUtil> m_skyboxShader;

    std::unique_ptr<CubemapTextureUtil> m_cubeCubeTexture;

    std::unique_ptr<CubemapTextureUtil> m_cubemapTexture;
    QOpenGLTexture m_cubeTexture;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    CameraUtil m_camera;
};
