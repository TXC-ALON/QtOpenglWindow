#include "Review_camera.h"

#include <QTime>
#include <QMessageBox>

Review_Camera::Review_Camera(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this), frameCount(0), fps(0.0), lastUpdateTime(0)
{
    timer.start(); // 启动计时器

    // 初始化定时器
    refreshTimer = new QTimer(this);
    // connect(refreshTimer, &QTimer::timeout, this, &Review_Camera::mupdate);
    refreshTimer->start(16); // 每 16 毫秒触发一次，约等于 60 FPS
}

Review_Camera::~Review_Camera()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();
    m_texture[0]->destroy();
    m_texture[1]->destroy();
    m_VAO_Axe.destroy();
    m_VBO_Axe.destroy();
    doneCurrent();
}

void Review_Camera::custom_initializeGL()
{
    if (!context())
    {
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    // shader初始化
    {
        bool result = true;
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/06_coordinate/CoordinateSystemsDepth.vert");
        if (!result)
        {
            qDebug() << m_program.log();
        }
        result = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/06_coordinate/CoordinateSystemsDepth.frag");
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
            // 位置              //纹理坐标
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
        // cube positions
        m_cubePositions = {
            {1.0, 1.0, 2.0},
            {2.0, 5.0, -15.0},
            {-1.5, -2.2, -2.5},
            {-3.8, -2.0, -12.3},
            {2.4, -0.4, -3.5},
            {-1.7, 3.0, -7.5},
            {1.3, -2.0, -2.5},
            {1.5, 2.0, -2.5},
            {1.5, 0.2, -1.5},
            {-1.3, 1.0, -1.5}};
        // 1. 绑定顶点数组对象
        m_VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        // 3. 设置顶点属性指针
        // 3.1 位置
        m_program.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
        m_program.enableAttributeArray(0);
        // 3.2 纹理
        m_program.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
        m_program.enableAttributeArray(1);
    }
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
    // MVP
    {
        m_models.resize(m_cubePositions.size());
        for (int i = 0; i < m_cubePositions.size(); ++i)
        {
            m_models[i].translate(m_cubePositions[i]);
        }
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    }

    initializeAxes();

    // 相机类初始化
    m_camera.init();
}

void Review_Camera::custom_resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    qDebug() << "w " << w << " h" << h;
}

void Review_Camera::custom_paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_program.bind();
    m_texture[0]->bind(0);
    m_program.setUniformValue("texture1", 0);
    m_texture[1]->bind(1);
    m_program.setUniformValue("texture2", 1);
    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_program.setUniformValue("projection", m_projection);
    m_program.setUniformValue("view", m_camera.getViewMatrix());
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    for (int i = 0; i < m_cubePositions.size(); ++i)
    {
        // m_models[i].rotate(10.0 , {0.5, 1.0, 0.0});
        m_program.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_texture[0]->release();
    m_texture[1]->release();
    m_program.release();

    QMatrix4x4 mvp = m_projection * m_camera.getViewMatrix();
    drawAxes(mvp);
    // 或者以更紧凑的方式打印整个矩阵
    // qDebug() << "MVP Matrix:" << mvp;
    // 累计帧数
    frameCount++;

    // 获取当前时间
    qint64 currentTime = timer.elapsed();

    // 每 0.1 秒计算一次帧率
    if (currentTime - lastUpdateTime >= 100)
    { // 100 毫秒
        qint64 elapsedTime = currentTime - lastUpdateTime;
        fps = frameCount * 1000.0 / elapsedTime; // 计算帧率
        frameCount = 0;                          // 重置帧计数
        lastUpdateTime = currentTime;            // 更新时间戳

        // 输出帧率到控制台（可选）
        // qDebug() << "FPS:" << fps;
    }
}

bool Review_Camera::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void Review_Camera::drawAxes(QMatrix4x4 mvp)
{
    m_program_Axe.bind();
    m_VAO_Axe.bind();
    glLineWidth(10.0f);
    // 设置线宽为2.0，可以根据需要调整这个值
    glEnable(GL_LINE_SMOOTH); // 启用点平滑
    for (int i = 0; i < 3; ++i)
    {
        glDrawArrays(GL_LINES, i * 2, 2); // 每个轴由两个顶点组成
    }
    m_VAO_Axe.release();

    m_program_Axe.setUniformValue("mvp", mvp);
    m_program_Axe.release();
}

void Review_Camera::initializeAxes()
{
    float vertices_axes[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // Y轴
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // Z轴
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f};

    // shader初始化
    {
        bool result = true;

        // no uniform
        result = m_program_Axe.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/08_review/review_axes.vert");
        if (!result)
        {
            qDebug() << m_program_Axe.log();
        }
        result = m_program_Axe.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/08_review/review_axes.frag");
        if (!result)
        {
            qDebug() << m_program_Axe.log();
        }
        result = m_program_Axe.link();
        if (!result)
        {
            qDebug() << m_program_Axe.log();
        }
    }
    {
        // 1. 绑定顶点数组对象
        m_VAO_Axe.create();
        m_VAO_Axe.bind();
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO_Axe.create();
        m_VBO_Axe.bind();
        // 将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
        m_VBO_Axe.allocate(vertices_axes, sizeof(vertices_axes));
        // 3. 设置顶点属性指针
        // 位置属性
        m_program_Axe.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_program_Axe.enableAttributeArray(0);
        // 颜色属性
        m_program_Axe.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
        m_program_Axe.enableAttributeArray(1);
    }
}
