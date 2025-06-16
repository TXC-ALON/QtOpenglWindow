#include "Basic_Colors.h"

#include <QDebug>
#include <QTime>
#include "ColorButton.h"
Basic_Colors::Basic_Colors(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(0, 0, 1.0),
      m_camera(this)
{
    QStringList text;
    text << "yaw"
         << "pitch"
         << "camera pos";
    for (int i = 0; i < text.size(); ++i)
    {
        QLabel *label = new QLabel(text[i], this);
        label->setStyleSheet("QLabel { color : white; }");
        label->setGeometry(10, 20 * i, 70, 20);
    }
    m_labelYaw = new QLabel(this);
    m_labelYaw->setStyleSheet("QLabel { color : white; }");
    m_labelYaw->setGeometry(90, 0, 100, 20);

    m_labelPitch = new QLabel(this);
    m_labelPitch->setStyleSheet("QLabel { color : white; }");
    m_labelPitch->setGeometry(90, 20, 100, 20);

    m_labelCameraPos = new QLabel(this);
    m_labelCameraPos->setStyleSheet("QLabel { color : white; }");
    m_labelCameraPos->setGeometry(90, 40, 100, 20);
}

Basic_Colors::~Basic_Colors()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();
    m_lightCubeVAO.destroy();
    doneCurrent();
}

void Basic_Colors::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/06_review_lighting/colors.vert");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/06_review_lighting/colors.frag");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.link();
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }

        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/06_review_lighting/light_cube.vert");
        if (!result)
        {
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/06_review_lighting/light_cube.frag");
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
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

        // light
        // 1. 绑定顶点数组对象
        m_lightVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        // 3. 设置顶点属性指针
        m_lightShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_lightShader.enableAttributeArray(0);
        m_lightShader.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
        m_lightShader.enableAttributeArray(1);

        // light cube
        m_lightCubeVAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
        m_VBO.bind();
        m_lightCubeShader.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_lightCubeShader.enableAttributeArray(0);
    }
    // MVP
    {
        m_projection.perspective(45.0, 1.0 * width() / height(), 0.1, 100.0);
    }
    // 相机类初始化
    m_camera.init();
    m_camera.setCameraPos({1.4, 0.25, 2.75});
    // m_camera.setCameraPos({0.0, 0.0, 10.0});
    m_camera.setYawAngle(-120);
    m_camera.setPitchAngle(-0.03);
}

void Basic_Colors::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Basic_Colors::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // light
    m_lightShader.bind();
    // MVP
    m_projection.setToIdentity();
    m_projection.perspective(m_camera.getFov(), 1.0 * width() / height(), 0.1, 100.0);
    m_lightShader.setUniformValue("projection", m_projection);
    m_lightShader.setUniformValue("view", m_camera.getViewMatrix());
    m_lightShader.setUniformValue("model", QMatrix4x4());
    // color

    m_lightShader.setUniformValue("objectColor", modelcolor->getColor());
    m_lightShader.setUniformValue("lightColor", lightcolor->getColor());

    // pos
    m_lightShader.setUniformValue("lightPos", m_lightPos);
    m_lightShader.setUniformValue("viewPos", m_camera.getCameraPos());
    // strength

    m_lightShader.setUniformValue("ambientStrength", (GLfloat)ambientStrength);
    m_lightShader.setUniformValue("specularStrength", (GLfloat)specularStrength);
    m_lightShader.setUniformValue("specularShininess", (GLfloat)specularShininess);

    m_lightShader.setUniformValue("useAmbient", useAmbient);   // 开启环境光
    m_lightShader.setUniformValue("useDiffuse", useDiffuse);   // 关闭漫反射光
    m_lightShader.setUniformValue("useSpecular", useSpecular); // 开启镜面光

    QVector3D modelColor = modelcolor->getColor();
    QVector3D lightColor = lightcolor->getColor();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    m_lightShader.release();

    // lamp object
    m_lightCubeShader.bind();
    // MVP
    m_lightCubeShader.setUniformValue("projection", m_projection);
    m_lightCubeShader.setUniformValue("view", m_camera.getViewMatrix());
    m_lightCubeShader.setUniformValue("lightbaseColor", lightcolor->getColor());

    QMatrix4x4 model;
    model.translate(m_lightPos);
    model.scale(0.2);
    m_lightCubeShader.setUniformValue("model", model);

    QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

    m_lightCubeShader.release();
    // show angle
    m_labelYaw->setText(QString::number(m_camera.getYaw()));
    m_labelPitch->setText(QString::number(m_camera.getPitch()));
    QVector3D CameraPos = m_camera.getCameraPos();
    QString cameraPosText = QString("%1,%2,%3")
                                .arg(QString::number(CameraPos.x(), 'f', 2))
                                .arg(QString::number(CameraPos.y(), 'f', 2))
                                .arg(QString::number(CameraPos.z(), 'f', 2));
    m_labelCameraPos->setText(cameraPosText);
}

