#include "LoadModel1.h"

#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include "Config.h"
#define POINT_LIGHT_NUMS 4

LoadModel1::LoadModel1(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_lightPos(1.2f, 1.0f, 2.0f),
      m_camera(this)
{
    m_pointLightValues.resize(POINT_LIGHT_NUMS);
    m_pointLightPositions.resize(POINT_LIGHT_NUMS);

    m_pointLightPositions = {
        {0.7f, 0.2f, 2.0f},
        {2.3f, -3.3f, -4.0f},
        {-4.0f, 2.0f, 3.0f},
        {0.0f, 0.0f, -3.0f}};
    // qDebug() << "set point lights";
    for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
    {
        QString pointlight_name = "point_light_" + QString::number(i);
        m_pointLightValues[i] = std::make_unique<Point_Light>(pointlight_name, m_pointLightPositions[i]);
        // m_pointLightValues[i]->print();
    }
}

LoadModel1::~LoadModel1()
{
    makeCurrent();

    m_VBO.destroy();
    m_lightVAO.destroy();
    m_lightCubeVAO.destroy();
    m_textureDiffuse->destroy();
    m_textureSpecular->destroy();
    m_VAO_Axe.destroy();
    m_VBO_Axe.destroy();
    m_model->destory();
    doneCurrent();
}

void LoadModel1::initializeGL()
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
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/03_Model/multiple_lights.vert");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/03_Model/multiple_lights.frag");
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }
        result = m_lightShader.link();
        if (!result)
        {
            qDebug() << m_lightShader.log();
        }

        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/03_Model/light_cube.vert");
        if (!result)
        {
            qDebug() << m_lightCubeShader.log();
        }
        result = m_lightCubeShader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/03_Model/light_cube.frag");
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
            {-3.0f, 0.0f, 0.0f},
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
    // load model
    {
        // 获取可执行文件所在的目录
        QString executableDir = QCoreApplication::applicationDirPath();

        // 构建相对路径
        QString modelPath = QDir(executableDir).filePath("Resources/objects/nanosuit/nanosuit.obj");
        qDebug() << "executableDir is" << executableDir;
        qDebug() << "modelPath is" << modelPath;

        // use the model in src file path
        m_model = std::make_unique<ModelUtil>(modelPath, context());
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

    initializeAxes();
    // 相机类初始化
    m_camera.init();
    emit initialized();
    qDebug() << "initialized!!";
}

void LoadModel1::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    update();
}

