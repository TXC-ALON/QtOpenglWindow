#include "CameraUtil.h"
#include "Manager.h"
CameraUtil::CameraUtil(QWidget *widget)
    : QObject(nullptr),
      m_widget(widget)
{
    m_keyMap[FORWARD] = Qt::Key_W;
    m_keyMap[BACKWARD] = Qt::Key_S;
    m_keyMap[LEFT] = Qt::Key_A;
    m_keyMap[RIGHT] = Qt::Key_D;
    m_keyMap[DISABLE] = Qt::Key_Q;
    m_keyMap[ENABLE] = Qt::Key_E;
    m_keyMap[RESET] = Qt::Key_C;
    m_keyMap[SEE] = Qt::Key_P;
    QWidget *camera_widget = create_camera_panel(widget);
    Manager::get_instance()->UpdateCameraDockWidget(camera_widget);
    reset();
}

CameraUtil::~CameraUtil()
{
    // if (camera_panel != nullptr)
    // {
    //     delete camera_panel;
    //     camera_panel = nullptr;
    // }
    // qDebug( ) << "CameraUtil destroyed";
}

void CameraUtil::init()
{
    m_widget->activateWindow();
    m_widget->setFocus();
}

void CameraUtil::reset()
{
    // 重置成员变量为初始值
    m_yaw_angle = -90.0;
    m_pitch_angle = 0;
    m_yaw = qDegreesToRadians(m_yaw_angle);
    m_pitch = qDegreesToRadians(m_pitch_angle);
    m_fov = 45.0;
    update_params();
    calculateCameraDirection();
    m_moveSpeed = 1.8;
    m_sensitivity = 0.1;
    m_cameraPos = QVector3D(0, 0, 5);
    update_position();
    m_widget->killTimer(m_timerId);
    m_timerId = 0;
    m_deltaTime = 0.0;
    m_isMouseMoveDisabled = false;
    m_isPitchConstrained = true;
    m_lastMousePos = m_widget->geometry().center();
    calculateViewMatrix();
    printParameters();
    m_widget->update();
}
void CameraUtil::see_origin()
{
    m_widget->killTimer(m_timerId);
    m_timerId = 0;
    // 计算从当前位置看向原点 (0, 0, 0) 的方向向量
    QVector3D target = QVector3D(0.0, 0.0, 0.0);
    m_cameraDirection = (target - m_cameraPos).normalized();

    std::pair<float, float> pair = calculatePitchAndYaw(m_cameraPos);
    m_pitch = pair.first;
    m_yaw = pair.second;
    m_fov = 45;
    update_params();
    calculateCameraDirection();
    // 更新视图矩阵
    m_view.lookAt(m_cameraPos, m_center, m_cameraUp);
    // 强制刷新 widget 以应用更改
    m_widget->update();
}
void CameraUtil::printParameters()
{
    qDebug() << "Camera Parameters:";
    qDebug() << "Yaw:" << m_yaw;
    qDebug() << "Pitch:" << m_pitch;
    qDebug() << "Field of View (FOV):" << m_fov;
    qDebug() << "Camera Position:" << m_cameraPos;
    qDebug() << "Camera Up Vector:" << m_cameraUp;

    // 如果需要打印其他成员变量，可以继续添加类似的调试信息
}
void CameraUtil::handle(QEvent *event)
{
    bool debuginfo = true;
    switch (event->type())
    {
    // 鼠标移动
    case QEvent::MouseMove:
    {
        auto mouseEvent = static_cast<QMouseEvent *>(event);
        handleMouseMoveEvent(mouseEvent);
        break;
    }
    // 定时器
    case QEvent::Timer:
    {
        auto timerEvent = static_cast<QTimerEvent *>(event);
        handleTimerEvent(timerEvent);
        break;
    }
    // 滚轮
    case QEvent::Wheel:
    {
        auto wheelEvent = static_cast<QWheelEvent *>(event);
        handleWheelEvent(wheelEvent);
        break;
    }
        // 按键按下
    case QEvent::KeyPress:
    {
        auto keyPressEvent = static_cast<QKeyEvent *>(event);
        m_pressedKeys.insert(keyPressEvent->key());
        // 启动定时器
        if (!keyPressEvent->isAutoRepeat() && m_timerId == 0)
        {
            m_timerId = m_widget->startTimer(1);
        }
        // printParameters();
        break;
    }
    // 按键松开
    case QEvent::KeyRelease:
    {
        auto keyReleaseEvent = static_cast<QKeyEvent *>(event);
        m_pressedKeys.remove(keyReleaseEvent->key());
        // 当没有按键按下且定时器正在运行，才关闭定时器
        if (!keyReleaseEvent->isAutoRepeat() && m_timerId != 0 && m_pressedKeys.empty())
        {
            m_widget->killTimer(m_timerId);
            m_timerId = 0;
        }
        break;
    }
    // 刷新
    case QEvent::Paint:
    {
        // 这里有个问题,glfw的似乎是一直在刷新的,而openglwidget仅在需要重新绘制时才刷新,
        //  帧时间
        float curTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
        m_deltaTime = curTime - m_lastFrame;
        m_lastFrame = curTime;
        break;
    }
    // 刷新
    case QEvent::FocusIn:
    {
        m_lastMousePos = m_widget->geometry().center();
        QCursor::setPos(m_widget->mapToGlobal(m_lastMousePos)); // 鼠标在中间
        // m_widget->setCursor(Qt::BlankCursor);                   // 鼠标不可见
        m_widget->setMouseTracking(true);
        break;
    }
    default:
    {
        debuginfo = false;
    }
    }
}

