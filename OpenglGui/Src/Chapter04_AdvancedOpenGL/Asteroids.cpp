#include "Asteroids.h"

#include <QDebug>

Asteroids::Asteroids(QWidget *parent)
    : GLWidget(parent),
      m_camera(this)
{
    srand(QTime::currentTime().msecsSinceStartOfDay());
}

Asteroids::~Asteroids()
{
    makeCurrent();
    m_modelPlanet->destory();
    m_modelRock->destory();
    doneCurrent();
}

void Asteroids::initializeGL()
{
    if (!context())
    {
        qCritical() << "Cant't get OpenGL contex";
        close();
        return;
    }
    initializeOpenGLFunctions();

    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize(); // initializes in the current context, i.e. ctx
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &Asteroids::handleLoggedMessage);
    logger->startLogging();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/08_Instances/asteroids.vert",
                                                ":/Resources/shaders/04_Advanced/08_Instances/asteroids.frag");
    }
    // load model
    {
        QString executableDir = QCoreApplication::applicationDirPath();
        QString planet_Path = QDir(executableDir).filePath("Resources/objects/planet/planet.obj");
        QString rock_Path = QDir(executableDir).filePath("Resources/objects/rock/rock.obj");

        m_modelPlanet = std::make_unique<ModelUtil>(planet_Path, context());
        m_modelRock = std::make_unique<ModelUtil>(rock_Path, context());
    }
    // model matrices
    {
        int n = 5000;
        m_modelMatrices.resize(n);
        float radius = 50.0f;
        float offset = 2.5f;
        for (int i = 0; i < n; ++i)
        {
            QMatrix4x4 model;
            // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
            float angle = i * 360.0f / n;
            float displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
            float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
            float z = cos(angle) * radius + displacement;
            model.translate(x, y, z);
            // 2.缩放
            float scale = (rand() % 20) / 100.0f + 0.05;
            model.scale(scale);
            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = (rand() % 360);
            model.rotate(rotAngle, QVector3D(0.4f, 0.6f, 0.8f));
            // 4. 添加到数组中
            m_modelMatrices[i] = model;
        }
    }
    // 相机类初始化
    m_camera.init();
    m_camera.setCameraPos({0.0f, 0.0f, 55.0f});
}

void Asteroids::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Asteroids::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();
    // mvp
    // if use ```QMatrix4x4 projection;```, the projection will not be a identity
    auto projection = QMatrix4x4();
    projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1f, 100.0f);
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", m_camera.getViewMatrix());
    // draw planet
    auto model = QMatrix4x4(); // m_modelPlanet->getNDCMatrix();
    model.translate(0.0f, -3.0f, 0.0f);
    model.scale(4.0f);
    m_shader->setUniformValue("model", model);
    m_modelPlanet->Draw(m_shader.get());
    // draw meteorites
    for (auto &i : m_modelMatrices)
    {
        m_shader->setUniformValue("model", i);
        m_modelRock->Draw(m_shader.get());
    }
    m_shader->release();
}

bool Asteroids::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void Asteroids::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