void LoadModel1::paintGL()
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
        m_lightShader.setUniformValue(QString("%1.position").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->position);
        m_lightShader.setUniformValue(QString("%1.ambient").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->ambient);
        m_lightShader.setUniformValue(QString("%1.diffuse").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->diffuse);
        m_lightShader.setUniformValue(QString("%1.specular").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->specular);
        m_lightShader.setUniformValue(QString("%1.constant").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->constant);
        m_lightShader.setUniformValue(QString("%1.linear").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->linear);
        m_lightShader.setUniformValue(QString("%1.quadratic").arg(pointLightStr).toStdString().c_str(), m_pointLightValues[i]->quadratic);
    }
    // spotLight

    // m_spotLight.position = m_camera.getCameraPos();
    if (m_spotLight.follow_camera_position == true)
    {
        m_spotLight.position = m_camera.getCameraPos();
        m_spotLight.update_Position();
    }
    else
    {
        m_spotLight.position = m_spotLight.position;
    }

    if (m_spotLight.follow_camera_direction == true)
    {
        m_spotLight.direction = m_camera.getCameraDirection();
        m_spotLight.update_Direction();
    }
    else
    {
        m_spotLight.direction = m_spotLight.direction;
    }

    m_lightShader.setUniformValue("spotLight.position", m_spotLight.position);
    m_lightShader.setUniformValue("spotLight.direction", m_spotLight.direction);
    m_lightShader.setUniformValue("spotLight.ambient", m_spotLight.ambient);
    m_lightShader.setUniformValue("spotLight.diffuse", m_spotLight.diffuse);
    m_lightShader.setUniformValue("spotLight.specular", m_spotLight.specular);
    m_lightShader.setUniformValue("spotLight.constant", m_spotLight.constant);
    m_lightShader.setUniformValue("spotLight.linear", m_spotLight.linear);
    m_lightShader.setUniformValue("spotLight.quadratic", m_spotLight.quadratic);
    m_lightShader.setUniformValue("spotLight.cutoff", m_spotLight.get_cutoff());
    m_lightShader.setUniformValue("spotLight.outercutoff", m_spotLight.get_outercutoff());
    // m_spotLight.print();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_lightVAO);
    for (int i = 0; i < m_cubePositions.size(); ++i)
    {
        m_lightShader.setUniformValue("model", m_models[i]);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }
    // model from Assimp
    QMatrix4x4 model;
    model.scale(0.15);
    model.translate(0.0, -7.0, 0.0);
    m_lightShader.setUniformValue("model", model);
    m_model->Draw(m_lightShader);

    m_textureDiffuse->release();
    m_textureSpecular->release();
    m_lightShader.release();

    QMatrix4x4 mvp = m_projection * m_camera.getViewMatrix();
    drawAxes(mvp);

    // lamp object
    m_lightCubeShader.bind();
    // MVP
    m_lightCubeShader.setUniformValue("projection", m_projection);
    m_lightCubeShader.setUniformValue("view", m_camera.getViewMatrix());
    // we now draw as many light bulbs as we have point lights.
    QOpenGLVertexArrayObject::Binder vaoBinder1(&m_lightCubeVAO);

    // 点光源的位置颜色设置
    for (int i = 0; i < POINT_LIGHT_NUMS; ++i)
    {
        QMatrix4x4 model;
        model.translate(m_pointLightValues[i]->position);
        model.scale(0.2);
        m_lightCubeShader.setUniformValue("model", model);
        m_lightCubeShader.setUniformValue("lightColor", m_pointLightValues[i]->specular);
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
        // m_pointLightValues[i]->print();
    }
    // 方向光的显示(实际上应该是很远的地方传入的平行光)
    {
        QMatrix4x4 model;
        model.translate(m_dirLight.direction * (-15));
        model.scale(1.5);
        m_lightCubeShader.setUniformValue("model", model);
        m_lightCubeShader.setUniformValue("lightColor", m_dirLight.lightcolor->getColor());
        glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    }
    m_lightCubeShader.release();
}

bool LoadModel1::event(QEvent *e)
{
    m_camera.handle(e);
    return QWidget::event(e);
}

QWidget *LoadModel1::createControlPanel()
{
    // qDebug() << "createControlPanel() start";
    QWidget *panel = new QWidget();
    layout = new QVBoxLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0); // 设置边距为5像素
    layout->setSpacing(10);
    // panel->setStyleSheet("border: 1px solid red;"); // 方便查看widget的大小布局
    {

        QLabel *Label_DirectionLight = new QLabel("方向光:", panel);
        QLabel *Label_PointLight = new QLabel("点光源:", panel);
        QLabel *Label_current_PointLight = new QLabel("当前点光源:", panel);
        QLabel *Label_SpotLight = new QLabel("聚光:", panel);
        pointlightComboBox = new QComboBox(panel);
        for (int i = 0; i < m_pointLightValues.size(); i++)
        {
            pointlightComboBox->addItem(QString::number(i));
        }
        Label_DirectionLight->setStyleSheet("QLabel { font-weight: bold; font-size: 16px;   }");
        Label_PointLight->setStyleSheet("QLabel { font-weight: bold; font-size: 16px; }");
        Label_current_PointLight->setStyleSheet("QLabel { font-weight: bold; font-size: 13px; }");
        Label_SpotLight->setStyleSheet("QLabel { font-weight: bold;font-size: 16px;  }");

        QWidget *directionlight_control_panel = m_dirLight.create_light_control_panel();
        currentPointLightPanel = m_pointLightValues[0]->create_light_control_panel();
        QWidget *spotlight_control_panel = m_spotLight.create_light_control_panel();

        QFrame *separator1 = new QFrame;
        separator1->setFrameShape(QFrame::HLine);
        QFrame *separator2 = new QFrame;
        separator2->setFrameShape(QFrame::HLine);

        layout->addWidget(Label_DirectionLight);
        layout->addWidget(directionlight_control_panel);
        layout->addWidget(separator1);
        layout->addWidget(Label_current_PointLight);
        layout->addWidget(pointlightComboBox);
        layout->addWidget(Label_PointLight);
        layout->addWidget(currentPointLightPanel);
        layout->addWidget(separator2);
        layout->addWidget(Label_SpotLight);
        layout->addWidget(spotlight_control_panel);
    }
    {
        connect(pointlightComboBox, &QComboBox::currentIndexChanged, this, &LoadModel1::onPointLightSelected);
    }
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    // qDebug() << "Size before adjustSize():" << panel->size();
    panel->adjustSize();
    // qDebug() << "Size after adjustSize():" << panel->size();

    // qDebug() << "createControlPanel() end";
    return panel;
}

