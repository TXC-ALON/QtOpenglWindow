#include "HelloTriangle.h"

HelloTriangle::HelloTriangle(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer)
{
}

HelloTriangle::~HelloTriangle()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();

    doneCurrent();
}

void HelloTriangle::initializeGL()
{
    if (!context())
    {
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();
    // shader初始化
    {
        bool result = true;
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/02_hellotriangle/hello_triangle.vert");
        if (!result)
        {
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/02_hellotriangle/hello_triangle.frag");
        if (!result)
        {
            qDebug() << m_program.log();
        }
        result = m_program.link();
        if (!result)
        {
            qDebug() << m_program.log();
        }
    }
    //
    {
        // 2D triangle
        GLfloat vertices[] = {
            -0.5f, -0.5f, 0.0f, // left
            0.5f, -0.5f, 0.0f,  // right
            0.0f, 0.5f, 0.0f    // top
        };
        // 1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        // 将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
        m_VBO.allocate(vertices, sizeof(vertices));
        // 3. 设置顶点属性指针
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        m_program.enableAttributeArray(0);
    }
}

void HelloTriangle::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HelloTriangle::custom_paintGL()
{
    init_paintGL();
    m_program.bind();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    set_orth_projection(m_program);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_program.release();
}
