#ifndef TEXTURESEXERCISE2_H
#define TEXTURESEXERCISE2_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <memory>
#include "glWidget.h"

class TexturesExercise2 : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE TexturesExercise2(QWidget *parent = nullptr);
    ~TexturesExercise2();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;

    QOpenGLShaderProgram m_program;

    std::unique_ptr<QOpenGLTexture> m_texture[2];
};

#endif // TEXTURESEXERCISE2_H
