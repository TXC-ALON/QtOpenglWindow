#ifndef CAMERAUTIL_H
#define CAMERAUTIL_H

#include <QWidget>
#include <QObject>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QTime>
#include <QSet>
#include <QMap>
#include "PositionSpinbox.h"
#include "PositionSpinbox_New.h"
#include <QVBoxLayout>
#include <QCheckBox>
#include <QtMath>

// https://blog.csdn.net/qq_40946921/article/details/106014771
class CameraUtil : public QObject
{
    Q_OBJECT
public:
    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        DISABLE,
        ENABLE,
        RESET,
        SEE
    };
    CameraUtil(QWidget *widget);
    ~CameraUtil();
    // 初始化
    void init();
    void reset();
    void see_origin();

    // 处理事件
    void handle(QEvent *event);
    // 设置按键响应
    void setKeyMap(CameraMovement key, Qt::Key value);
    // 移动速度
    float getMoveSpeed() const;
    void setMoveSpeed(float value);
    // 鼠标灵敏度
    float getSensitivity() const;
    void setSensitivity(float value);
    // 偏航角
    float getYaw();
    void setYaw(float value);
    void setYawAngle(float angle);

    // 俯视角
    float getPitch();
    void setPitch(float value);
    void setPitchAngle(float angle);
    void setPitchConstrained(bool flag);
    // fov
    float getFov() const;
    void setFov(float value);
    // 摄像机位置
    QVector3D getCameraPos() const;
    void setCameraPos(const QVector3D &value);
    QVector3D getCameraUp() const;
    void setCameraUp(const QVector3D &value);

    // 摄像机方向，由于方向是由偏航角和俯视角得到的，故不提供set函数
    QVector3D getCameraDirection() const;
    // MVP
    QMatrix4x4 getViewMatrix();

    // 打印调试
    void printParameters();

public:
    QWidget *camera_panel = nullptr;
    QWidget *create_camera_panel(QWidget *widget = nullptr);
    PositionSpinbox *camera_position_spinbox = nullptr;
    PositionSpinbox *camera_direction_spinbox = nullptr;
    PositionSpinbox_New *camera_params_spinbox = nullptr;
    bool move_position_enabled = false;
    void onPositionChanged();
    void onParamsChanged();
    void update_position();
    void update_params();

private:
    std::pair<float, float> calculatePitchAndYaw(const QVector3D &cameraPosition);

    void handleMouseMoveEvent(QMouseEvent *event);
    void handleWheelEvent(QWheelEvent *event);
    void handleTimerEvent(QTimerEvent *event);
    void calculateCameraDirection();
    void calculateViewMatrix();

private:
    QWidget *m_widget;
    // 相机的控制参数
    float m_sensitivity;
    float m_yaw;
    float m_pitch;
    float m_yaw_angle = -89.5;
    float m_pitch_angle = 0;
    float m_fov;
    QVector3D m_center = QVector3D(0, 0, 0);
    QVector3D m_cameraDirection = QVector3D(0, 0, -1);
    // 相机的基本参数
    QMatrix4x4 m_view;
    QVector3D m_cameraPos = QVector3D(0, 0, 5);
    QVector3D m_cameraUp = QVector3D(0, 1, 0); // 永远为010
    // 相机的按键响应
    QMap<CameraMovement, Qt::Key> m_keyMap;
    QSet<int> m_pressedKeys;
    // 相机的时间响应
    int m_timerId = 0;
    float m_deltaTime = 0.0; // 当前帧与上一帧的时间差
    float m_lastFrame = 0.0; // 上一帧的时间
    float m_moveSpeed = 1.8;

    QPoint m_lastMousePos;

    bool m_isMouseMoveDisabled = false;
    bool m_isPitchConstrained = true;
};

#endif // CAMERAUTIL_H
