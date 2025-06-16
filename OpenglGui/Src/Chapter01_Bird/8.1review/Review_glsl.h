#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "glWidget.h"
#include <QVector2D>
using namespace Qt;
class Review_GLSL : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Review_GLSL(QWidget *parent = nullptr);
    ~Review_GLSL();

protected:
    void custom_initializeGL() override;
    void custom_resizeGL(int w, int h) override;
    void custom_paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLShaderProgram m_program_triangle;

    QOpenGLVertexArrayObject m_VAO_origin;
    QOpenGLBuffer m_VBO_origin;
    QOpenGLShaderProgram m_program_origin;

    QTimer m_timer;
    float time = 0;
    float m_angleSelf;  // 自转角度
    float m_angleOrbit; // 公转角度
    void updateAngles();
    QVector3D m_cameraPos;
    QVector2D center = QVector2D(2.0, 2.0);
    float sideLength = 1.0f;
    float height = sqrtf(3.0f) / 2.0f * sideLength;

    QMatrix4x4 projection;
};
