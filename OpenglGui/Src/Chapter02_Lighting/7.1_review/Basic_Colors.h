#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QLabel>
#include "glWidget.h"
#include "CameraUtil.h"
#include "SliderWithLabel.h"
#include "PositionSpinbox.h"
class ColorButton;
class Basic_Colors : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Basic_Colors(QWidget *parent = nullptr);
    ~Basic_Colors();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;
    QWidget *createControlPanel() override;

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;

    QLabel *m_labelYaw;
    QLabel *m_labelPitch;
    QLabel *m_labelCameraPos;

private:
    ColorButton *modelcolor;
    ColorButton *lightcolor;
    double ambientStrength = 0.1;
    double specularStrength = 1;
    double specularShininess = 32;
    QList<SliderWithLabel *> light_controls;

    bool useAmbient = true;
    bool useDiffuse = true;
    bool useSpecular = true;
    QCheckBox *checkBoxAmbient;
    QCheckBox *checkBoxDiffuse;
    QCheckBox *checkBoxSpecular;

    PositionSpinbox *pPositionSpinbox = nullptr;

private slots:
    void onSliderValueChanged();
    void onCheckBoxChanged();
    void onPositionChanged();
};
