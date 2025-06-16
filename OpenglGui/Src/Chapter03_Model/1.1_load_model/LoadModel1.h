#pragma once

#include "glWidget.h"
#include "CameraUtil.h"
#include "ModelUtil.h"
#include "Light.h"

class LoadModel1 : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE LoadModel1(QWidget *parent = nullptr);
    ~LoadModel1();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;

public:
    QWidget *createControlPanel() override;
signals:
    void initialized();

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    std::unique_ptr<QOpenGLTexture> m_textureDiffuse;
    std::unique_ptr<QOpenGLTexture> m_textureSpecular;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_pointLightPositions;
    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    CameraUtil m_camera;

    std::unique_ptr<ModelUtil> m_model;

    QVector4D m_backgroundColor = {0.1, 0.1, 0.1, 1.0};

    Direction_Light m_dirLight;
    Spot_Light m_spotLight;
    std::vector<std::unique_ptr<Point_Light>> m_pointLightValues;

    QWidget *currentPointLightPanel = nullptr;
    QComboBox *pointlightComboBox;
    void onPointLightSelected(int index);
    QVBoxLayout *layout;
    void replaceWidget(QVBoxLayout *layout, QWidget *oldWidget, QWidget *newWidget);

    // 绘制坐标轴
    void drawAxes(QMatrix4x4 mvp);
    void initializeAxes();

    QOpenGLShaderProgram m_program_Axe;
    QOpenGLVertexArrayObject m_VAO_Axe;
    QOpenGLBuffer m_VBO_Axe;
};