//************************************** get set ****************************************************

void CameraUtil::setKeyMap(CameraUtil::CameraMovement key, Qt::Key value)
{
    m_keyMap[key] = value;
}

float CameraUtil::getMoveSpeed() const
{
    return m_moveSpeed;
}

void CameraUtil::setMoveSpeed(float value)
{
    m_moveSpeed = value;
}

float CameraUtil::getSensitivity() const
{
    return m_sensitivity;
}

void CameraUtil::setSensitivity(float value)
{
    m_sensitivity = value;
}

float CameraUtil::getYaw()
{
    m_yaw_angle = camera_params_spinbox->getValueByIndex(0);
    m_yaw = qDegreesToRadians(m_yaw_angle);
    return m_yaw;
}

void CameraUtil::setYaw(float value)
{
    m_yaw = value;
    calculateCameraDirection();
    update_params();
}

void CameraUtil::setYawAngle(float angle)
{
    m_yaw = qDegreesToRadians(angle);
    calculateCameraDirection();
    update_params();
}

float CameraUtil::getPitch()
{
    m_pitch_angle = camera_params_spinbox->getValueByIndex(1);
    m_pitch = qDegreesToRadians(m_pitch_angle);
    return m_pitch;
}

void CameraUtil::setPitch(float value)
{
    m_pitch = value;
    calculateCameraDirection();
    update_params();
}

void CameraUtil::setPitchAngle(float angle)
{
    m_pitch = qDegreesToRadians(angle);
    calculateCameraDirection();
    update_params();
}

void CameraUtil::setPitchConstrained(bool flag)
{
    m_isPitchConstrained = flag;
}

float CameraUtil::getFov() const
{
    double m_fov = camera_params_spinbox->getValueByIndex(2);
    return m_fov;
}

void CameraUtil::setFov(float value)
{
    m_fov = value;
    camera_params_spinbox->setValueByIndex(2, m_fov);
}

QVector3D CameraUtil::getCameraPos() const
{
    return m_cameraPos;
}

void CameraUtil::setCameraPos(const QVector3D &value)
{
    m_cameraPos = value;
    update_position();
}

QVector3D CameraUtil::getCameraUp() const
{
    return m_cameraUp;
}

void CameraUtil::setCameraUp(const QVector3D &value)
{
    m_cameraUp = value;
}

QVector3D CameraUtil::getCameraDirection() const
{
    return m_cameraDirection;
}

QMatrix4x4 CameraUtil::getViewMatrix()
{
    calculateViewMatrix();
    return m_view;
}

