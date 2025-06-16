#include "Geometry_Shader_normals.h"

Geometry_Shader_normals::Geometry_Shader_normals(QWidget *parent)
    : GLWidget(parent),
      m_camera(this)
{
}

Geometry_Shader_normals::~Geometry_Shader_normals()
{
    makeCurrent();
    m_model->destory();
    doneCurrent();
}

void Geometry_Shader_normals::initializeGL()
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
    connect(logger, &QOpenGLDebugLogger::messageLogged, this, &Geometry_Shader_normals::handleLoggedMessage);
    logger->startLogging();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // shader初始化
    {
        m_normalShader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/07_Geometry_shader/normal_visualization.vert",
                                                      ":/Resources/shaders/04_Advanced/07_Geometry_shader/normal_visualization.frag",
                                                      ":/Resources/shaders/04_Advanced/07_Geometry_shader/normal_visualization.geom");

        m_middle_normal_Shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/07_Geometry_shader/middle_normal_visualization.vert",
                                                              ":/Resources/shaders/04_Advanced/07_Geometry_shader/middle_normal_visualization.frag",
                                                              ":/Resources/shaders/04_Advanced/07_Geometry_shader/middle_normal_visualization.geom");

        m_wire_Shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/07_Geometry_shader/wire_visualization.vert",
                                                     ":/Resources/shaders/04_Advanced/07_Geometry_shader/wire_visualization.frag",
                                                     ":/Resources/shaders/04_Advanced/07_Geometry_shader/wire_visualization.geom");

        m_shader = std::make_unique<ShaderUtil>(":/Resources/shaders/04_Advanced/07_Geometry_shader/default.vert",
                                                ":/Resources/shaders/04_Advanced/07_Geometry_shader/default.frag");
    }
    // load model
    QString executableDir = QCoreApplication::applicationDirPath();
    QString modelPath = QDir(executableDir).filePath("Resources/objects/nanosuit/nanosuit.obj");
    m_model = std::make_unique<ModelUtil>(modelPath, context());
    // 相机类初始化
    m_camera.init();
}

void Geometry_Shader_normals::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Geometry_Shader_normals::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->bind();
    // mvp
    auto projection = QMatrix4x4();
    projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    auto view = m_camera.getViewMatrix();
    auto model = QMatrix4x4();
    // model.scale(0.1);
    m_shader->setUniformValue("projection", projection);
    m_shader->setUniformValue("view", view);
    m_shader->setUniformValue("model", model);
    // draw model as usual
    m_model->Draw(m_shader.get());

    // then draw model with normal visualizing geometry shader
    if (m_normal)
    {
        m_normalShader->bind();
        m_normalShader->setUniformValue("projection", projection);
        m_normalShader->setUniformValue("view", view);
        m_normalShader->setUniformValue("model", model);
        m_model->Draw(m_normalShader.get());
        m_normalShader->release();
    }

    if (m_middle_normal)
    {
        m_middle_normal_Shader->bind();
        m_middle_normal_Shader->setUniformValue("projection", projection);
        m_middle_normal_Shader->setUniformValue("view", view);
        m_middle_normal_Shader->setUniformValue("model", model);
        m_model->Draw(m_middle_normal_Shader.get());
        m_middle_normal_Shader->release();
    }

    if (m_wire)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        m_wire_Shader->bind();
        m_wire_Shader->setUniformValue("projection", projection);
        m_wire_Shader->setUniformValue("view", view);
        m_wire_Shader->setUniformValue("model", model);
        m_wire_Shader->setUniformValue("radio", m_Radio);
        m_model->Draw(m_wire_Shader.get());
        m_wire_Shader->release();
    }
}

bool Geometry_Shader_normals::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

QWidget *Geometry_Shader_normals::createControlPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);
    QCheckBox *cb_normal = new QCheckBox("顶点法向显示", this);
    cb_normal->setChecked(true); // 设置为选中状态
    QCheckBox *cb_mid_normal = new QCheckBox("面片法向显示", this);
    QCheckBox *cb_wire = new QCheckBox("线框显示", this);

    QString text = "线框偏移度:";
    double default_value = {m_Radio};
    double step = 0.0001;
    std::vector<double> range = {{0, 1}};
    radio_slider = new SliderWithLabel(text, range[0], range[1], step, default_value, this, -1, 5);
    QSize hintSize = radio_slider->sizeHint();
    radio_slider->setMinimumSize(hintSize);

    layout->addWidget(cb_normal);
    layout->addWidget(cb_mid_normal);
    layout->addWidget(cb_wire);
    layout->addWidget(radio_slider);
    radio_slider->setEnabled(false);

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    connect(cb_normal, &QCheckBox::stateChanged, this, [this, cb_normal](int state)
            { this->m_normal = (state == Qt::Checked);
            update(); });
    connect(cb_mid_normal, &QCheckBox::stateChanged, this, [this, cb_mid_normal](int state)
            { this->m_middle_normal = (state == Qt::Checked);
            update(); });
    connect(cb_wire, &QCheckBox::stateChanged, this, [this, cb_wire](int state)
            { this->m_wire = (state == Qt::Checked);
            update();
            radio_slider->setEnabled(this->m_wire); });
    connect(radio_slider, SIGNAL(slider_valueChanged()), this, SLOT(onSliderValueChanged()));

    return panel;
}
void Geometry_Shader_normals::onSliderValueChanged()
{
    m_Radio = radio_slider->getValue();
    update();
}
void Geometry_Shader_normals::handleLoggedMessage(const QOpenGLDebugMessage &debugMessage)
{
    qDebug() << debugMessage.message();
}
