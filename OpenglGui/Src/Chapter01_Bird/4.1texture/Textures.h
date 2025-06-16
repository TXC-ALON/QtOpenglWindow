#ifndef TEXTURES_H
#define TEXTURES_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <memory>
#include "glWidget.h"

class Textures : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Textures(QWidget *parent = nullptr);
    ~Textures();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[3];
    int currentTextureIndex = 0;
    void selectTexture();
};

#endif // TEXTURES_H
