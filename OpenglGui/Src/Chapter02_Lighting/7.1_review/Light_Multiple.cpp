#include "Light_Multiple.h"

#include <QDebug>
#include <QTime>
#include "Manager.h"
#define POINT_LIGHT_NUMS 6

Light_Multiple::Light_Multiple(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_spotlightPos(1.2f, 1.0f, 2.0f),
      m_camera(this)
{
    m_pointLightColors.resize(POINT_LIGHT_NUMS);
    m_pointLightValues.resize(POINT_LIGHT_NUMS);
    m_pointLightPositions.resize(POINT_LIGHT_NUMS);

    QStringList text;
    text << "origin" << "desert" << "factory" << "horror" << "biochemical lab";
    for (int i = 0; i < 5; ++i)
    {
        m_radioButtons[i] = new QRadioButton(text[i], this);
        m_radioButtons[i]->setStyleSheet("QRadioButton {color : white;}");
        m_radioButtons[i]->setGeometry(10, 20 * i, 120, 20);
        connect(m_radioButtons[i], &QRadioButton::clicked, this, &Light_Multiple::handleRadio);
    }
    m_radioButtons[0]->setChecked(true);
    handleRadio();
}

Light_Multiple::~Light_Multiple()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();
    m_lightCubeVAO.destroy();
    m_textureDiffuse->destroy();
    m_textureSpecular->destroy();

    doneCurrent();
}

void Light_Multiple::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/06_review_lighting/multiple_lights.vert");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/06_review_lighting/multiple_lights.frag");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.link();
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }

        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/05_multiple_lights/light_cube.vert");
        if (!result)
        {
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/05_multiple_lights/light_cube.frag");
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
            {0.0f, 0.0f, 0.0f},
            {2.0f, 5.0f, -15.0f},
            {-1.5f, -2.2f, -2.5f},
            {-3.8f, -2.0f, -12.3f},
            {2.4f, -0.4f, -3.5f},
            {-1.7f, 3.0f, -7.5f},
            {1.3f, -2.0f, -2.5f},
            {1.5f, 2.0f, -2.5f},
            {1.5f, 0.2f, -1.5f},
            {-1.3f, 1.0f, -1.5f}};
        // point lights
        m_pointLightPositions = {
            {0.7f, 0.2f, 2.0f},
            {2.3f, -3.3f, -4.0f},
            {-4.0f, 2.0f, -12.0f},
            {0.0f, 0.0f, -3.0f},
            {1.0f, 1.0f, -5.0f},
            {1.0f, 1.0f, -3.0f}};

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

