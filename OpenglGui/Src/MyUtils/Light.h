#pragma once
#include <iostream>
#include <QVector3D>
#include <QWidget>
#include <QMap>
#include <QString>
#include <QComboBox>
#include <QCheckBox>
#include <ColorButton.h>
#include <SliderWithLabel.h>
#include <PositionSpinbox.h>
#include <QVBoxLayout>
#include <QObject>
#include <QtMath>
class BasicLight
{
public:
    // 默认构造函数
    BasicLight(double c = 1.0, double l = 0.09, double q = 0.032)
        : constant(c), linear(l), quadratic(q) {}

    // 成员函数，用于输出光的参数
    void print() const;

public:
    float constant;
    float linear;
    float quadratic;
};

extern QMap<int, BasicLight> BasicLight_Map;

class Light : public QObject
{
    Q_OBJECT
public:
    explicit Light(QString _name = "default_light",
                   const QVector3D &ambient = QVector3D(0.1, 0.1, 0.1),
                   const QVector3D &diffuse = QVector3D(0.5, 0.5, 0.5),
                   const QVector3D &specular = QVector3D(1.0, 1.0, 1.0))
        : QObject(nullptr), enabled(true), name(_name), ambient(ambient), diffuse(diffuse), specular(specular)
    {
        set_basic_clq(4);
    }
    virtual ~Light();
    // 检查光源是否启用
    bool isEnabled() const;
    void print_info();

public:
    bool enabled;
    QList<int> level_list = {7, 13, 20, 32, 50, 65, 100, 160, 200, 325, 600, 3250};
    void set_basic_clq(int level)
    {
        basic_clq = BasicLight_Map[level_list[level]];
        constant = basic_clq.constant;
        linear = basic_clq.linear;
        quadratic = basic_clq.quadratic;
    }
    QWidget *create_light_clq_panel(QWidget *widget = nullptr, int clq_index = 4);
    virtual QWidget *create_light_control_panel(QWidget *widget = nullptr)
    {
        return nullptr;
    }

public:
    void onCLQChanged(int index);

public:
    QString name;
    int clq_level = 4;
    BasicLight basic_clq; // constant,linear,quadratic
    float constant;
    float linear;
    float quadratic;
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;

    QComboBox *comboBox;
    QLabel *labelConstant;
    QLabel *labelLinear;
    QLabel *labelQuadratic;
};

class Direction_Light : public Light
{
    Q_OBJECT
public:
    // 构造函数
    explicit Direction_Light(QString _name = "default_directionlight",
                             const QVector3D &_LightDirection = QVector3D(0, 0, -1),
                             const QVector3D &ambient = QVector3D(0.1, 0.1, 0.1),
                             const QVector3D &diffuse = QVector3D(0.5, 0.5, 0.5),
                             const QVector3D &specular = QVector3D(1.0, 1.0, 1.0))
        : Light(_name, ambient, diffuse, specular), direction(_LightDirection) {}
    ~Direction_Light()
    {
        qDebug() << "Direction_Light destroyed:" << name;
    }
    void print();
    virtual QWidget *create_light_control_panel(QWidget *widget = nullptr) override;

public:
    QVector3D direction;
    float lastAngleX = 0;
    float lastAngleY = 0;
    float lastAngleZ = 0;
    QVector3D position = QVector3D(0, 0, 1);
    ColorButton *lightcolor;
    PositionSpinbox *lightdirection;
    QList<SliderWithLabel *> light_rotation_controls;
public slots:
    void onLightColorChanged();
    void onDirectionChanged();
    void onRotationChanged(int index);

public:
    QVector3D rotateAroundX(const QVector3D &v, float angle)
    {
        float y = v.y() * qCos(angle) + v.z() * qSin(angle);
        float z = -v.y() * qSin(angle) + v.z() * qCos(angle);
        return QVector3D(v.x(), y, z);
    }

    QVector3D rotateAroundY(const QVector3D &v, float angle)
    {
        float x = v.x() * qCos(angle) + v.z() * qSin(angle);
        float z = -v.x() * qSin(angle) + v.z() * qCos(angle);
        return QVector3D(x, v.y(), z);
    }

