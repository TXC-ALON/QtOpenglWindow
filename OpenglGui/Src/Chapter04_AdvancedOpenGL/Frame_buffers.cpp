#include "Frame_buffers.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QOverload>
Frame_buffers::Frame_buffers(QWidget *parent)
    : GLWidget(parent),
      m_cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_planeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this),
      m_isLineMode(false),
      m_postProcessingId(0),
      m_buf_scale(6.0f) // 初始化像素化比例
{
    device_ratio = devicePixelRatioF();
    QCheckBox *cBoxLine = new QCheckBox("draw as wireframe", this);
    cBoxLine->setStyleSheet("QCheckBox{color:white}");
    connect(cBoxLine, &QCheckBox::clicked, this, &Frame_buffers::handleLineMode);

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
    connect(radioGroup, &QButtonGroup::buttonClicked, this, &Frame_buffers::handlePostProcessingChange);
}
QWidget *Frame_buffers::createControlPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);
    QCheckBox *cb_pixelMode = new QCheckBox("像素化", this);

    QString text = "像素化程度:";
    double default_value = {m_buf_scale};
    double steps = {1};
    std::vector<double> range = {{0, 50}};
    pixel_weight_slider = new SliderWithLabel(text, range[0], range[1], steps, default_value, this);
    QSize hintSize = pixel_weight_slider->sizeHint();
    pixel_weight_slider->setMinimumSize(hintSize);

    layout->addWidget(cb_pixelMode);
    layout->addWidget(pixel_weight_slider);
    pixel_weight_slider->setEnabled(false);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    connect(cb_pixelMode, &QCheckBox::stateChanged, this, [this, cb_pixelMode](int state)
            { this->m_isPixelMode = (state == Qt::Checked); 
            update(); 
            pixel_weight_slider->setEnabled(m_isPixelMode); });
    connect(pixel_weight_slider, SIGNAL(slider_valueChanged()), this, SLOT(onSliderValueChanged()));
    return panel;
}

void Frame_buffers::onSliderValueChanged()
{
    m_buf_scale = pixel_weight_slider->getValue();
    update();
}

void Frame_buffers::handlePostProcessingChange(QAbstractButton *button)
{
    int id = button->property("id").toInt();

    m_postProcessingId = id;

    qDebug() << "current m_postProcessingId is" << m_postProcessingId;
    update();
}
Frame_buffers::~Frame_buffers()
{
    makeCurrent();

    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void Frame_buffers::initializeGL()
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
                                  -1.0f, 1.0f, 0.0f, 1.0f,
                                  -1.0f, -1.0f, 0.0f, 0.0f,
                                  1.0f, -1.0f, 1.0f, 0.0f,

                                  -1.0f, 1.0f, 0.0f, 1.0f,
                                  1.0f, -1.0f, 1.0f, 0.0f,
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

void Frame_buffers::resizeGL(int w, int h)
{
    // qDebug() << "Frame_buffers::resizeGL w:" << w << ",h:" << h;
    qreal ratio = devicePixelRatioF();
    // qDebug() << " resizeGL ratio:" << ratio;
    //   调整视口大小，使用逻辑尺寸乘以设备像素比率得到物理像素尺寸
    glViewport(0, 0, w * device_ratio, h * device_ratio);
    m_FBO.reset(new QOpenGLFramebufferObject(w, h, QOpenGLFramebufferObject::CombinedDepthStencil));
}

void Frame_buffers::paintGL()
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
    int buf_width = static_cast<int>(std::floor(width() / m_buf_scale));
    int buf_height = static_cast<int>(std::floor(height() / m_buf_scale));
    // 保存原始视口大小
    if (m_isPixelMode)
    {
        // 绑定颜色纹理
        glBindTexture(GL_TEXTURE_2D, m_FBO->texture());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, buf_width, buf_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // 缩小视口
        glViewport(0, 0, buf_width * device_ratio, buf_height * device_ratio);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_FBO->texture());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glViewport(0, 0, width() * device_ratio, height() * device_ratio);
    }
    // glViewport(0, 0, width( ), height( ));

    // 绑定帧缓冲
    m_FBO->bind();
    glEnable(GL_DEPTH_TEST);

    // 清除帧缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.bind();

    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    // m_projection.perspective(m_camera.getFov(), double(buf_width) / buf_height, 0.1, 100.0); // 比例是一样的
    m_shader.setUniformValue("projection", m_projection);
    m_shader.setUniformValue("view", m_camera.getViewMatrix());

    // cubes
    m_texture[0]->bind();
    m_model.setToIdentity();
    m_model.translate({-1.0f, 0.0f, -1.0f});
    m_shader.setUniformValue("model", m_model);
    m_cubeVAO.bind();
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

    glViewport(0, 0, device_ratio * width(), device_ratio * height());
    // 释放帧缓冲
    m_FBO->release();
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    m_screenShader.bind();
    m_quadVAO.bind();
    glBindTexture(GL_TEXTURE_2D, m_FBO->texture());
    m_screenShader.setUniformValue("uPostProcessingId", m_postProcessingId);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Frame_buffers::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void Frame_buffers::handleLineMode(bool flag)
{
    m_isLineMode = flag;
    update();
}
