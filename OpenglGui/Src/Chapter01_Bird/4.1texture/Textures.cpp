#include "Textures.h"

Textures::Textures(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer)
{
    QPushButton *button1 = new QPushButton("Texture awesomeface", this);
    button1->setProperty("textureId", 0); // 设置纹理ID

    QPushButton *button2 = new QPushButton("Texture container", this);
    button2->setProperty("textureId", 1); // 设置纹理ID

    QPushButton *button3 = new QPushButton("Texture wall", this);
    button3->setProperty("textureId", 2); // 设置纹理ID

    button1->setGeometry(10, 10, 150, 30);
    button2->setGeometry(10, 50, 150, 30);
    button3->setGeometry(10, 90, 150, 30);

    connect(button1, &QPushButton::clicked, this, &Textures::selectTexture);
    connect(button2, &QPushButton::clicked, this, &Textures::selectTexture);
    connect(button3, &QPushButton::clicked, this, &Textures::selectTexture);
}

Textures::~Textures()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();
    m_texture[0]->destroy();
    m_texture[1]->destroy();
    m_texture[2]->destroy();

    doneCurrent();
}

void Textures::initializeGL()
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
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/04_texture/textures.vert");
        if (!result)
        {
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/04_texture/textures.frag");
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
        // 2D rect
        GLfloat vertices[] = {
            // 位置              // 颜色               //纹理坐标
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
        };
        // 1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        // 3. 设置顶点属性指针
        // 3.1 位置
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_program.enableAttributeArray(0);
        // 3.2 颜色
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
        m_program.enableAttributeArray(1);
        // 3.3 纹理
        m_program.setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
        m_program.enableAttributeArray(2);
    }
    // texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/awesomeface.png").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/container.jpg").mirrored());
    m_texture[2] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/wall.jpg").mirrored());

    for (int i = 0; i < 3; ++i)
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

void Textures::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Textures::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program.bind();
    if (currentTextureIndex >= 0 && currentTextureIndex < 3)
    {
        m_texture[currentTextureIndex]->bind();
        m_program.setUniformValue("texture_set", 0);
    }
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // GL_POLYGON在3.x版本已弃用
    m_texture[0]->release();
    m_texture[1]->release();
    m_texture[2]->release();

    m_program.release();
}

void Textures::selectTexture()
{
    QObject *senderObj = sender(); // 获取发送信号的对象
    if (QPushButton *button = qobject_cast<QPushButton *>(senderObj))
    {
        // 读取按钮属性作为纹理索引
        currentTextureIndex = button->property("textureId").toInt();
        // 请求重绘
        update();
    }
}
