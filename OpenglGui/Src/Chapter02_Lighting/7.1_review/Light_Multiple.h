#pragma once

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
#include <QRadioButton>
#include "CameraUtil.h"
#include "glWidget.h"
#include "PositionSpinbox.h"
#include <QComboBox>
#include "SliderWithLabel.h"
class Light_Multiple : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Light_Multiple(QWidget *parent = nullptr);
    ~Light_Multiple();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    QWidget *createControlPanel() override;
    virtual bool event(QEvent *e) override;

private:
    void handleRadio();
private slots:
    void onPositionChanged();
    void onDistanceChanged(int);
    void onSliderValueChanged();
    void onSoftButtonChanged(int state);

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    std::unique_ptr<QOpenGLTexture> m_textureDiffuse;
    std::unique_ptr<QOpenGLTexture> m_textureSpecular;

    QVector3D m_spotlightPos;

    QMatrix4x4 m_projection;

    std::vector<QVector3D> m_pointLightPositions;
    std::vector<QVector3D> m_cubePositions;
    std::vector<QMatrix4x4> m_models;

    CameraUtil m_camera;

    QRadioButton *m_radioButtons[5];
    struct Data
    {
        double constant;
        double linear;
        double quadratic;
    };
    // styles
    struct DirLight
    {
        QVector3D direction;

        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };

    struct PointLight
    {
        QVector3D position;

        float constant;
        float linear;
        float quadratic;

        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };

    struct SpotLight
    {
        QVector3D position;
        QVector3D direction;
        float cutOff;
        float outerCutOff;

        float constant;
        float linear;
        float quadratic;

        QVector3D ambient;
        QVector3D diffuse;
        QVector3D specular;
    };

    void set_spot_light();
    QVector4D m_backgroundColor;
    DirLight m_dirLight;
    SpotLight m_spotLight;
    std::vector<QVector3D> m_pointLightColors;
    std::vector<PointLight> m_pointLightValues;

    QList<SliderWithLabel *> light_controls;

    QCheckBox *m_checkBox;

    double constant = 1.0;
    double linear = 0.09;
    double quadratic = 0.032;
    PositionSpinbox *pPositionSpinbox = nullptr;
    QMap<int, Light_Multiple::Data> data;
    QMap<int, Light_Multiple::Data> prepare_data();
    QComboBox *comboBox;
    QLabel *labelConstant;
    QLabel *labelLinear;
    QLabel *labelQuadratic;

    double m_sliderCutOff = 15;
    double m_sliderOuterCutOff = 15;
};
