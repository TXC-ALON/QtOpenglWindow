
#include "Anti_aliasing_offscreen.h"
#define MSAA_LEVEL 16
Anti_aliasing_offscreen::Anti_aliasing_offscreen(QWidget *parent)
    : GLWidget(parent),
      cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this)
{
    QSurfaceFormat format = this->format();
    qDebug() << "Samples:" << format.samples();
}
Anti_aliasing_offscreen::~Anti_aliasing_offscreen()
{
    // qDebug() << "~Anti_aliasing_offscreen start";
    makeCurrent();
    cubeVAO.destroy();
    cubeVBO.destroy();
    quadVAO.destroy();
    quadVBO.destroy();
    doneCurrent();
    // qDebug() << "~Anti_aliasing_offscreen end";
}

void Anti_aliasing_offscreen::initializeGL()
{
    // qDebug() << "Anti_aliasing_offscreen initializeGL start";

    if (!context())
    {
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/09_Anti_aliasing/anti_aliasing.vert",
                                                ":/Resources/shaders/04_Advanced/09_Anti_aliasing/anti_aliasing.frag");
        m_screenshader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/09_Anti_aliasing/aa_post.vert",
                                                      ":/Resources/shaders/04_Advanced/09_Anti_aliasing/aa_post.frag");
        if (!m_shader->isLinked())
        {
            qDebug() << "ERROR::SHADER:: Shader program failed to link!";
            qDebug() << m_shader->log();
        }
        if (!m_screenshader->isLinked())
        {
            qDebug() << "ERROR::SHADER:: Screen shader program failed to link!";
            qDebug() << m_screenshader->log();
        }
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    GLfloat cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f};
    GLfloat quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                              // positions   // texCoords
                              -1.0f, 1.0f, 0.0f, 1.0f,
                              -1.0f, -1.0f, 0.0f, 0.0f,
                              1.0f, -1.0f, 1.0f, 0.0f,

                              -1.0f, 1.0f, 0.0f, 1.0f,
                              1.0f, -1.0f, 1.0f, 0.0f,
                              1.0f, 1.0f, 1.0f, 1.0f};

    // 设置立方体VAO和VBO
    cubeVAO.create();
    cubeVAO.bind();
    cubeVBO.create();
    cubeVBO.bind();
    cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
    m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    m_shader->enableAttributeArray(0);
    cubeVAO.release();

    // 设置屏幕四边形VAO和VBO
    quadVAO.create();
    quadVAO.bind();
    quadVBO.create();
    quadVBO.bind();
    quadVBO.allocate(quadVertices, sizeof(quadVertices));
    m_screenshader->setAttributeBuffer(0, GL_FLOAT, 0, 2, 4 * sizeof(GLfloat));
    m_screenshader->enableAttributeArray(0);
    m_screenshader->setAttributeBuffer(1, GL_FLOAT, 2 * sizeof(GLfloat), 2, 4 * sizeof(GLfloat));
    m_screenshader->enableAttributeArray(1);
    quadVAO.release();

    // 创建多重采样 FBO
    QOpenGLFramebufferObjectFormat multisampleFormat;
    multisampleFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    multisampleFormat.setSamples(MSAA_LEVEL); // 设置多重采样级别
    m_multisampleFBO = std::make_unique<QOpenGLFramebufferObject>(this->size(), multisampleFormat);

    if (!m_multisampleFBO->isValid())
    {
        qDebug() << "ERROR::FRAMEBUFFER:: Multisample framebuffer is not complete!";
    }

    // 创建中间 FBO，用于后处理
    m_intermediateFBO = std::make_unique<QOpenGLFramebufferObject>(this->size(), QOpenGLFramebufferObject::NoAttachment);

    if (!m_intermediateFBO->isValid())
    {
        qDebug() << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!";
    }
    // 相机类初始化
    // qDebug() << " initializeGL FBO size:" << m_multisampleFBO->size().width() << "x" << m_multisampleFBO->size().height();

    m_camera.init();
}

void Anti_aliasing_offscreen::resizeGL(int w, int h)
{
    // qDebug() << "Anti_aliasing_offscreen resizeGL start";

    // 获取当前的设备像素比率
    qreal ratio = devicePixelRatioF();
    // qDebug() << " resizeGL ratio:" << ratio;
    //  调整视口大小，使用逻辑尺寸乘以设备像素比率得到物理像素尺寸
    glViewport(0, 0, w * ratio, h * ratio);

    // 创建或更新 FBO 时也考虑到设备像素比率
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(MSAA_LEVEL);

    m_multisampleFBO.reset(new QOpenGLFramebufferObject(w * ratio, h * ratio, format));
    m_intermediateFBO.reset(new QOpenGLFramebufferObject(w * ratio, h * ratio, QOpenGLFramebufferObject::NoAttachment));

    // qDebug() << " resizeGL FBO size:" << m_multisampleFBO->size().width() << "x" << m_multisampleFBO->size().height();
}

void Anti_aliasing_offscreen::paintGL()
{
    QSize widgetSize = this->size();
    qreal ratio = devicePixelRatioF(); // 获取设备像素比率
    // qDebug() << "QOpenGLWidget size:" << widgetSize.width() << "x" << widgetSize.height();
    // qDebug() << "QOpenGLWidget physical size:" << widgetSize.width() * ratio << "x" << widgetSize.height() * ratio;
    // 检查 OpenGL 错误
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        qDebug() << "OpenGL Error before rendering:" << error;
    }
    //  1. Render to multisample FBO
    m_multisampleFBO->bind();
    glViewport(0, 0, m_multisampleFBO->size().width(), m_multisampleFBO->size().height()); // Set viewport to match FBO size

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 检查 OpenGL 错误
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        qDebug() << "OpenGL Error after clearing buffers:" << error;
    }
    m_shader->bind();
    if (!m_shader->isLinked())
    {
        qDebug() << "Shader program is not linked!";
    }
    m_model.setToIdentity();
    m_model.rotate(-45, QVector3D(1, 1, 0));
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * m_multisampleFBO->size().width() / m_multisampleFBO->size().height(), 0.1, 100.0); // Use FBO's aspect ratio
    m_shader->setUniformValue("projection", m_projection);
    m_shader->setUniformValue("view", m_camera.getViewMatrix());
    m_shader->setUniformValue("model", m_model);

    cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    cubeVAO.release();
    // 检查 OpenGL 错误
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        qDebug() << "OpenGL Error after drawing:" << error;
    }

    // 检查 OpenGL 错误
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        qDebug() << "OpenGL Error after release:" << error;
    }
    // Blit the multisampled FBO to the default framebuffer (screen)
    GLuint defaultFBO = QOpenGLContext::currentContext()->defaultFramebufferObject();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multisampleFBO->handle());

    GLenum status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "Read FBO is not complete: " << status;
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, defaultFBO);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        qDebug() << "Draw FBO is not complete: " << status;
    }
    glViewport(0, 0, widgetSize.width() * ratio, widgetSize.height() * ratio);
    glBlitFramebuffer(
        0, 0, m_multisampleFBO->size().width(), m_multisampleFBO->size().height(),
        0, 0, widgetSize.width() * ratio, widgetSize.height() * ratio,
        GL_COLOR_BUFFER_BIT, GL_LINEAR);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        qDebug() << "OpenGL Error after blitting:" << error;
    }
    m_multisampleFBO->release();
}

bool Anti_aliasing_offscreen::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

// 折腾了一天，发现那个锯齿就是因为因为屏幕纹理又变回了一个只有单一采样点的普通纹理，
// 像是边缘检测这样的后期处理滤镜会重新导致锯齿。
