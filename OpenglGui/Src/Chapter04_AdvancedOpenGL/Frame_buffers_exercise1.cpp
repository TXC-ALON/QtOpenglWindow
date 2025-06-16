#include "Frame_buffers_exercise1.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOverload>
Frame_buffers_exercise1::Frame_buffers_exercise1(QWidget *parent)
    : GLWidget(parent),
      m_cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_planeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this),
      m_isLineMode(false),
      m_postProcessingId(0)
{
    QCheckBox *cBoxLine = new QCheckBox("draw as wireframe", this);
    cBoxLine->setStyleSheet("QCheckBox{color:white}");
    connect(cBoxLine, &QCheckBox::clicked, this, &Frame_buffers_exercise1::handleLineMode);

    QButtonGroup *radioGroup = new QButtonGroup(this);
    QStringList text;
    text << "normal"
         << "inversion"
         << "gray scale"
         << "blur"
         << "edge detection"
         << "sharpen";
    for (int i = 0; i < text.size(); ++i)
    {
        QRadioButton *radio = new QRadioButton(text[i], this);
        radio->setStyleSheet("QRadioButton {color : white;}");
        radio->setGeometry(0, 20 * i + 20, 120, 20);
        radio->setProperty("id", i); // 设置每个按钮的 id 属性
        radioGroup->addButton(radio, i);
    }
    radioGroup->button(0)->setChecked(true);
    connect(radioGroup, &QButtonGroup::buttonClicked, this, &Frame_buffers_exercise1::handlePostProcessingChange);
}
void Frame_buffers_exercise1::handlePostProcessingChange(QAbstractButton *button)
{
    int id = button->property("id").toInt();
    m_postProcessingId = id;
    qDebug() << "current m_postProcessingId is" << m_postProcessingId;
    update();
}
Frame_buffers_exercise1::~Frame_buffers_exercise1()
{
    makeCurrent();

    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void Frame_buffers_exercise1::initializeGL()
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
        result = m_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/04_Advanced/04_Framebuffers/framebuffers.vert");
        if (!result)
        {
            qDebug() << m_shader.log();
        }
        result = m_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/04_Advanced/04_Framebuffers/framebuffers.frag");
        if (!result)
        {
            qDebug() << m_shader.log();
        }
        result = m_shader.link();
        if (!result)
        {
            qDebug() << m_shader.log();
        }

        result = m_screenShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/04_Advanced/04_Framebuffers/framebuffers_screen.vert");
        if (!result)
        {
            qDebug() << m_screenShader.log();
        }
        result = m_screenShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/04_Advanced/04_Framebuffers/framebuffers_screen.frag");
        if (!result)
        {
            qDebug() << m_screenShader.log();
        }
        result = m_screenShader.link();
        if (!result)
        {
            qDebug() << m_screenShader.log();
        }
    }
    //
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        GLfloat cubeVertices[] = {
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
        GLfloat planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

            5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
            5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

        GLfloat quadVertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                                  // positions   // texCoords
                                  0.5f, 1.0f, 0.0f, 1.0f,
                                  0.5f, 0.6f, 0.0f, 0.0f,
                                  1.0f, 0.6f, 1.0f, 0.0f,

                                  0.5f, 1.0f, 0.0f, 1.0f,
                                  1.0f, 0.6f, 1.0f, 0.0f,
                                  1.0f, 1.0f, 1.0f, 1.0f};

        m_cubeVAO.create();
        m_cubeVAO.bind();
        m_cubeVBO.create();
        m_cubeVBO.bind();
        m_cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
        m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_shader.enableAttributeArray(0);
        m_shader.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_shader.enableAttributeArray(1);
        m_cubeVAO.release();

        m_planeVAO.create();
        m_planeVAO.bind();
        m_planeVBO.create();
        m_planeVBO.bind();
        m_planeVBO.allocate(planeVertices, sizeof(planeVertices));
        m_shader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 5);
        m_shader.enableAttributeArray(0);
        m_shader.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 3, 2, sizeof(GL_FLOAT) * 5);
        m_shader.enableAttributeArray(1);
        m_planeVAO.release();

        m_quadVAO.create();
        m_quadVAO.bind();
        m_quadVBO.create();
        m_quadVBO.bind();
        m_quadVBO.allocate(quadVertices, sizeof(quadVertices));
        m_screenShader.bind();
        m_screenShader.setAttributeBuffer(0, GL_FLOAT, 0, 2, sizeof(GL_FLOAT) * 4);
        m_screenShader.enableAttributeArray(0);
        m_screenShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GL_FLOAT) * 2, 2, sizeof(GL_FLOAT) * 4);
        m_screenShader.enableAttributeArray(1);
        m_quadVAO.release();
    }
    // texture
    m_texture[0] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/container.jpg").mirrored());
    m_texture[1] = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/metal.png").mirrored());
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
    m_shader.bind();
    m_shader.setUniformValue("texture1", 0);
    m_screenShader.bind();
    m_screenShader.setUniformValue("screenTexture", 0);
    // todo 分析这两者的不同
    //  framebuffer configuration
    //  QOpenGLFramebufferObjectFormat fboFormat;
    //  fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    //  m_FBO = std::make_unique< QOpenGLFramebufferObject >(this->size( ), fboFormat);

    m_FBO = std::make_unique<QOpenGLFramebufferObject>(this->size(), QOpenGLFramebufferObject::CombinedDepthStencil);

    if (!m_FBO->isValid())
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";

    // 相机类初始化
    m_camera.init();
}

