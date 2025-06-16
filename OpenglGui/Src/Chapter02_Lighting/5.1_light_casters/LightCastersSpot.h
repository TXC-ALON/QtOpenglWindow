#ifndef LIGHTCASTERSSPOT_H
#define LIGHTCASTERSSPOT_H

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
#include <QCheckBox>
#include "CameraUtil.h"
#include "glWidget.h"
#include "PositionSpinbox.h"
#include <QComboBox>
#include "SliderWithLabel.h"
class LightCastersSpot : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE LightCastersSpot(QWidget *parent = nullptr);
    ~LightCastersSpot();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    QWidget *createControlPanel() override;
    virtual bool event(QEvent *e) override;

private:
    void handleTimeout();
    PositionSpinbox *pPositionSpinbox = nullptr;
private slots:
    void onPositionChanged();
    void onDistanceChanged(int);
    void onSliderValueChanged();
    void onSoftButtonChanged(int state);

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

    QList<SliderWithLabel *> light_controls;

    QCheckBox *m_checkBox;

    double constant = 1.0;
    double linear = 0.09;
    double quadratic = 0.032;

    QMap<int, LightCastersSpot::Data> data;
    QMap<int, LightCastersSpot::Data> prepare_data();
    QComboBox *comboBox;
    QLabel *labelConstant;
    QLabel *labelLinear;
    QLabel *labelQuadratic;

    double m_sliderCutOff = 15;
    double m_sliderOuterCutOff = 15;
};

#endif // LIGHTCASTERSSPOT_H
