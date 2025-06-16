#include "HelloTriangleExercise1.h"

HelloTriangleExercise1::HelloTriangleExercise1(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_IBO(QOpenGLBuffer::IndexBuffer)
{
}

HelloTriangleExercise1::~HelloTriangleExercise1()
{
    makeCurrent();

    m_IBO.destroy();
    m_VBO.destroy();
    m_VAO.destroy();

    doneCurrent();
}

void HelloTriangleExercise1::initializeGL()
{
    if (!context())
    {
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
            -0.9f, -0.5f, 0.0f, // left
            -0.0f, -0.5f, 0.0f, // right
            -0.45f, 0.5f, 0.0f, // top
                                // second triangle
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f   // top
        };
        unsigned int indices[] = {
            // 注意索引从0开始!
            0, 1, 2, // 第一个三角形
            1, 3, 4  // 第二个三角形
        };
        // 1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
        m_IBO.create();
        m_IBO.bind();
        m_IBO.allocate(indices, sizeof(indices));
        // 4. 设置顶点属性指针
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        m_program.enableAttributeArray(0);
    }
}

void HelloTriangleExercise1::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void HelloTriangleExercise1::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program.bind();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    set_orth_projection(m_program);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_program.release();
}
