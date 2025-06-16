#include "CoordinateSystemsExercise.h"

#include <QTime>
#include <QLabel>

CoordinateSystemsExercise::CoordinateSystemsExercise(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_fov(45.0),
      m_aspectRatio(1.0),
      m_nearPlane(1.0),
      m_farPlane(100.0)
{

    m_timer.start(100);
    connect(&m_timer, &QTimer::timeout, this, &CoordinateSystemsExercise::handleTimeout);
}

CoordinateSystemsExercise::~CoordinateSystemsExercise()
{
    makeCurrent();

    m_VBO.destroy();
    m_VAO.destroy();
    m_texture[0]->destroy();
    m_texture[1]->destroy();

    doneCurrent();
}

void CoordinateSystemsExercise::initializeGL()
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
            {0.0, 0.0, 0.0},
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
        m_view.translate({0.0, 0.0, -3.0});
        m_projection.perspective(45.0, 1.0, 0.1, 100.0);
    }
}

void CoordinateSystemsExercise::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void CoordinateSystemsExercise::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.bind();
    m_texture[0]->bind(0);
    m_program.setUniformValue("texture1", 0);
    m_texture[1]->bind(1);
    m_program.setUniformValue("texture2", 1);
    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
    m_program.setUniformValue("view", m_view);
    m_program.setUniformValue("projection", m_projection);
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    for (int i = 0; i < m_cubePositions.size(); ++i)
    {
        m_models[i].rotate(10.0, {0.5, 1.0, 0.0});
        m_program.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_texture[0]->release();
    m_texture[1]->release();
    m_program.release();
}

void CoordinateSystemsExercise::mousePressEvent(QMouseEvent *event)
{
}

void CoordinateSystemsExercise::mouseMoveEvent(QMouseEvent *event)
{
}

void CoordinateSystemsExercise::wheelEvent(QWheelEvent *event)
{
    auto degree = event->angleDelta();
    double factor = degree.y() / 360.0;

    factor += 1.0;
    qDebug() << "wheel degree : " << degree.y() << ", "
             << "scale factor" << factor;

    m_view.scale(factor);
    update();
}

void CoordinateSystemsExercise::handleTimeout()
{
    update();
}

QWidget *CoordinateSystemsExercise::createControlPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setSpacing(5); //
    QStringList text;
    text << "fov"
         << "aspect ratio"
         << "near plane"
         << "far plane"
         << "view x"
         << "view y"
         << "view z";
    std::vector<float> value = {m_fov, m_aspectRatio, m_nearPlane, m_farPlane, 0.0, 0.0, -3.0};
    std::vector<std::vector<int>> range = {{1, 360}, {1, 10}, {1, 100}, {1, 100}, {-10, 10}, {-10, 10}, {-10, 10}};
    for (int i = 0; i < text.size(); ++i)
    {
        m_sliders.push_back(new SliderWithLabel(text[i], range[i][0], range[i][1], 0.1, value[i], this));
        layout->addWidget(m_sliders[i]);
        connect(m_sliders[i], SIGNAL(slider_valueChanged()), this, SLOT(onSliderValueChanged()));
    }

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    return panel;
}
void CoordinateSystemsExercise::updateLabel(double value, QLabel *label)
{
    // 更新标签显示滑块的当前值
    label->setText(QString::number(value));
}
void CoordinateSystemsExercise::handleMatrix()
{
    m_fov = m_sliders[0]->getValue();
    m_aspectRatio = m_sliders[1]->getValue();
    m_nearPlane = m_sliders[2]->getValue();
    m_farPlane = m_sliders[3]->getValue();
    qDebug() << "m_fov changed to:" << m_fov;
    qDebug() << "m_aspectRatio changed to:" << m_aspectRatio;
    qDebug() << "m_nearPlane changed to:" << m_nearPlane;
    qDebug() << "m_farPlane changed to:" << m_farPlane;
    m_view.setToIdentity();
    m_view.translate(m_sliders[4]->getValue(), m_sliders[5]->getValue(), m_sliders[6]->getValue());
}

void CoordinateSystemsExercise::onSliderValueChanged()
{
    // // 这里我们可以通过 sender() 来确定是哪个滑块触发了信号
    // SliderWithLabel *slider = static_cast<SliderWithLabel *>(sender());
    // double svalue = slider->getValue();
    // qDebug() << "Slider value changed to:" << svalue;
    handleMatrix();
}
