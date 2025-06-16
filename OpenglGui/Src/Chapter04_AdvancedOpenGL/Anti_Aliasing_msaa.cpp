
#include "Anti_Aliasing_msaa.h"

Anti_Aliasing_msaa::Anti_Aliasing_msaa(QWidget *parent)
    : GLWidget(parent),
      cubeVBO(QOpenGLBuffer::VertexBuffer),
      m_camera(this)
{
    QSurfaceFormat format = this->format();
    qDebug() << "Samples:" << format.samples();
}
Anti_Aliasing_msaa::~Anti_Aliasing_msaa()
{
    makeCurrent();
    cubeVBO.destroy();
    cubeVAO.destroy();
    doneCurrent();
}

void Anti_Aliasing_msaa::initializeGL()
{
    if (!context())
    {
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/09_Anti_aliasing/anti_aliasing.vert",
                                                ":/Resources/shaders/04_Advanced/09_Anti_aliasing/anti_aliasing.frag");
    }

    {
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

        cubeVAO.create();
        cubeVAO.bind();
        cubeVBO.create();
        cubeVBO.bind();
        cubeVBO.allocate(cubeVertices, sizeof(cubeVertices));
        m_shader->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GL_FLOAT) * 3);
        m_shader->enableAttributeArray(0);
        cubeVAO.release();
    }
    // 相机类初始化
    m_camera.init();
}

void Anti_Aliasing_msaa::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Anti_Aliasing_msaa::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw scene as normal
    m_shader->bind();
    // MVP
    m_model.setToIdentity();
    m_model.rotate(-45, QVector3D(1, 1, 0));
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_shader->setUniformValue("projection", m_projection);
    m_shader->setUniformValue("view", m_camera.getViewMatrix());
    m_shader->setUniformValue("model", m_model);

    // cubes
    cubeVAO.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    cubeVAO.release();
}

bool Anti_Aliasing_msaa::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}
