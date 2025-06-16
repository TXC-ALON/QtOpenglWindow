#include "Light_Materials.h"

#include <QDebug>
#include <QTime>
#include "ColorButton.h"
Light_Materials::Light_Materials(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(0, 0, 1.0),
      m_camera(this)
{

    set_camera_labels();
    InitializeMaterials();
}

Light_Materials::~Light_Materials()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();
    m_lightCubeVAO.destroy();

    doneCurrent();
}

void Light_Materials::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/02_Lighting/06_review_lighting/materials.vert");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/02_Lighting/06_review_lighting/materials.frag");
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

void Light_Materials::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Light_Materials::paintGL()
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

    // light
    m_lightShader.setUniformValue("light.position", m_lightPos);
    m_lightShader.setUniformValue("light.ambient", lightcolor->getColor() * 0.2);
    m_lightShader.setUniformValue("light.diffuse", lightcolor->getColor() * 0.5);
    m_lightShader.setUniformValue("light.specular", lightcolor->getColor());

    m_lightShader.setUniformValue("material.ambient", current_material.ambient);
    m_lightShader.setUniformValue("material.diffuse", current_material.diffuse);
    m_lightShader.setUniformValue("material.specular", current_material.specular);
    m_lightShader.setUniformValue("material.shininess", current_material.shininess);

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

bool Light_Materials::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

QWidget *Light_Materials::createControlPanel()
{
    QWidget *panel = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    {
        lightcolor = new ColorButton("光源光照", QVector3D(255, 255, 255));

        modelcolor = new ColorButton("物体光照");
        QColor model_color = QColor::fromRgbF(current_material.diffuse.x(), current_material.diffuse.y(), current_material.diffuse.z());
        modelcolor->setColor(model_color);
        modelcolor->setEnabled(false);

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
        connect(checkBoxAmbient, &QCheckBox::stateChanged, this, &Light_Materials::onCheckBoxChanged);
        connect(checkBoxDiffuse, &QCheckBox::stateChanged, this, &Light_Materials::onCheckBoxChanged);
        connect(checkBoxSpecular, &QCheckBox::stateChanged, this, &Light_Materials::onCheckBoxChanged);
    }

    {
        QLabel *Light_control_Label = new QLabel("\n物体材质:", this);
        layout->addWidget(Light_control_Label);
        QComboBox *materialComboBox = new QComboBox(this);
        layout->addWidget(materialComboBox);
        for (int i = 0; i < materials.size(); i++)
        {
            materialComboBox->addItem(materials[i].name);
        }
        connect(materialComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaterial(int)));
    }
    {
        QLabel *light_postion_Label = new QLabel("\n光源位置控制:", this);
        layout->addWidget(light_postion_Label);
        pPositionSpinbox = new PositionSpinbox(-10, 10, 0.1, 0.0);
        layout->addWidget(pPositionSpinbox);
        pPositionSpinbox->setValueX(m_lightPos.x());
        pPositionSpinbox->setValueY(m_lightPos.y());
        pPositionSpinbox->setValueZ(m_lightPos.z());
        connect(pPositionSpinbox, &PositionSpinbox::spinbox_valueChanged, this, &Light_Materials::onPositionChanged);
    }

    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    return panel;
}