QWidget *CameraUtil::create_camera_panel(QWidget *parent)
{
    if (camera_panel != nullptr)
    {
        delete camera_panel;
        camera_panel = nullptr;
    }
    camera_panel = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(camera_panel);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(1);
    QLabel *camera_Label = new QLabel("相机设置:", parent);
    layout->addWidget(camera_Label);
    QPushButton *camera_see_origin = new QPushButton("看向原点", parent);
    layout->addWidget(camera_see_origin);
    {
        QStringList position_label = {"X", "Y", "Z"};
        camera_position_spinbox = new PositionSpinbox(-1000, 1000, 0.1, 0.0, "摄像机位置", camera_panel, position_label);
        QString params_name = "摄像机参数";
        QStringList params_label = {"yaw", "pitch", "fov"};
        std::vector<double> params_default_value = {m_yaw_angle, m_pitch_angle, m_fov};
        std::vector<double> params_steps = {0.5, 0.5, 0.5};
        std::vector<std::vector<double>> params_range = {{-360, 360}, {-360, 360}, {1, 180}}; // fov范围超过180会画面翻转
        camera_params_spinbox = new PositionSpinbox_New(params_name, params_label, params_default_value, params_steps, params_range, camera_panel);
        layout->addWidget(camera_position_spinbox);
        layout->addWidget(camera_params_spinbox);
    }
    // 设置位置的默认值
    QVector3D camera_position = getCameraPos();
    {
        camera_position_spinbox->setValue(camera_position);
    }
    update_params();
    connect(camera_see_origin, &QPushButton::clicked, this, &CameraUtil::see_origin);
    connect(camera_position_spinbox, &PositionSpinbox::spinbox_valueChanged, this, &CameraUtil::onPositionChanged);
    connect(camera_params_spinbox, &PositionSpinbox_New::spinbox_valueChanged, this, &CameraUtil::onParamsChanged);

    QSpacerItem *camera_verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(camera_verticalSpacer);
    return camera_panel;
}

//***************************************** handle event *********************************************

void CameraUtil::onPositionChanged()
{
    if (!camera_position_spinbox)
    {
        return;
    }
    double pos_x = camera_position_spinbox->getValueX();
    double pos_y = camera_position_spinbox->getValueY();
    double pos_z = camera_position_spinbox->getValueZ();
    QVector3D camera_position = QVector3D(pos_x, pos_y, pos_z);
    setCameraPos(camera_position);
    // qDebug() << "current camera_position  is " << camera_position;
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}

void CameraUtil::onParamsChanged()
{
    if (!m_isMouseMoveDisabled)
    {
        return;
    }
    if (!camera_params_spinbox)
    {
        return;
    }
    m_yaw_angle = camera_params_spinbox->getValueByIndex(0);
    m_pitch_angle = camera_params_spinbox->getValueByIndex(1);
    // qDebug() << "onParamsChanged m_yaw_angle is " << m_yaw_angle;
    // qDebug() << "onParamsChanged m_pitch_angle is " << m_pitch_angle;
    m_fov = camera_params_spinbox->getValueByIndex(2);
    m_yaw = qDegreesToRadians(m_yaw_angle);
    m_pitch = qDegreesToRadians(m_pitch_angle);
    calculateCameraDirection();
    m_widget->update();
}

void CameraUtil::update_position()
{
    camera_position_spinbox->setValue(m_cameraPos);
}

void CameraUtil::update_params()
{
    m_yaw_angle = qRadiansToDegrees(m_yaw);
    m_pitch_angle = qRadiansToDegrees(m_pitch);

    camera_params_spinbox->setValueByIndex(0, m_yaw_angle);
    camera_params_spinbox->setValueByIndex(1, m_pitch_angle);
    camera_params_spinbox->setValueByIndex(2, m_fov);
}

std::pair<float, float> CameraUtil::calculatePitchAndYaw(const QVector3D &cameraPosition)
{
    // 计算相机位置向原点的向量
    QVector3D direction = {-cameraPosition.x(), -cameraPosition.y(), -cameraPosition.z()};
    qDebug() << "calculatePitchAndYaw direction " << direction;
    direction = direction.normalized();
    // 计算pitch角度（绕x轴的旋转）
    float pitch = std::asin(direction.y());
    // 计算yaw角度（绕y轴的旋转）
    float yaw = -std::acos(direction.x() / std::cos(pitch));
    // float yaw = std::atan2(direction.x(), direction.z());
    return {pitch, yaw};
}

