#ifndef HELLOTRIANGLE_H
#define HELLOTRIANGLE_H

#include "glWidget.h"
class HelloTriangle : public GLWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE HelloTriangle(QWidget *parent = nullptr);
    ~HelloTriangle();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void custom_paintGL() override;

private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO;
    QOpenGLShaderProgram m_program;
};
#endif // HELLOTRIANGLE_H