void LoadModel1::onPointLightSelected(int index)
{
    // qDebug() << "change ! current index light = " << index;

    if (index < 0 || index > m_pointLightValues.size())
    {
        qDebug() << "overrange!!!" << index;
        return;
    }
    // 删除旧的点光源控制面板
    if (currentPointLightPanel)
    {
        currentPointLightPanel->deleteLater();
    }
    QWidget *newPointLightPanel = m_pointLightValues[index]->create_light_control_panel();
    // qDebug( ) << "onPointLightSelected !!!" << index;
    // m_pointLightValues[index]->print();
    // 使用辅助函数替换控件
    replaceWidget(layout, currentPointLightPanel, newPointLightPanel);
    // 更新当前点光源控制面板的引用
    currentPointLightPanel = newPointLightPanel;
    newPointLightPanel = nullptr;
}

void LoadModel1::replaceWidget(QVBoxLayout *layout, QWidget *oldWidget, QWidget *newWidget)
{
    if (layout && oldWidget && newWidget)
    {
        // 将旧的widget从布局中移除，并隐藏
        int index = layout->indexOf(oldWidget);
        layout->removeWidget(oldWidget);
        oldWidget->hide();
        // 获取旧widget在布局中的位置
        if (index != -1)
        {
            // 在相同的位置插入新widget
            layout->insertWidget(index, newWidget);
        }
        else
        {
            // 如果旧widget不在布局中，直接添加新widget
            layout->addWidget(newWidget);
        }

        // 显示新widget
        newWidget->show();

        // 确保删除旧widget
        oldWidget->deleteLater();
    }
}

void LoadModel1::drawAxes(QMatrix4x4 mvp)
{
    m_program_Axe.bind();
    m_VAO_Axe.bind();
    glLineWidth(3.0f);
    // 设置线宽为2.0，可以根据需要调整这个值
    glEnable(GL_LINE_SMOOTH); // 启用点平滑
    for (int i = 0; i < 3; ++i)
    {
        glDrawArrays(GL_LINES, i * 2, 2); // 每个轴由两个顶点组成
    }
    m_VAO_Axe.release();

    m_program_Axe.setUniformValue("mvp", mvp);
    m_program_Axe.release();
}

void LoadModel1::initializeAxes()
{
    float vertices_axes[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        10.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // Y轴
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // Z轴
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f};

    // shader初始化
    {
        bool result = true;

        // no uniform
        result = m_program_Axe.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/08_review/review_axes.vert");
        if (!result)
        {
            qDebug() << m_program_Axe.log();
        }
        result = m_program_Axe.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/08_review/review_axes.frag");
        if (!result)
        {
            qDebug() << m_program_Axe.log();
        }
        result = m_program_Axe.link();
        if (!result)
        {
            qDebug() << m_program_Axe.log();
        }
    }
    {
        // 1. 绑定顶点数组对象
        m_VAO_Axe.create();
        m_VAO_Axe.bind();
        // 2. 把顶点数组复制到缓冲中供OpenGL使用
        m_VBO_Axe.create();
        m_VBO_Axe.bind();
        // 将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
        m_VBO_Axe.allocate(vertices_axes, sizeof(vertices_axes));
        // 3. 设置顶点属性指针
        // 位置属性
        m_program_Axe.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
        m_program_Axe.enableAttributeArray(0);
        // 颜色属性
        m_program_Axe.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
        m_program_Axe.enableAttributeArray(1);
    }
}