void Light_Materials::set_camera_labels()
{
    QStringList text = {"yaw", "pitch", "camera pos"};
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

void Light_Materials::InitializeMaterials()
{
    // 金材质
    Material goldMaterial = Material(
        "金",
        QVector3D(0.25, 0.20725, 0.0),           // 环境光
        QVector3D(0.75164, 0.60648, 0.22648),    // 漫反射
        QVector3D(0.628281, 0.555802, 0.366065), // 镜面反射
        51.2                                     // 高光指数
    );

    // 银材质
    Material silverMaterial = Material(
        "银",
        QVector3D(0.19250, 0.19250, 0.19250),    // 环境光
        QVector3D(0.507540, 0.507540, 0.507540), // 漫反射
        QVector3D(0.508273, 0.508273, 0.508273), // 镜面反射
        51.2                                     // 高光指数
    );

    // 铜材质
    Material copperMaterial = Material(
        "铜",
        QVector3D(0.19375, 0.0675, 0.013125),    // 环境光
        QVector3D(0.7038, 0.27048, 0.0828),      // 漫反射
        QVector3D(0.256777, 0.137622, 0.086014), // 镜面反射
        18.4                                     // 高光指数
    );

    // 青铜材质
    Material bronzeMaterial = Material(
        "青铜",
        QVector3D(0.2125, 0.1275, 0.054),        // 环境光
        QVector3D(0.714, 0.4284, 0.18144),       // 漫反射
        QVector3D(0.393548, 0.271906, 0.166721), // 镜面反射
        25.6                                     // 高光指数
    );

    // 黄铜材质
    Material brassMaterial = Material(
        "黄铜",
        QVector3D(0.33, 0.22, 0.03), // 环境光
        QVector3D(0.78, 0.57, 0.11), // 漫反射
        QVector3D(0.99, 0.91, 0.81), // 镜面反射
        27.8                         // 高光指数
    );

    // 锡合金材质
    Material pewterMaterial = Material(
        "锡合金",
        QVector3D(0.105882, 0.058824, 0.113725), // 环境光
        QVector3D(0.427451, 0.470588, 0.541176), // 漫反射
        QVector3D(0.333333, 0.333333, 0.521569), // 镜面反射
        50.0                                     // 高光指数
    );

    // 锌材质
    Material zincMaterial = Material(
        "锌",
        QVector3D(0.0787, 0.08, 0.0833), // 环境光
        QVector3D(0.618, 0.647, 0.668),  // 漫反射
        QVector3D(0.628, 0.628, 0.668),  // 镜面反射
        51.2                             // 高光指数
    );

    // 铬材质
    Material chromeMaterial = Material(
        "铬",
        QVector3D(0.25, 0.25, 0.25),             // 环境光
        QVector3D(0.1, 0.1, 0.1),                // 漫反射
        QVector3D(0.774597, 0.774597, 0.774597), // 镜面反射
        76.8                                     // 高光指数
    );

    // 镍材质
    Material nickelMaterial = Material(
        "镍",
        QVector3D(0.135, 0.2225, 0.1575), // 环境光
        QVector3D(0.663, 0.637, 0.546),   // 漫反射
        QVector3D(0.632, 0.622, 0.549),   // 镜面反射
        50.0                              // 高光指数
    );

    // 钛材质
    Material titaniumMaterial = Material(
        "钛",
        QVector3D(0.4, 0.4, 0.3), // 环境光
        QVector3D(0.5, 0.4, 0.3), // 漫反射
        QVector3D(0.8, 0.6, 0.4), // 镜面反射
        51.2                      // 高光指数
    );

    // 翡翠材质
    Material emeraldMaterial = Material(
        "翡翠",
        QVector3D(0.0215, 0.1745, 0.0215),    // 环境光
        QVector3D(0.07568, 0.61424, 0.07568), // 漫反射
        QVector3D(0.633, 0.727811, 0.633),    // 镜面反射
        76.8                                  // 高光指数
    );

    // 玉石材质
    Material jadeMaterial = Material(
        "玉石",
        QVector3D(0.135, 0.2225, 0.1575),   // 环境光
        QVector3D(0.5143, 0.58039, 0.5143), // 漫反射
        QVector3D(0.396, 0.471, 0.396),     // 镜面反射
        50.0                                // 高光指数
    );

    // 红宝石材质
    Material rubyMaterial = Material(
        "红宝石",
        QVector3D(0.0246, 0.0, 0.0),             // 环境光
        QVector3D(0.61424, 0.04136, 0.04136),    // 漫反射
        QVector3D(0.727811, 0.626959, 0.626959), // 镜面反射
        76.8                                     // 高光指数
    );

    // 钻石材质
    Material diamondMaterial = Material(
        "钻石",
        QVector3D(0.05, 0.05, 0.05), // 环境光
        QVector3D(0.4, 0.4, 0.4),    // 漫反射
        QVector3D(1.0, 1.0, 1.0),    // 镜面反射
        150.0                        // 高光指数
    );

    // 珍珠材质
    Material pearlMaterial = Material(
        "珍珠",
        QVector3D(0.25, 0.20725, 0.20725),       // 环境光
        QVector3D(1.0, 0.829, 0.829),            // 漫反射
        QVector3D(0.296648, 0.296648, 0.296648), // 镜面反射
        11.264                                   // 高光指数
    );

    // 将材质添加到列表中
    materials.push_back(goldMaterial);
    materials.push_back(silverMaterial);
    materials.push_back(copperMaterial);
    materials.push_back(bronzeMaterial);
    materials.push_back(brassMaterial);
    materials.push_back(pewterMaterial);
    materials.push_back(zincMaterial);
    materials.push_back(chromeMaterial);
    materials.push_back(nickelMaterial);
    materials.push_back(titaniumMaterial);
    materials.push_back(emeraldMaterial);
    materials.push_back(jadeMaterial);
    materials.push_back(rubyMaterial);
    materials.push_back(diamondMaterial);
    materials.push_back(pearlMaterial);

    // 设置默认材质
    current_material = materials[0];
}

void Light_Materials::onCheckBoxChanged()
{
    useAmbient = checkBoxAmbient->isChecked();
    useDiffuse = checkBoxDiffuse->isChecked();
    useSpecular = checkBoxSpecular->isChecked();
    // qDebug() << "Ambient: " << useAmbient << ", Diffuse: " << useDiffuse << ", Specular: " << useSpecular;

    update();
}

void Light_Materials::onPositionChanged()
{
    double pos_x = pPositionSpinbox->getValueX();
    double pos_y = pPositionSpinbox->getValueY();
    double pos_z = pPositionSpinbox->getValueZ();
    m_lightPos = QVector3D(pos_x, pos_y, pos_z);
    update();
}

void Light_Materials::updateMaterial(int index)
{
    current_material = materials[index];
    QColor model_color = QColor::fromRgbF(current_material.diffuse.x(), current_material.diffuse.y(), current_material.diffuse.z());
    modelcolor->setColor(model_color);
    update();
}
