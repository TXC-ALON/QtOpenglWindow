#ifndef HELLOTRIANGLEINDEXED_H
#define HELLOTRIANGLEINDEXED_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "glWidget.h"

class HelloTriangleIndexed : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE HelloTriangleIndexed(QWidget *parent = nullptr);
    ~HelloTriangleIndexed();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLBuffer m_IBO; // 明确指定这是索引缓冲
    QOpenGLShaderProgram m_program;
};

#endif // HELLOTRIANGLEINDEXED_H