void Frame_buffers_exercise1::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_FBO.reset(new QOpenGLFramebufferObject(w, h, QOpenGLFramebufferObject::CombinedDepthStencil));
}

void Frame_buffers_exercise1::paintGL()
{
    // is line mode
    if (m_isLineMode)
    {
        // draw as wireframe, 线框模式下纹理没有作用，我们就能看到所画出的简单四边形了！
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        update();
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // bind to framebuffer and draw scene as we normally would to color texture
    m_FBO->bind();
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

    // make sure we clear the framebuffer's content
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();
    // MVP
    float pitch = m_camera.getPitch();
    m_camera.setPitchAngle(pitch + 180.0f);
    m_camera.setPitchConstrained(false);
    auto view = m_camera.getViewMatrix();

    m_camera.setPitchAngle(pitch);
    m_camera.setPitchConstrained(true);
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_shader.setUniformValue("projection", m_projection);
    m_shader.setUniformValue("view", view);

    // cubes
    m_cubeVAO.bind();
    m_texture[0]->bind();
    m_model.setToIdentity();
    m_model.translate({-1.0f, 0.0f, -1.0f});
    m_shader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_model.setToIdentity();
    m_model.translate({2.0f, 0.0f, 0.0f});
    m_shader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_cubeVAO.release();

    // floor
    m_planeVAO.bind();
    m_texture[1]->bind(0);
    m_model.setToIdentity();
    m_shader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_planeVAO.release();

    // second render pass: draw as normal
    // ----------------------------------
    m_FBO->release();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // MVP
    m_shader.setUniformValue("view", m_camera.getViewMatrix());

    // cubes
    m_cubeVAO.bind();
    m_texture[0]->bind();
    m_model.setToIdentity();
    m_model.translate({-1.0f, 0.0f, -1.0f});
    m_shader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_model.setToIdentity();
    m_model.translate({2.0f, 0.0f, 0.0f});
    m_shader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    m_cubeVAO.release();

    // floor
    m_planeVAO.bind();
    m_texture[1]->bind(0);
    m_model.setToIdentity();
    m_shader.setUniformValue("model", m_model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_planeVAO.release();

    m_texture[0]->release();
    m_texture[1]->release();

    m_shader.release();
    // now draw the mirror quad with screen texture
    // --------------------------------------------
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

    m_screenShader.bind();
    m_quadVAO.bind();
    glBindTexture(GL_TEXTURE_2D, m_FBO->texture());
    m_screenShader.setUniformValue("uPostProcessingId", m_postProcessingId);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Frame_buffers_exercise1::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void Frame_buffers_exercise1::handleLineMode(bool flag)
{
    m_isLineMode = flag;
    update();
}
