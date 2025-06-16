#ifndef LIGHTCASTERSPOINT_H
#define LIGHTCASTERSPOINT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QSlider>
#include <QLabel>
#include "CameraUtil.h"
#include "glWidget.h"
#include "PositionSpinbox.h"
#include "QComboBox"

class LightCastersPoint : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE LightCastersPoint(QWidget *parent = nullptr);
    ~LightCastersPoint();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

    virtual bool event(QEvent *e) override;
    QWidget *createControlPanel() override;

private:
    void handleTimeout();
    PositionSpinbox *pPositionSpinbox = nullptr;
    void onPositionChanged();
private slots:
    void onDistanceChanged(int);

private:
    struct Data
    {
        double constant;
        double linear;
        double quadratic;
    };
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    std::unique_ptr<QOpenGLTexture> m_textureDiffuse;
    std::unique_ptr<QOpenGLTexture> m_textureSpecular;
    std::unique_ptr<QOpenGLTexture> m_textureEmission;

    float m_matrixLight;
    float m_matrixMove;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    CameraUtil m_camera;

    QTimer m_timer;

    QSlider *m_sliderLightConstant;
    QSlider *m_sliderLightLinear;
    QSlider *m_sliderLightQuadratic;

    double constant = 1.0;
    double linear = 0.09;
    double quadratic = 0.032;

    QMap<int, LightCastersPoint::Data> data;
    QMap<int, LightCastersPoint::Data> prepare_data();
    QComboBox *comboBox;
    QLabel *labelConstant;
    QLabel *labelLinear;
    QLabel *labelQuadratic;
};

#endif // LIGHTCASTERSPOINT_H
