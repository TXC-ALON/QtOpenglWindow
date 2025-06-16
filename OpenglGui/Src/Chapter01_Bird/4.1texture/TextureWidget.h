#ifndef GLWidget_H
#define GLWidget_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <memory>
#include "glWidget.h"

class TextureWidget : public GLWidget
{

    Q_OBJECT

public:
    TextureWidget(QWidget *parent = 0);
    ~TextureWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private:
    QOpenGLShaderProgram *program;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;

    int m_attr;
    int m_color;
    std::unique_ptr<QOpenGLTexture> m_texture[2];
};

#endif // GLWidget_H