    QVector3D rotateAroundZ(const QVector3D &v, float angle)
    {
        float x = v.x() * qCos(angle) - v.y() * qSin(angle);
        float y = v.x() * qSin(angle) + v.y() * qCos(angle);
        return QVector3D(x, y, v.z());
    }
};

class Point_Light : public Light
{
    Q_OBJECT
public:
    // 构造函数
    explicit Point_Light(QString _name = "default_pointlight",
                         QVector3D _LightPosition = QVector3D(0, 0, 1),
                         QVector3D _LightDirection = QVector3D(0, 0, -1),
                         const QVector3D &ambient = QVector3D(0.1, 0.1, 0.1),
                         const QVector3D &diffuse = QVector3D(0.5, 0.5, 0.5),
                         const QVector3D &specular = QVector3D(1.0, 1.0, 1.0))
        : Light(_name, ambient, diffuse, specular),
          position(_LightPosition),
          direction(_LightDirection)
    {
    }
    ~Point_Light()
    {
        qDebug() << "Point_Light destroyed:" << name;
    }
    void print();
    virtual QWidget *create_light_control_panel(QWidget *widget = nullptr) override;

public:
    QVector3D position;
    QVector3D direction;
    ColorButton *lightcolor;
    PositionSpinbox *lightdirection;
    PositionSpinbox *lightposition;
    QWidget *clq_panel = nullptr;
private slots:
    void onLightColorChanged();
    void onPositionChanged();
    void onDirectionChanged();
};

class Spot_Light : public Light
{
    Q_OBJECT
public:
    // 构造函数
    explicit Spot_Light(QString _name = "default_spotlight",
                        const QVector3D &_LightPosition = QVector3D(0, 0, 1),
                        const QVector3D &_LightDirection = QVector3D(0, 0, -1),
                        float _cutOff = 10,
                        float _outerCutOff = 15,
                        const QVector3D &ambient = QVector3D(0.1, 0.1, 0.1),
                        const QVector3D &diffuse = QVector3D(0.5, 0.5, 0.5),
                        const QVector3D &specular = QVector3D(1.0, 1.0, 1.0))
        : Light(_name, ambient, diffuse, specular),
          position(_LightPosition),
          direction(_LightDirection),
          cutoff_angle(_cutOff),
          outercutoff_angle(_outerCutOff)
    {
        cutoff = cos(cutoff_angle * 3.1415926 / 180.0);
        outercutoff = cos(outercutoff_angle * 3.1415926 / 180.0);
    }
    ~Spot_Light()
    {
        qDebug() << "Spot_Light destroyed:" << name;
    }
    void print();
    virtual QWidget *create_light_control_panel(QWidget *widget = nullptr) override;
    void update_Position();
    void update_Direction();
    float get_cutoff();
    float get_outercutoff();

public:
    QCheckBox *check_follow_camera_position = nullptr;
    QCheckBox *check_follow_camera_direction = nullptr;
    QCheckBox *check_soft_spot_light = nullptr;
    QCheckBox *check_fix_soft_spot_light = nullptr;

    ColorButton *lightcolor;
    PositionSpinbox *lightdirection;
    PositionSpinbox *lightposition;
    QWidget *clq_panel = nullptr;
    QList<SliderWithLabel *> light_cutoff_controls;

public:
    QVector3D position;
    QVector3D direction;
    float cutoff_angle;
    float outercutoff_angle;
    float cutoff;
    float outercutoff;

public:
    bool follow_camera_position = true;  // 聚光采用相机位置作为光照位置
    bool follow_camera_direction = true; // 聚光采用相机方向作为光照方向
    bool is_soft_spot_light = true;      // 是否采用柔光
    bool is_fix_soft_spot_light = true;  // 设置cutoff一定小于outercutoff

private slots:
    void onLightColorChanged();
    void onPositionChanged();
    void onDirectionChanged();
    void onSoftButtonChanged(int state);
    void onSlideCutOffChanged();
};