bool Basic_Colors::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

QWidget *Basic_Colors::createControlPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    {
        lightcolor = new ColorButton("光源光照", QVector3D(255, 255, 255));
        modelcolor = new ColorButton("物体光照", QVector3D(255, 127.5, 76.5));

        layout->addWidget(lightcolor);
        layout->addWidget(modelcolor);
    }
    {
        QLabel *Light_Label = new QLabel("\n光照模式选择:", this);
        layout->addWidget(Light_Label);
        QHBoxLayout *hlayout = new QHBoxLayout();
        checkBoxAmbient = new QCheckBox("Ambient", this);
        checkBoxDiffuse = new QCheckBox("Diffuse", this);
        checkBoxSpecular = new QCheckBox("Specular", this);
        checkBoxAmbient->setChecked(useAmbient);   // 默认启用环境光
        checkBoxDiffuse->setChecked(useDiffuse);   // 默认启用漫反射光
        checkBoxSpecular->setChecked(useSpecular); // 默认启用镜面光
        // QSpacerItem *fixSpacer1 = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        // layout->addItem(fixSpacer1);
        hlayout->addWidget(checkBoxAmbient);
        hlayout->addWidget(checkBoxDiffuse);
        hlayout->addWidget(checkBoxSpecular);
        layout->addLayout(hlayout);
        // 将复选框的stateChanged信号连接到槽函数
        connect(checkBoxAmbient, &QCheckBox::stateChanged, this, &Basic_Colors::onCheckBoxChanged);
        connect(checkBoxDiffuse, &QCheckBox::stateChanged, this, &Basic_Colors::onCheckBoxChanged);
        connect(checkBoxSpecular, &QCheckBox::stateChanged, this, &Basic_Colors::onCheckBoxChanged);
    }

    {
        QLabel *Light_control_Label = new QLabel("\n光照参数控制:", this);
        layout->addWidget(Light_control_Label);
        QStringList text = {"ambient_strength", "specular_strength", "specular_shininess"};
        std::vector<float> default_value = {0.1, 1, 32};
        std::vector<float> steps = {0.001, 0.001, 1};
        std::vector<std::vector<int>> range = {{0, 1}, {0, 10}, {1, 1000}}; // 正常specular_strength是0-1，不过开放一下看看效果
        for (int i = 0; i < text.size(); ++i)
        {
            light_controls.push_back(new SliderWithLabel(text[i], range[i][0], range[i][1], steps[i], default_value[i], this));

            QSize hintSize = light_controls[i]->sizeHint();
            light_controls[i]->setMinimumSize(hintSize);
            layout->addWidget(light_controls[i]);
            connect(light_controls[i], SIGNAL(slider_valueChanged()), this, SLOT(onSliderValueChanged()));
        }
    }

    {
        QLabel *llight_postion_Label = new QLabel("\n光源位置控制:", this);
        layout->addWidget(llight_postion_Label);
        pPositionSpinbox = new PositionSpinbox(-10, 10, 0.1, 0.0);
        layout->addWidget(pPositionSpinbox);
        pPositionSpinbox->setValueX(m_lightPos.x());
        pPositionSpinbox->setValueY(m_lightPos.y());
        pPositionSpinbox->setValueZ(m_lightPos.z());
        connect(pPositionSpinbox, &PositionSpinbox::spinbox_valueChanged, this, &Basic_Colors::onPositionChanged);
    }

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    return panel;
}

void Basic_Colors::onCheckBoxChanged()
{
    useAmbient = checkBoxAmbient->isChecked();
    useDiffuse = checkBoxDiffuse->isChecked();
    useSpecular = checkBoxSpecular->isChecked();
    // qDebug() << "Ambient: " << useAmbient << ", Diffuse: " << useDiffuse << ", Specular: " << useSpecular;
    update();
}

void Basic_Colors::onPositionChanged()
{
    double pos_x = pPositionSpinbox->getValueX();
    double pos_y = pPositionSpinbox->getValueY();
    double pos_z = pPositionSpinbox->getValueZ();
    m_lightPos = QVector3D(pos_x, pos_y, pos_z);
    update();
}

void Basic_Colors::onSliderValueChanged()
{

    ambientStrength = light_controls[0]->getValue();
    specularStrength = light_controls[1]->getValue();
    specularShininess = light_controls[2]->getValue();
    // qDebug() << "ambientStrength:" << ambientStrength << ",specularStrength:"
    //          << specularStrength << ",specularShininess:" << specularShininess;
    update();
}