void CameraUtil::handleMouseMoveEvent(QMouseEvent *event)
{
    // qDebug( ) << "handleMouseMoveEvent:: x=" << event->x( ) << "y = " << event->y( );

    float offsetX = event->x() - m_lastMousePos.x();
    float offsetY = m_lastMousePos.y() - event->y(); // 注意坐标方向
    offsetX *= m_sensitivity;
    offsetY *= m_sensitivity;
    // 更新lastpos
    m_lastMousePos = event->pos();
    // 如果禁用鼠标移动
    if (m_isMouseMoveDisabled)
        return;
    // 更新欧拉角
    // qDebug() << "handleMouseMoveEvent m_yaw_angle is " << m_yaw_angle;
    // qDebug() << "handleMouseMoveEvent m_pitch_angle is " << m_pitch_angle;

    m_yaw_angle += offsetX;
    m_pitch_angle += offsetY;

    if (m_isPitchConstrained)
    {
        if (m_pitch_angle > 89.5)
        {
            m_pitch_angle = 89.5;
        }
        else if (m_pitch_angle < -89.5)
        {
            m_pitch_angle = -89.5;
        }
    }
    // if (m_yaw_angle > 180)
    // {
    //     m_yaw_angle = 180;
    // }
    // else if (m_yaw_angle < -180)
    // {
    //     m_yaw_angle = -180;
    // }
    m_yaw = qDegreesToRadians(m_yaw_angle);
    m_pitch = qDegreesToRadians(m_pitch_angle);

    calculateCameraDirection();

    camera_params_spinbox->setValueByIndex(0, m_yaw_angle);
    camera_params_spinbox->setValueByIndex(1, m_pitch_angle);
    m_widget->update();
}

void CameraUtil::handleWheelEvent(QWheelEvent *event)
{
    m_fov += event->angleDelta().y() / 360.0;
    if (m_fov > 100)
    {
        m_fov = 100;
    }
    else if (m_fov < 1.0)
    {
        m_fov = 1.0;
    }
    camera_params_spinbox->setValueByIndex(2, m_fov);
    m_widget->update();
}

void CameraUtil::handleTimerEvent(QTimerEvent *event)
{
    // 是否是同一个 timer
    if (event->timerId() != m_timerId)
    {
        return;
    }
    float speed = m_deltaTime * m_moveSpeed;
    if (m_pressedKeys.contains(m_keyMap[FORWARD]))
    {
        m_cameraPos += speed * m_cameraDirection;
    }
    if (m_pressedKeys.contains(m_keyMap[BACKWARD]))
    {
        m_cameraPos -= speed * m_cameraDirection;
    }
    if (m_pressedKeys.contains(m_keyMap[LEFT]))
    {
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos -= speed * cameraRight;
    }
    if (m_pressedKeys.contains(m_keyMap[RIGHT]))
    {
        auto cameraRight = QVector3D::crossProduct(m_cameraDirection, m_cameraUp);
        cameraRight.normalize();
        m_cameraPos += speed * cameraRight;
    }
    // 禁用鼠标监控
    if (m_pressedKeys.contains(m_keyMap[DISABLE]))
    {
        m_isMouseMoveDisabled = true;
        move_position_enabled = true;
        m_widget->setCursor(Qt::ArrowCursor);
    }
    // 启用鼠标监控
    if (m_pressedKeys.contains(m_keyMap[ENABLE]))
    {
        m_isMouseMoveDisabled = false;
        move_position_enabled = false;
        m_widget->setCursor(Qt::BlankCursor); // 鼠标不可见
    }
    // 重置摄像机
    if (m_pressedKeys.contains(m_keyMap[RESET]))
    {
        qDebug() << "Camera reset";
        move_position_enabled = false;
        this->reset();
    }
    if (m_pressedKeys.contains(m_keyMap[SEE]))
    {
        qDebug() << "Camera see_origin";
        m_cameraPos = QVector3D(8, 8, 8);
        this->see_origin();
    }
    if (camera_position_spinbox)
    {
        QVector3D camera_position = getCameraPos();
        camera_position_spinbox->setValue(camera_position);
    }
    m_widget->update();
}

//**************************************** calculate method ********************************************
/*!
 * \brief CameraUtil::calculateCameraDirection 更新摄像机坐标系
 */
void CameraUtil::calculateCameraDirection()
{
    m_cameraDirection = QVector3D(cos(m_yaw) * cos(m_pitch), sin(m_pitch), sin(m_yaw) * cos(m_pitch));
    m_cameraDirection.normalize();
}

/*!
 * \brief CameraUtil::calculateViewMatrix 更新 view 矩阵
 */
void CameraUtil::calculateViewMatrix()
{
    m_view.setToIdentity();
    m_view.lookAt(m_cameraPos, m_cameraPos + m_cameraDirection, m_cameraUp);
    // 如下设置,则相机一直凝视着原点
    //    m_view.lookAt(m_cameraPos, QVector3D(0, 0, 0), m_cameraUp);
}
