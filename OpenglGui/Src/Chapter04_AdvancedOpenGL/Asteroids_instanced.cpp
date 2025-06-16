#include "Asteroids_instanced.h"
#include "Manager.h"

Asteroids_instanced::Asteroids_instanced(QWidget *parent)
    : GLWidget(parent),
      m_camera(this)
{
    srand(QTime::currentTime().msecsSinceStartOfDay());
}

Asteroids_instanced::~Asteroids_instanced()
{
    makeCurrent();
    m_modelPlanet->destory();
    m_modelRock->destory();
    doneCurrent();
}

void Asteroids_instanced::initializeGL()
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
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &Asteroids_instanced::handleLoggedMessage);
    logger->startLogging();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // shader初始化
    {
        m_shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/08_Instances/planet.vert",
                                                ":/Resources/shaders/04_Advanced/08_Instances/planet.frag");
        m_rockShader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/08_Instances/asteroids_instanced.vert",
                                                    ":/Resources/shaders/04_Advanced/08_Instances/asteroids_instanced.frag");
    }
    // load model
    {
        QString executableDir = QCoreApplication::applicationDirPath();
        QString planet_Path = QDir(executableDir).filePath("Resources/objects/planet/planet.obj");
        QString rock_Path = QDir(executableDir).filePath("Resources/objects/rock/rock.obj");

        m_modelPlanet = std::make_unique<ModelUtil>(planet_Path, context());
        m_modelRock = std::make_unique<ModelUtil>(rock_Path, context());
    }
    updateModelMatrices(planet_num, radius, offset);
    // 相机类初始化
    m_camera.init();
    m_camera.setCameraPos({0.0f, 0.0f, 155.0f});
}

void Asteroids_instanced::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Asteroids_instanced::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = QMatrix4x4();
    projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1f, 1000.0f);
    auto view = m_camera.getViewMatrix();

    m_shader->bind();
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", view);
    // draw planet
    auto model = QMatrix4x4(); // m_modelPlanet->getNDCMatrix();
    model.translate(0.0f, -3.0f, 0.0f);
    model.scale(4.0f);
    m_shader->setUniformValue("model", model);
    m_modelPlanet->Draw(m_shader.get());

    // draw meteorites
    m_rockShader->bind();
    m_rockShader->setUniformValue("projection", projection);
    m_rockShader->setUniformValue("view", view);
    m_rockShader->setUniformValue("texture_diffuse1", 0);
    m_modelRock->texturesLoaded[0]->texture.bind();
    for (auto &mesh : m_modelRock->meshes)
    {
        auto VAO = mesh->getVAO();
        VAO->bind();
        glDrawElementsInstanced(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0, m_modelMatrices.size());
        VAO->release();
    }
    m_rockShader->release();
}

bool Asteroids_instanced::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

QWidget *Asteroids_instanced::createControlPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    QStringList text;
    text << "planet_num"
         << "radius"
         << "offset";
    std::vector<float> default_value = {100000.0, radius, offset};
    std::vector<std::vector<int>> range = {{1, 1000000}, {1, 10000}, {1, 100}};
    for (int i = 0; i < text.size(); ++i)
    {
        m_sliders.push_back(new SliderWithLabel(text[i], range[i][0], range[i][1], 1, default_value[i], this));
        layout->addWidget(m_sliders[i]);
        connect(m_sliders[i], SIGNAL(slider_valueChanged()), this, SLOT(onSliderValueChanged()));
    }
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    return panel;
}
void Asteroids_instanced::onSliderValueChanged()
{
    planet_num = m_sliders[0]->getValue();
    radius = m_sliders[1]->getValue();
    offset = m_sliders[2]->getValue();
    updateModelMatrices(planet_num, radius, offset);
    update();
}
void Asteroids_instanced::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}

void Asteroids_instanced::updateModelMatrices(int planet_num, float _radius, float _offset)
{
    // model matrices
    {
        int n = planet_num;
        m_modelMatrices.resize(n);

        float radius = _radius;
        float offset = _offset;
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
            memcpy(&m_modelMatrices[i], model.data(), sizeof(mat4));
        }
        m_VBO.create();
        m_VBO.bind();
        // 注意如果使用 QMatrix4x4, 大小和间隔都要重新计算, 并且 sizeof(QMatrix4x4) = 68, 多了个sizeof(flagBits), 计算起来麻烦
        m_VBO.allocate(m_modelMatrices.data(), n * sizeof(mat4));

        int vec4Size = 4 * sizeof(float);
        for (auto &mesh : m_modelRock->meshes)
        {
            auto VAO = mesh->getVAO();
            VAO->bind();
            // 顶点属性, 顶点属性最大允许的数据大小等于一个vec4， 所以一个mat4要用4个vec4
            m_rockShader->enableAttributeArray(3);
            m_rockShader->setAttributeBuffer(3, GL_FLOAT, 0 * vec4Size, 4, 4 * vec4Size);
            m_rockShader->enableAttributeArray(4);
            m_rockShader->setAttributeBuffer(4, GL_FLOAT, 1 * vec4Size, 4, 4 * vec4Size);
            m_rockShader->enableAttributeArray(5);
            m_rockShader->setAttributeBuffer(5, GL_FLOAT, 2 * vec4Size, 4, 4 * vec4Size);
            m_rockShader->enableAttributeArray(6);
            m_rockShader->setAttributeBuffer(6, GL_FLOAT, 3 * vec4Size, 4, 4 * vec4Size);
            // 渲染一个新实例的时候更新顶点属性
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            VAO->release();
        }
    }
}
