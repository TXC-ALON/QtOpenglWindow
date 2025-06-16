#include "LightCastersPoint.h"

#include <QDebug>
#include <QTime>
#include "Manager.h"
LightCastersPoint::LightCastersPoint(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(1.2f, 1.0f, 2.0f),
      m_camera(this)
{
    m_timer.start(10);
    // connect(&m_timer, &QTimer::timeout, this, &LightCastersPoint::handleTimeout);
}

LightCastersPoint::~LightCastersPoint()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();
    m_lightCubeVAO.destroy();
    m_textureDiffuse->destroy();
    m_textureSpecular->destroy();
    m_textureEmission->destroy();

    doneCurrent();
}

void LightCastersPoint::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/04_Light_Casters/light_casters_point.vert");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/04_Light_Casters/light_casters_point.frag");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.link();
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }

        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/01_Basic_Lighting/light_cube.vert");
        if (!result)
        {
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/01_Basic_Lighting/light_cube.frag");
        if (!result)
        {
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.link();
        if (!result)
        {
            qDebug() << m_lightCubeShader.log();
        }
    }
    //
    {
        // 2D rect
        GLfloat vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};
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

        // light
        // 1. 绑定顶点数组对象
        m_lightVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        // 3. 设置顶点属性指针
        // 3.1 位置
        m_lightShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_lightShader.enableAttributeArray(0);
        // 3.2 法线
        m_lightShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 8);
        m_lightShader.enableAttributeArray(1);
        // 3.3 纹理
        m_lightShader.setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 6, 2, sizeof(GLfloat) * 8);
        m_lightShader.enableAttributeArray(2);

        // light cube
        m_lightCubeVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
        m_VBO.bind();
        m_lightCubeShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
        m_lightCubeShader.enableAttributeArray(0);
    }
    // texture
    {
        m_textureDiffuse = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/container2.png").mirrored());
        m_textureDiffuse->create();
        // 纹理环绕方式
        m_textureDiffuse->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_textureDiffuse->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        // 纹理过滤
        m_textureDiffuse->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        // 多级渐远纹理
        m_textureDiffuse->generateMipMaps();
        m_textureDiffuse->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

        m_textureSpecular = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/container2_specular.png").mirrored());
        m_textureSpecular->create();
        // 纹理环绕方式
        m_textureSpecular->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_textureSpecular->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        // 纹理过滤
        m_textureSpecular->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        // 多级渐远纹理
        m_textureSpecular->generateMipMaps();
        m_textureSpecular->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);

        m_textureEmission = std::make_unique<QOpenGLTexture>(QImage(":/Resources/textures/matrix.jpg").mirrored());
        m_textureEmission->create();
        // 纹理环绕方式
        m_textureEmission->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
        m_textureEmission->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
        // 纹理过滤
        m_textureEmission->setMinMagFilters(QOpenGLTexture::Nearest, QOpenGLTexture::Linear);
        // 多级渐远纹理
        m_textureEmission->generateMipMaps();
        m_textureEmission->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
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
    // 相机类初始化
    m_camera.init();
}

void LightCastersPoint::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void LightCastersPoint::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    m_lightShader.bind();
    // texture
    m_textureDiffuse->bind(0);
    m_lightShader.setUniformValue("material.diffuse", 0);
    m_textureSpecular->bind(1);
    m_lightShader.setUniformValue("material.specular", 1);
    m_textureEmission->bind(2);
    m_lightShader.setUniformValue("material.emission", 2);
    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_lightShader.setUniformValue("projection", m_projection);
    m_lightShader.setUniformValue("view", m_camera.getViewMatrix());
    m_lightShader.setUniformValue("model", QMatrix4x4());
    // pos
    m_lightShader.setUniformValue("viewPos", m_camera.getCameraPos());
    // material
    m_lightShader.setUniformValue("material.shininess", GLfloat(64.0));
    // light
    m_lightShader.setUniformValue("light.position", m_lightPos);
    m_lightShader.setUniformValue("light.ambient", QVector3D(0.2, 0.2, 0.2));
    m_lightShader.setUniformValue("light.diffuse", QVector3D(0.5, 0.5, 0.5));
    m_lightShader.setUniformValue("light.specular", QVector3D(1.0, 1.0, 1.0));
    m_lightShader.setUniformValue("light.constant", GLfloat(constant));
    m_lightShader.setUniformValue("light.linear", GLfloat(linear));
    m_lightShader.setUniformValue("light.quadratic", GLfloat(quadratic));
    // emission
    m_lightShader.setUniformValue("matrixMove", GLfloat(m_matrixMove));
    m_lightShader.setUniformValue("matrixLight", GLfloat(m_matrixLight));

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
    for (int i = 0; i < m_cubePositions.size(); ++i)
    {
        // m_models[i].rotate(1.0 , {0.5, 1.0, 0.0});
        m_lightShader.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_textureDiffuse->release();
    m_textureSpecular->release();
    m_textureEmission->release();
    m_lightShader.release();

    // lamp object
    m_lightCubeShader.bind();
    // MVP
    m_lightCubeShader.setUniformValue("projection", m_projection);
    m_lightCubeShader.setUniformValue("view", m_camera.getViewMatrix());
    QMatrix4x4 model;
    model.translate(m_lightPos);
    model.scale(0.2);
    m_lightCubeShader.setUniformValue("model", model);

    QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    m_lightCubeShader.release();
}