void Light_Multiple::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Light_Multiple::paintGL()
{
    glClearColor(m_backgroundColor.x(), m_backgroundColor.y(), m_backgroundColor.z(), m_backgroundColor.w());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    m_lightShader.bind();
    // texture
    m_textureDiffuse->bind(0);
    m_lightShader.setUniformValue("material.diffuse", 0);
    m_textureSpecular->bind(1);
    m_lightShader.setUniformValue("material.specular", 1);
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
    /*
       Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
       the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
       by defining light types as classes and set their values in there, or by using a more efficient uniform approach
       by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
    */
    // dirLight
    m_lightShader.setUniformValue("dirLight.direction", m_dirLight.direction);
    m_lightShader.setUniformValue("dirLight.ambient", m_dirLight.ambient);
    m_lightShader.setUniformValue("dirLight.diffuse", m_dirLight.diffuse);
    m_lightShader.setUniformValue("dirLight.specular", m_dirLight.specular);
    // pointLights[i]
    for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
    {
        QString pointLightStr = QString("pointLights[%1]").arg(i);
        m_lightShader.setUniformValue(QString("%1.position").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].position);
        m_lightShader.setUniformValue(QString("%1.ambient").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].ambient);
        m_lightShader.setUniformValue(QString("%1.diffuse").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].diffuse);
        m_lightShader.setUniformValue(QString("%1.specular").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].specular);
        m_lightShader.setUniformValue(QString("%1.constant").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].constant);
        m_lightShader.setUniformValue(QString("%1.linear").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].linear);
        m_lightShader.setUniformValue(QString("%1.quadratic").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i].quadratic);
    }
    // spotLight
    m_lightShader.setUniformValue("spotLight.position", m_camera.getCameraPos());

    m_spotLight.direction = m_camera.getCameraDirection();
    m_spotLight.ambient = {0.0, 0.0, 0.0};
    m_spotLight.diffuse = {1.0, 0.0, 0.0};
    m_spotLight.specular = {1.0, 0.0, 0.0};
    m_spotLight.constant = constant;
    m_spotLight.linear = linear;
    m_spotLight.quadratic = quadratic;
    m_spotLight.cutOff = cos(m_sliderCutOff * 3.1415926 / 180.0);
    m_spotLight.outerCutOff = cos(m_sliderOuterCutOff * 3.1415926 / 180.0);

    m_lightShader.setUniformValue("spotLight.direction", m_spotLight.direction);
    m_lightShader.setUniformValue("spotLight.ambient", m_spotLight.ambient);
    m_lightShader.setUniformValue("spotLight.diffuse", m_spotLight.diffuse);
    m_lightShader.setUniformValue("spotLight.specular", m_spotLight.specular);
    m_lightShader.setUniformValue("spotLight.constant", m_spotLight.constant);
    m_lightShader.setUniformValue("spotLight.linear", m_spotLight.linear);
    m_lightShader.setUniformValue("spotLight.quadratic", m_spotLight.quadratic);
    m_lightShader.setUniformValue("spotLight.cutOff", m_spotLight.cutOff);
    m_lightShader.setUniformValue("spotLight.outerCutOff", m_spotLight.outerCutOff);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
    for (int i = 0; i < m_cubePositions.size(); ++i)
    {
        m_lightShader.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_textureDiffuse->release();
    m_textureSpecular->release();
    m_lightShader.release();

    // lamp object
    m_lightCubeShader.bind();
    // MVP
    m_lightCubeShader.setUniformValue("projection", m_projection);
    m_lightCubeShader.setUniformValue("view", m_camera.getViewMatrix());
    // we now draw as many light bulbs as we have point lights.
    QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
    for (int i = 0; i < m_pointLightPositions.size(); ++i)
    {

        QMatrix4x4 model;
        if (i == 0)
        {
            model.translate(m_spotlightPos);
            model.scale(0.5);
        }
        else
        {
            model.translate(m_pointLightPositions[i]);
            model.scale(0.2);
        }

        m_lightCubeShader.setUniformValue("model", model);
        m_lightCubeShader.setUniformValue("lightColor", m_pointLightColors[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }

    m_lightCubeShader.release();
}

QWidget *Light_Multiple::createControlPanel()
{
    qDebug() << "createControlPanel() start";
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);
    {
        QLabel *light_postion_Label = new QLabel("\n聚光光源位置控制:", this);
        layout->addWidget(light_postion_Label);
        pPositionSpinbox = new PositionSpinbox(-10, 10, 0.1, 0.0);
        layout->addWidget(pPositionSpinbox);
        pPositionSpinbox->setValueX(m_spotlightPos.x());
        pPositionSpinbox->setValueY(m_spotlightPos.y());
        pPositionSpinbox->setValueZ(m_spotlightPos.z());
        connect(pPositionSpinbox, &PositionSpinbox::spinbox_valueChanged, this, &Light_Multiple::onPositionChanged);
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
    connect(comboBox, &QComboBox::currentIndexChanged, this, &Light_Multiple::onDistanceChanged);
    onDistanceChanged(0);
    comboBox->setCurrentIndex(4);

    QLabel *light_spot_Label = new QLabel("\n聚光控制:", this);
    layout->addWidget(light_spot_Label);

    m_checkBox = new QCheckBox("soft spot", this);
    layout->addWidget(m_checkBox);
    connect(m_checkBox, &QCheckBox::stateChanged, this, &Light_Multiple::onSoftButtonChanged);

    QStringList text = {"m_sliderCutOff", "m_sliderOuterCutOff"};
    std::vector<double> default_value = {m_sliderCutOff, m_sliderOuterCutOff};
    std::vector<double> steps = {1, 1};
    std::vector<std::vector<int>> range = {{1, 90}, {1, 90}}; // 正常specular_strength是0-1，不过开放一下看看效果
    for (int i = 0; i < text.size(); ++i)
    {
        light_controls.push_back(new SliderWithLabel(text[i], range[i][0], range[i][1], steps[i], default_value[i], this));

        QSize hintSize = light_controls[i]->sizeHint();
        light_controls[i]->setMinimumSize(hintSize);
        layout->addWidget(light_controls[i]);
        connect(light_controls[i], SIGNAL(slider_valueChanged()), this, SLOT(onSliderValueChanged()));
    }

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    qDebug() << "createControlPanel() end";

    return panel;
}

bool Light_Multiple::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

void Light_Multiple::handleRadio()
{
    // origin
    if (m_radioButtons[0]->isChecked())
    {
        m_backgroundColor = {0.1, 0.1, 0.1, 1.0};
        m_pointLightColors = {
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f}};

        m_dirLight.direction = {-0.2, -1.0, -0.3};
        m_dirLight.ambient = {0.05, 0.05, 0.05};
        m_dirLight.diffuse = {0.4, 0.4, 0.4};
        m_dirLight.specular = {0.5, 0.5, 0.5};

        m_spotLight.position = m_camera.getCameraPos();
        m_spotLight.direction = m_camera.getCameraDirection();
        m_spotLight.ambient = {0.2, 0.2, 0.2};
        m_spotLight.diffuse = {0.5, 0.5, 0.5};
        m_spotLight.specular = {1.0, 1.0, 1.0};
        m_spotLight.constant = 1.0;
        m_spotLight.linear = 0.09;
        m_spotLight.quadratic = 0.032;
        m_spotLight.cutOff = cos(12.5 * 3.1415926 / 180.0);
        m_spotLight.outerCutOff = cos(15.0 * 3.1415926 / 180.0);

        for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
        {
            m_pointLightValues[i].position = m_pointLightPositions[i];
            m_pointLightValues[i].ambient = {0.05, 0.05, 0.05};
            m_pointLightValues[i].diffuse = {0.8, 0.8, 0.8};
            m_pointLightValues[i].specular = {1.0, 1.0, 1.0};
            m_pointLightValues[i].constant = 1.0;
            m_pointLightValues[i].linear = 0.09;
            m_pointLightValues[i].quadratic = 0.032;
        }
    }
    // desert
    else if (m_radioButtons[1]->isChecked())
    {
        m_backgroundColor = {0.75, 0.52, 0.3, 1.0};
        m_pointLightColors = {
            {1.0f, 0.6f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.2f, 0.2f, 1.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}};

        m_dirLight.direction = {-0.2, -1.0, -0.3};
        m_dirLight.ambient = {0.3, 0.24, 0.14};
        m_dirLight.diffuse = {0.7, 0.42, 0.26};
        m_dirLight.specular = {0.5, 0.5, 0.5};

        m_spotLight.position = m_camera.getCameraPos();
        m_spotLight.direction = m_camera.getCameraDirection();
        m_spotLight.ambient = {0.0, 0.0, 0.0};
        m_spotLight.diffuse = {0.8, 0.8, 0.0};
        m_spotLight.specular = {0.8, 0.8, 0.0};
        m_spotLight.constant = 1.0;
        m_spotLight.linear = 0.09;
        m_spotLight.quadratic = 0.032;
        m_spotLight.cutOff = cos(12.5 * 3.1415926 / 180.0);
        m_spotLight.outerCutOff = cos(13.0 * 3.1415926 / 180.0);

        for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
        {
            m_pointLightValues[i].position = m_pointLightPositions[i];
            m_pointLightValues[i].ambient = m_pointLightColors[i];
            m_pointLightValues[i].diffuse = m_pointLightColors[i] * 0.1;
            m_pointLightValues[i].specular = m_pointLightColors[i];
            m_pointLightValues[i].constant = 1.0;
            m_pointLightValues[i].linear = 0.09;
            m_pointLightValues[i].quadratic = 0.032;
        }
    }
    // factory
    else if (m_radioButtons[2]->isChecked())
    {
        m_backgroundColor = {0.1, 0.1, 0.1, 1.0};
        m_pointLightColors = {
            {0.2f, 0.2f, 0.6f},
            {0.3f, 0.3f, 0.7f},
            {0.0f, 0.0f, 0.3f},
            {0.4f, 0.4f, 0.4f}};

        m_dirLight.direction = {-0.2, -1.0, -0.3};
        m_dirLight.ambient = {0.05, 0.05, 0.1};
        m_dirLight.diffuse = {0.2, 0.2, 0.7};
        m_dirLight.specular = {0.7, 0.7, 0.7};

        m_spotLight.position = m_camera.getCameraPos();
        m_spotLight.direction = m_camera.getCameraDirection();
        m_spotLight.ambient = {0.0, 0.0, 0.0};
        m_spotLight.diffuse = {1.0, 1.0, 1.0};
        m_spotLight.specular = {1.0, 1.0, 1.0};
        m_spotLight.constant = 1.0;
        m_spotLight.linear = 0.009;
        m_spotLight.quadratic = 0.0032;
        m_spotLight.cutOff = cos(10.0 * 3.1415926 / 180.0);
        m_spotLight.outerCutOff = cos(12.5 * 3.1415926 / 180.0);

        for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
        {
            m_pointLightValues[i].position = m_pointLightPositions[i];
            m_pointLightValues[i].ambient = m_pointLightColors[i];
            m_pointLightValues[i].diffuse = m_pointLightColors[i] * 0.1;
            m_pointLightValues[i].specular = m_pointLightColors[i];
            m_pointLightValues[i].constant = 1.0;
            m_pointLightValues[i].linear = 0.09;
            m_pointLightValues[i].quadratic = 0.032;
        }
    }
    // horror
    else if (m_radioButtons[3]->isChecked())
    {
        m_backgroundColor = {0.0, 0.0, 0.0, 1.0};
        m_pointLightColors = {
            {0.1f, 0.1f, 0.1f},
            {0.1f, 0.1f, 0.1f},
            {0.1f, 0.1f, 0.1f},
            {0.3f, 0.1f, 0.1f}};

        m_dirLight.direction = {-0.2, -1.0, -0.3};
        m_dirLight.ambient = {0.0, 0.0, 0.0};
        m_dirLight.diffuse = {0.05, 0.05, 0.05};
        m_dirLight.specular = {0.2, 0.2, 0.2};

        m_spotLight.position = m_camera.getCameraPos();
        m_spotLight.direction = m_camera.getCameraDirection();
        m_spotLight.ambient = {0.0, 0.0, 0.0};
        m_spotLight.diffuse = {1.0, 1.0, 1.0};
        m_spotLight.specular = {1.0, 1.0, 1.0};
        m_spotLight.constant = 1.0;
        m_spotLight.linear = 0.09;
        m_spotLight.quadratic = 0.032;
        m_spotLight.cutOff = cos(10.0 * 3.1415926 / 180.0);
        m_spotLight.outerCutOff = cos(15.0 * 3.1415926 / 180.0);

        for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
        {
            m_pointLightValues[i].position = m_pointLightPositions[i];
            m_pointLightValues[i].ambient = m_pointLightColors[i];
            m_pointLightValues[i].diffuse = m_pointLightColors[i] * 0.1;
            m_pointLightValues[i].specular = m_pointLightColors[i];
            m_pointLightValues[i].constant = 1.0;
            m_pointLightValues[i].linear = 0.14;
            m_pointLightValues[i].quadratic = 0.07;
        }
        m_pointLightValues[2].linear = 0.22;
        m_pointLightValues[2].quadratic = 0.20;
    }
    // biochemical lab
    else if (m_radioButtons[4]->isChecked())
    {
        m_backgroundColor = {0.9, 0.9, 0.9, 1.0};
        m_pointLightColors = {
            {0.4f, 0.7f, 0.1f},
            {0.4f, 0.7f, 0.1f},
            {0.4f, 0.7f, 0.1f},
            {0.4f, 0.7f, 0.1f}};

        m_dirLight.direction = {-0.2, -1.0, -0.3};
        m_dirLight.ambient = {0.5, 0.5, 0.5};
        m_dirLight.diffuse = {1.0, 1.0, 1.0};
        m_dirLight.specular = {1.0, 1.0, 1.0};

        m_spotLight.position = m_camera.getCameraPos();
        m_spotLight.direction = m_camera.getCameraDirection();
        m_spotLight.ambient = {0.0, 0.0, 0.0};
        m_spotLight.diffuse = {0.0, 1.0, 0.0};
        m_spotLight.specular = {0.0, 1.0, 0.0};
        m_spotLight.constant = 1.0;
        m_spotLight.linear = 0.07;
        m_spotLight.quadratic = 0.017;
        m_spotLight.cutOff = cos(7.0 * 3.1415926 / 180.0);
        m_spotLight.outerCutOff = cos(10.0 * 3.1415926 / 180.0);

        for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
        {
            m_pointLightValues[i].position = m_pointLightPositions[i];
            m_pointLightValues[i].ambient = m_pointLightColors[i];
            m_pointLightValues[i].diffuse = m_pointLightColors[i] * 0.1;
            m_pointLightValues[i].specular = m_pointLightColors[i];
            m_pointLightValues[i].constant = 1.0;
            m_pointLightValues[i].linear = 0.07;
            m_pointLightValues[i].quadratic = 0.017;
        }
    }

    update();
}

void Light_Multiple::set_spot_light()
{
    m_spotLight.position = m_spotlightPos;
    m_spotLight.direction = m_camera.getCameraDirection();
    m_spotLight.ambient = {0.0, 0.0, 0.0};
    m_spotLight.diffuse = {0.0, 1.0, 0.0};
    m_spotLight.specular = {0.0, 1.0, 0.0};
    m_spotLight.constant = constant;
    m_spotLight.linear = linear;
    m_spotLight.quadratic = quadratic;
    m_spotLight.cutOff = cos(7.0 * 3.1415926 / 180.0);
    m_spotLight.outerCutOff = cos(10.0 * 3.1415926 / 180.0);
}

void Light_Multiple::onPositionChanged()
{
    double pos_x = pPositionSpinbox->getValueX();
    double pos_y = pPositionSpinbox->getValueY();
    double pos_z = pPositionSpinbox->getValueZ();
    m_spotlightPos = QVector3D(pos_x, pos_y, pos_z);

    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        update();
    }
}
QMap<int, Light_Multiple::Data> Light_Multiple::prepare_data()
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
void Light_Multiple::onDistanceChanged(int index)
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
void Light_Multiple::onSliderValueChanged()
{

    m_sliderCutOff = light_controls[0]->getValue();
    m_sliderOuterCutOff = light_controls[1]->getValue();
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        update();
    }
    // qDebug() << "ambientStrength:" << ambientStrength << ",specularStrength:"
    //          << specularStrength << ",specularShininess:" << specularShininess;
}

void Light_Multiple::onSoftButtonChanged(int state)
{
    if (state == Qt::Checked)
    {
        light_controls[1]->enable();
    }
    else if (state == Qt::Unchecked)
    {
        light_controls[1]->disable();
    }
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        update();
    }
}