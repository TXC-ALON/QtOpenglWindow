#include "Review_glsl.h"

Review_GLSL::Review_GLSL(QWidget *parent)
    : GLWidget(parent),
      m_VBO(QOpenGLBuffer::VertexBuffer),
      m_angleSelf(0.0f), m_angleOrbit(0.0f)

{
    createDrawModeControl();
    connect(&m_timer, &QTimer::timeout, this, &Review_GLSL::updateAngles);
    m_timer.start(16); // 每秒大约60次更新}
}

Review_GLSL::~Review_GLSL()
{
    makeCurrent();

    m_VAO.destroy();
    m_VAO_origin.destroy();
    m_VBO.destroy();
    m_VBO_origin.destroy();

    doneCurrent();
}

void Review_GLSL::custom_initializeGL()
{
    if (!context())
    {
        qCritical() << "Can't get OpenGL context";
        close();
        return;
    }
    initializeOpenGLFunctions();
    m_cameraPos = QVector3D(0.0f, 0.0f, 15.0f);

    // 初始化三角形的着色器程序
    {
        bool result = true;
        result = m_program_triangle.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/08_review/review_glsl.vert");
        if (!result)
        {
            qDebug() << m_program_triangle.log();
        }
        result = m_program_triangle.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/08_review/review_glsl.frag");
        if (!result)
        {
            qDebug() << m_program_triangle.log();
        }
        result = m_program_triangle.link();
        if (!result)
        {
            qDebug() << m_program_triangle.log();
        }
    }

    // 初始化原点的着色器程序
    {
        bool result = true;
        result = m_program_origin.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/shaders/01_Bird/08_review/origin.vert");
        if (!result)
        {
            qDebug() << m_program_origin.log();
        }
        result = m_program_origin.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/shaders/01_Bird/08_review/origin.frag");
        if (!result)
        {
            qDebug() << m_program_origin.log();
        }
        result = m_program_origin.link();
        if (!result)
        {
            qDebug() << m_program_origin.log();
        }
    }

    // 设置三角形的VAO/VBO
    {
        GLfloat vertices[] = {
            0.268f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, // left
            3.732f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0, // right
            2.0f, 4.0f, 0.0f, 0.0f, 0.0f, 1.0f, 3.0    // top
        };
        m_VAO.create();
        m_VAO.bind();
        m_VBO.create();
        m_VBO.bind();
        m_VBO.allocate(vertices, sizeof(vertices));
        m_program_triangle.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 7);
        m_program_triangle.enableAttributeArray(0);
        m_program_triangle.setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 7);
        m_program_triangle.enableAttributeArray(1);
        m_program_triangle.setAttributeBuffer(2, GL_FLOAT, sizeof(GLfloat) * 3, 1, sizeof(GLfloat) * 7);
        m_program_triangle.enableAttributeArray(2);
        m_VAO.release();
        m_VBO.release();
    }

    // 设置原点的VAO/VBO
    if (1)
    {
        GLfloat origin[] = {
            0.0f, 0.0, 0.0f,
            1.0f, 0.0, 0.0f};
        m_VAO_origin.create();
        m_VAO_origin.bind();
        m_VBO_origin.create();
        m_VBO_origin.bind();
        m_VBO_origin.allocate(origin, sizeof(origin));
        m_program_origin.setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);
        m_program_origin.enableAttributeArray(0);
        m_VAO_origin.release();
        m_VBO_origin.release();
    }
}

void Review_GLSL::custom_resizeGL(int w, int h)
{
    qDebug() << "w is" << w << ",h is" << h;
    glViewport(0, 0, w, h);
    projection.setToIdentity();
    projection.perspective(45.0f, w / static_cast<float>(h ? h : 1), 0.1f, 100.0f);
}

void Review_GLSL::custom_paintGL()
{
    init_paintGL();
    // 绘制三角形
    m_program_triangle.bind();
    m_program_triangle.setUniformValue("time", time);
    QMatrix4x4 view;
    view.lookAt(m_cameraPos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    view = projection * view;
    m_program_triangle.setUniformValue("view", view);
    // m_program_triangle.setUniformValue("projection", projection);

    QMatrix4x4 model;
    model.translate(center.x(), center.y());   // 移动到中心点
    model.rotate(-m_angleSelf, 0, 0, 1);       // 绕自身中心顺时针旋转
    model.translate(-center.x(), -center.y()); // 移回原点
    m_program_triangle.setUniformValue("model", model);

    QMatrix4x4 transform;
    transform.rotate(m_angleOrbit, 0, 0, 1); // 逆时针旋转
    m_program_triangle.setUniformValue("transform", transform);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_program_triangle.release();

    // 绘制原点
    m_program_origin.bind();
    m_program_origin.setUniformValue("view", view);
    m_program_origin.setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoOriginBinder(&m_VAO_origin);
    glPointSize(15.0f);            // 这里的值可以根据需要调整
    glEnable(GL_POINT_SMOOTH);     // 启用点平滑
    glDrawArrays(GL_POINTS, 0, 2); // 注意这里只绘制一个点
    m_program_origin.release();
    end_paintGL();
}

void Review_GLSL::updateAngles()
{
    m_angleSelf += 2.0f;  // 每帧增加1度
    m_angleOrbit += 1.0f; // 每帧增加0.5度
    if (m_angleSelf >= 360.0f)
        m_angleSelf -= 360.0f;
    if (m_angleOrbit >= 360.0f)
        m_angleOrbit -= 360.0f;
    time += 0.016f;
    // qDebug() << "time is " << time;
    update(); // 触发重绘
}