bool LightCastersPoint::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

QWidget *LightCastersPoint::createControlPanel()
{
    // return nullptr;
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);
    {
        QLabel *light_postion_Label = new QLabel("\n光源位置控制:", this);
        layout->addWidget(light_postion_Label);
        pPositionSpinbox = new PositionSpinbox(-10, 10, 0.1, 0.0);
        layout->addWidget(pPositionSpinbox);
        pPositionSpinbox->setValueX(m_lightPos.x());
        pPositionSpinbox->setValueY(m_lightPos.y());
        pPositionSpinbox->setValueZ(m_lightPos.z());
        connect(pPositionSpinbox, &PositionSpinbox::spinbox_valueChanged, this, &LightCastersPoint::onPositionChanged);
    }
    // 创建ComboBox
    comboBox = new QComboBox(this);
    data = prepare_data();

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        comboBox->addItem(QString::number(it.key()));
    }

    // 创建标签

    QLabel *light_attenuation_Label = new QLabel("\n光源衰减控制:", this);
    layout->addWidget(light_attenuation_Label);
    labelConstant = new QLabel("Constant: ", this);
    labelLinear = new QLabel("Linear: ", this);
    labelQuadratic = new QLabel("Quadratic: ", this);
    // 将控件添加到布局
    layout->addWidget(comboBox);
    layout->addWidget(labelConstant);
    layout->addWidget(labelLinear);
    layout->addWidget(labelQuadratic);
    connect(comboBox, &QComboBox::currentIndexChanged, this, &LightCastersPoint::onDistanceChanged);
    onDistanceChanged(0);
    comboBox->setCurrentIndex(4);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    return panel;
}

void LightCastersPoint::handleTimeout()
{
    float cur = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    m_matrixMove = cur;
    m_matrixLight = (1.0 + sin(cur)) / 2 + 0.5;
    update();
}

void LightCastersPoint::onPositionChanged()
{
    double pos_x = pPositionSpinbox->getValueX();
    double pos_y = pPositionSpinbox->getValueY();
    double pos_z = pPositionSpinbox->getValueZ();
    m_lightPos = QVector3D(pos_x, pos_y, pos_z);
    update();
}

QMap<int, LightCastersPoint::Data> LightCastersPoint::prepare_data()
{
    QMap<int, Data> data;
    data[7] = {1.0, 0.7, 1.8};
    data[13] = {1.0, 0.35, 0.44};
    data[20] = {1.0, 0.22, 0.20};
    data[32] = {1.0, 0.14, 0.07};
    data[50] = {1.0, 0.09, 0.032};
    data[65] = {1.0, 0.07, 0.017};
    data[100] = {1.0, 0.045, 0.0075};
    data[160] = {1.0, 0.027, 0.0028};
    data[200] = {1.0, 0.022, 0.0019};
    data[325] = {1.0, 0.014, 0.0007};
    data[600] = {1.0, 0.007, 0.0002};
    data[3250] = {1.0, 0.0014, 0.000007};
    return data;
}
void LightCastersPoint::onDistanceChanged(int index)
{
    qDebug() << "current index is " << index;
    QList<int> distances = {7, 13, 20, 32, 50, 65, 100, 160, 200, 325, 600, 3250};
    int distance = distances[index];
    auto point = data.value(distance);
    qDebug() << point.constant << " " << point.linear << " " << point.quadratic;

    constant = point.constant;
    linear = point.linear;
    quadratic = point.quadratic;

    labelConstant->setText(QString("Constant: %1").arg(constant));
    labelLinear->setText(QString("Linear: %1").arg(linear));
    labelQuadratic->setText(QString("Quadratic: %1").arg(quadratic));
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        update();
    }
}
