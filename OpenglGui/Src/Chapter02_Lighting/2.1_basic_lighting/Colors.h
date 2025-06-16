#ifndef COLORS_H
#define COLORS_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QLabel>
#include "CameraUtil.h"
#include "glWidget.h"
class Colors : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Colors(QWidget *parent = nullptr);
    ~Colors( );

protected:
    virtual void initializeGL( ) override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL( ) override;

    virtual bool event(QEvent *e) override;

private:
    QOpenGLVertexArrayObject m_lightVAO;
    QOpenGLVertexArrayObject m_lightCubeVAO;
    QOpenGLBuffer            m_VBO;

    QOpenGLShaderProgram m_lightShader;
    QOpenGLShaderProgram m_lightCubeShader;

    QVector3D m_lightPos;

    QMatrix4x4 m_projection;

    CameraUtil m_camera;

    QLabel *m_labelYaw;
    QLabel *m_labelPitch;
};

#endif    // COLORS_H
