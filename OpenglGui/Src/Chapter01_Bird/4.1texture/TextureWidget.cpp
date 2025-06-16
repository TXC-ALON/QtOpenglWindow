#include "TextureWidget.h"

#include <iostream>

static GLfloat vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
};

TextureWidget::TextureWidget(QWidget *parent)
    : GLWidget(parent)
{
    std::cout << "TextureWidget start" << std::endl;
}

TextureWidget::~TextureWidget()
// 建议在析构函数中手动销毁openGL相关的对象，
// 文档中特意提到QT的回收机制难以保证回收所有openGL使用的资源
// 不销毁的话在关闭程序时可能会出现异常
{
    makeCurrent();
    m_vao.destroy();
    m_vbo.destroy();
    doneCurrent();
}

void TextureWidget::initializeGL()
{

    initializeOpenGLFunctions();

    // 创建并绑定着色器程序
    program = new QOpenGLShaderProgram;
    program->bind();
    std::cout << "bind" << std::endl;
    // 向program中添加顶点着色器
    if (!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/04_texture/textures_widget.frag"))
    {
        qDebug() << (program->log());
        return;
    }
    // 向program中添加片段着色器
    if (!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/04_texture/textures_widget.vert"))
    {
        qDebug() << (program->log());
        return;
    }
    if (!program->link())
    {
        qDebug() << (program->log());
        return;
    }

    // 创建并绑定VAO
    m_vao.create();
    m_vao.bind();

    // 创建并绑定VBO
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices)); // 向VBO传递我们准备好的数据(本文件起始部分的静态数组)

    // 3.1 位置
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
    program->enableAttributeArray(0);
    // 3.2 颜色
    program->setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
    program->enableAttributeArray(1);
    // 3.3 纹理
    program->setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
    program->enableAttributeArray(2);

    // texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/awesomeface.png").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/container.jpg").mirrored());
    for (int i = 0; i < 2; ++i)
    {
        m_texture[i]->create();
        // 纹理环绕方式
        m_texture[i]->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_texture[i]->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        // 纹理过滤
        m_texture[i]->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        // 多级渐远纹理
        m_texture[i]->generateMipMaps();
        m_texture[i]->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    }
}

void TextureWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    program->bind(); // 绑定绘制所要使用的openGL程序
    m_texture[0]->bind(0);
    program->setUniformValue("texture1", 0);
    m_texture[1]->bind(1);
    program->setUniformValue("texture2", 1);
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // GL_POLYGON在3.x版本已弃用
    m_texture[0]->release();
    m_texture[1]->release();
    m_vao.release(); // 解绑VAO
    program->release();
}

void TextureWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}
