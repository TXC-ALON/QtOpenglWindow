#include "Light.h"
#include <QDebug>
#include "Manager.h"

QMap<int, BasicLight> BasicLight_Map = {
    {7, BasicLight(1.0, 0.7, 1.8)},
    {13, BasicLight(1.0, 0.35, 0.44)},
    {20, BasicLight(1.0, 0.22, 0.20)},
    {32, BasicLight(1.0, 0.14, 0.07)},
    {50, BasicLight(1.0, 0.09, 0.032)},
    {65, BasicLight(1.0, 0.07, 0.017)},
    {100, BasicLight(1.0, 0.045, 0.0075)},
    {160, BasicLight(1.0, 0.027, 0.0028)},
    {200, BasicLight(1.0, 0.022, 0.0019)},
    {325, BasicLight(1.0, 0.014, 0.0007)},
    {600, BasicLight(1.0, 0.007, 0.0002)},
    {3250, BasicLight(1.0, 0.0014, 0.000007)}};
void BasicLight::print() const
{
    qDebug() << "Constant: " << constant << ", Linear: " << linear << ", Quadratic: " << quadratic;
}

Light::~Light()
{
    qDebug() << "Light destroyed:" << name;
}

bool Light::isEnabled() const
{
    return enabled;
}

void Light::print_info()
{
    qDebug() << "Lightname is" << name;
    basic_clq.print();
    qDebug() << "ambient: " << ambient << "diffuse: " << diffuse << "specular: " << specular;
}

QWidget *Light::create_light_clq_panel(QWidget *parent, int clq_index)
{
    QWidget *light_clq_panel = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(light_clq_panel);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(1);
    comboBox = new QComboBox(light_clq_panel);
    for (auto it = BasicLight_Map.begin(); it != BasicLight_Map.end(); ++it)
    {
        comboBox->addItem(QString::number(it.key()));
    }
    QLabel *light_attenuation_Label = new QLabel("光源传播距离:", parent);
    layout->addWidget(light_attenuation_Label);
    labelConstant = new QLabel("Constant: ", parent);
    labelLinear = new QLabel("Linear: ", parent);
    labelQuadratic = new QLabel("Quadratic: ", parent);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(2, 2, 2, 2);
    hlayout->setSpacing(1);
    hlayout->addWidget(labelConstant);
    hlayout->addWidget(labelLinear);
    hlayout->addWidget(labelQuadratic);

    // 将控件添加到布局
    layout->addWidget(comboBox);
    layout->addLayout(hlayout);

    connect(comboBox, &QComboBox::currentIndexChanged, this, &Light::onCLQChanged);
    comboBox->setCurrentIndex(clq_index);
    return light_clq_panel;
}
void Light::onCLQChanged(int index)
{
    // qDebug() << "Light::current index is " << index;
    clq_level = index;
    int distance = level_list[index];
    auto point = BasicLight_Map.value(distance);
    // qDebug() << point.constant << " " << point.linear << " " << point.quadratic;
    set_basic_clq(index);
    labelConstant->setText(QString("Constant: %1").arg(constant));
    labelLinear->setText(QString("Linear: %1").arg(linear));
    labelQuadratic->setText(QString("Quadratic: %1").arg(quadratic));
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}
void Direction_Light::print()
{
    qDebug() << "Direction_Light::print()";
    print_info();
    qDebug() << "direction: " << direction;
}
QWidget *Direction_Light::create_light_control_panel(QWidget *parent)
{
    QWidget *direction_light_panel = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(direction_light_panel);
    layout->setContentsMargins(2, 2, 2, 2);

    layout->setSpacing(1);
    direction_light_panel->setLayout(layout); // 确保设置布局

    {
        lightdirection = new PositionSpinbox(-10, 10, 0.1, 0.0, "方向光方向控制");
        lightcolor = new ColorButton("平行光颜色", QVector3D(255, 255, 255));
        layout->addWidget(lightdirection);
        layout->addWidget(lightcolor);
    }

    {
        lightdirection->setValueX(direction.x());
        lightdirection->setValueY(direction.y());
        lightdirection->setValueZ(direction.z());
    }
    {
        connect(lightcolor, &ColorButton::color_changed, this, &Direction_Light::onLightColorChanged);
        connect(lightdirection, &PositionSpinbox::spinbox_valueChanged, this, &Direction_Light::onDirectionChanged);
    }
    QStringList text = {"x-axis", "y-axis", "z-axis"};
    std::vector<double> default_value = {0, 0, 0};
    std::vector<double> steps = {1, 1, 1};
    std::vector<std::vector<int>> range = {{0, 360}, {0, 360}, {0, 360}};
    for (int i = 0; i < text.size(); ++i)
    {
        light_rotation_controls.push_back(new SliderWithLabel(text[i], range[i][0], range[i][1], steps[i], default_value[i], parent, i));
        QSize hintSize = light_rotation_controls[i]->sizeHint();
        light_rotation_controls[i]->setMinimumSize(hintSize);
        layout->addWidget(light_rotation_controls[i]);
        connect(light_rotation_controls[i], &SliderWithLabel::slider_valueChanged, this, &Direction_Light::onRotationChanged);
    }

    direction_light_panel->adjustSize();
    return direction_light_panel;
}
void Direction_Light::onLightColorChanged()
{
    QVector3D currentcolor = lightcolor->getColor();
    // qDebug() << "current direction light color is " << currentcolor;
    ambient = 0.1 * currentcolor;
    diffuse = 0.5 * currentcolor;
    specular = 1.0 * currentcolor;
}
void Direction_Light::onDirectionChanged()
{
    double pos_x = lightdirection->getValueX();
    double pos_y = lightdirection->getValueY();
    double pos_z = lightdirection->getValueZ();
    direction = QVector3D(pos_x, pos_y, pos_z);
    // qDebug() << "current dirction is " << direction;
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}
// void Direction_Light::onRotationChanged(int index)
// {
//     float angleX = light_rotation_controls[0]->getValue(); // 度数
//     float angleY = light_rotation_controls[1]->getValue(); // 度数
//     float angleZ = light_rotation_controls[2]->getValue(); // 度数

//     // 将度数转换为弧度
//     float angleXRadians = qDegreesToRadians(angleX);
//     float angleYRadians = qDegreesToRadians(angleY);
//     float angleZRadians = qDegreesToRadians(angleZ);
//     // 光源的初始位置
//     QVector3D position = QVector3D(0, 0, 1);

//     // 绕Z轴旋转
//     float rotatedXz = position.x() * qCos(angleZRadians) - position.y() * qSin(angleZRadians);
//     float rotatedYz = position.x() * qSin(angleZRadians) + position.y() * qCos(angleZRadians);
//     QVector3D tempPositionZ(rotatedXz, rotatedYz, position.z());
//     qDebug() << "tempPositionZ:" << tempPositionZ;
//     // 绕Y轴旋转
//     float rotatedXy = tempPositionZ.x() * qCos(angleYRadians) + tempPositionZ.z() * qSin(angleYRadians);
//     float rotatedZy = -tempPositionZ.x() * qSin(angleYRadians) + tempPositionZ.z() * qCos(angleYRadians);
//     QVector3D tempPositionY(rotatedXy, tempPositionZ.y(), rotatedZy);

//     // 绕X轴旋转
//     float rotatedYx = tempPositionY.y() * qCos(angleXRadians) - tempPositionY.z() * qSin(angleXRadians);
//     float rotatedZx = tempPositionY.y() * qSin(angleXRadians) + tempPositionY.z() * qCos(angleXRadians);
//     QVector3D newPosition(rotatedXy, rotatedYx, rotatedZx);

//     // 更新光源位置
//     position = newPosition;
//     qDebug() << "New Position:" << position;
//     QVector3D normalizedPosition = position.normalized();
//     direction = -normalizedPosition;
//     qDebug() << "onRotationChanged() direction is " << direction;
//     lightdirection->setValue(direction);
// }

void Direction_Light::onRotationChanged(int index)
{
    // qDebug() << "Direction_Light::onRotationChanged index is " << index;
    float angleX = light_rotation_controls[0]->getValue();
    float angleY = light_rotation_controls[1]->getValue();
    float angleZ = light_rotation_controls[2]->getValue();

    // 将度数转换为弧度
    float angleXRadians = qDegreesToRadians(angleX);
    float angleYRadians = qDegreesToRadians(angleY);
    float angleZRadians = qDegreesToRadians(angleZ);

    // 计算增量
    float deltaAngleX = angleXRadians - lastAngleX;
    float deltaAngleY = angleYRadians - lastAngleY;
    float deltaAngleZ = angleZRadians - lastAngleZ;

    // 应用旋转
    switch (index)
    {
    case 0: // X轴
        position = rotateAroundX(position, deltaAngleX);
        break;
    case 1: // Y轴
        position = rotateAroundY(position, deltaAngleY);
        break;
    case 2: // Z轴
        position = rotateAroundZ(position, deltaAngleZ);
        break;
    }

    // 更新上一次的旋转角度
    lastAngleX = angleXRadians;
    lastAngleY = angleYRadians;
    lastAngleZ = angleZRadians;

    // 归一化位置向量
    position = position.normalized();

    // 更新方向向量
    direction = -position; // 假设方向始终指向原点

    // qDebug() << "New Position:" << position;
    // qDebug() << "onRotationChanged() direction is " << direction;
    lightdirection->setValue(direction);
}

void Point_Light::print()
{
    qDebug() << "Point_Light::print()";
    print_info();
    qDebug() << "position: " << position;
    qDebug() << "direction: " << direction;
}

QWidget *Point_Light::create_light_control_panel(QWidget *parent)
{
    QWidget *point_light_panel = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(point_light_panel);
    layout->setContentsMargins(2, 2, 2, 2);

    layout->setSpacing(1);
    point_light_panel->setLayout(layout); // 确保设置布局

    {
        lightposition = new PositionSpinbox(-10, 10, 0.1, 0.0, "点光源位置控制");
        // lightdirection = new PositionSpinbox(-10, 10, 0.1, 0.0, "点光源方向控制");
        lightcolor = new ColorButton("点光源颜色", QVector3D(255, 255, 255));
        clq_panel = this->create_light_clq_panel(point_light_panel, clq_level);
        layout->addWidget(lightposition);
        // layout->addWidget(lightdirection);
        layout->addWidget(lightcolor);
        layout->addWidget(clq_panel);
    }

    {
        // qDebug() << "Point_Light create_light_control_panel current position is <<" << position;
        lightposition->setValueX(position.x());
        lightposition->setValueY(position.y());
        lightposition->setValueZ(position.z());
        // lightdirection->setValueX(direction.x());
        // lightdirection->setValueY(direction.y());
        // lightdirection->setValueZ(direction.z());
        lightcolor->setColor(specular);
    }
    {
        connect(lightcolor, &ColorButton::color_changed, this, &Point_Light::onLightColorChanged);
        connect(lightposition, &PositionSpinbox::spinbox_valueChanged, this, &Point_Light::onPositionChanged);
        // connect(lightdirection, &PositionSpinbox::spinbox_valueChanged, this, &Point_Light::onDirectionChanged);
    }

    point_light_panel->adjustSize();
    // qDebug() << "Point_Light return";

    return point_light_panel;
}
void Point_Light::onLightColorChanged()
{
    QVector3D currentcolor = lightcolor->getColor();
    // qDebug() << "current point light color is " << currentcolor;
    ambient = 0.1 * currentcolor;
    diffuse = 0.5 * currentcolor;
    specular = 1.0 * currentcolor;
}
void Point_Light::onPositionChanged()
{
    double pos_x = lightposition->getValueX();
    double pos_y = lightposition->getValueY();
    double pos_z = lightposition->getValueZ();
    position = QVector3D(pos_x, pos_y, pos_z);
    // qDebug() << "current position is " << position;
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}
void Point_Light::onDirectionChanged()
{
    double pos_x = lightdirection->getValueX();
    double pos_y = lightdirection->getValueY();
    double pos_z = lightdirection->getValueZ();
    direction = QVector3D(pos_x, pos_y, pos_z);
    // qDebug() << "current dirction is " << direction;
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}
void Spot_Light::print()
{
    qDebug() << "\nSpot_Light::print()";
    print_info();
    qDebug() << "position: " << position;
    qDebug() << "direction: " << direction;
    qDebug() << "cutOff: " << cutoff << "outerCutOff: " << outercutoff;
    qDebug() << "cutOff: " << cutoff_angle << "outerCutOff: " << outercutoff_angle;
}

QWidget *Spot_Light::create_light_control_panel(QWidget *parent)
{
    QWidget *spot_light_panel = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(spot_light_panel);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(1);
    spot_light_panel->setLayout(layout); // 确保设置布局

    {
        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->setContentsMargins(2, 2, 2, 2);
        hlayout->setSpacing(1);

        check_follow_camera_position = new QCheckBox("光源设为相机位置");
        check_follow_camera_direction = new QCheckBox("方向设为相机方向");
        check_follow_camera_position->setChecked(follow_camera_position);
        check_follow_camera_direction->setChecked(follow_camera_direction);

        hlayout->addWidget(check_follow_camera_position);
        hlayout->addWidget(check_follow_camera_direction);

        lightposition = new PositionSpinbox(-10, 10, 0.1, 0.0, "聚焦光源位置控制");
        lightdirection = new PositionSpinbox(-10, 10, 0.1, 0.0, "聚焦光源方向控制");
        lightposition->setEnabled(!follow_camera_position);
        lightdirection->setEnabled(!follow_camera_direction);

        lightcolor = new ColorButton("聚焦光源颜色", QVector3D(255, 255, 255));
        clq_panel = this->create_light_clq_panel(spot_light_panel, clq_level);

        layout->addLayout(hlayout);
        layout->addWidget(lightposition);
        layout->addWidget(lightdirection);
        layout->addWidget(lightcolor);
        layout->addWidget(clq_panel);
    }

    {
        lightposition->setValueX(position.x());
        lightposition->setValueY(position.y());
        lightposition->setValueZ(position.z());
        lightdirection->setValueX(direction.x());
        lightdirection->setValueY(direction.y());
        lightdirection->setValueZ(direction.z());
    }
    {
        connect(check_follow_camera_position, &QCheckBox::stateChanged, this, [this](int state)
                {
            follow_camera_position = state == Qt::Checked;
            lightposition->setEnabled(!state); });
        connect(check_follow_camera_direction, &QCheckBox::stateChanged, this, [this](int state)
                {
            follow_camera_direction = state == Qt::Checked;
            lightdirection->setEnabled(!state); });
        connect(lightcolor, &ColorButton::color_changed, this, &Spot_Light::onLightColorChanged);
        connect(lightposition, &PositionSpinbox::spinbox_valueChanged, this, &Spot_Light::onPositionChanged);
        connect(lightdirection, &PositionSpinbox::spinbox_valueChanged, this, &Spot_Light::onDirectionChanged);
    }

    {
        // QLabel *light_spot_Label = new QLabel("聚光控制:", parent);
        // layout->addWidget(light_spot_Label);

        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->setContentsMargins(2, 2, 2, 2);
        hlayout->setSpacing(1);

        check_soft_spot_light = new QCheckBox("采用柔光", parent);
        check_fix_soft_spot_light = new QCheckBox("柔光限制", parent);
        check_soft_spot_light->setChecked(is_soft_spot_light);
        check_fix_soft_spot_light->setChecked(is_fix_soft_spot_light);
        hlayout->addWidget(check_soft_spot_light);
        hlayout->addWidget(check_fix_soft_spot_light);
        connect(check_fix_soft_spot_light, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state)
                { is_fix_soft_spot_light = state == Qt::Checked; });

        connect(check_soft_spot_light, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state)
                {
                    is_soft_spot_light = state == Qt::Checked;
                    check_fix_soft_spot_light->setEnabled(is_soft_spot_light);
                    light_cutoff_controls[1]->setEnabled(is_soft_spot_light);
                    if(!is_soft_spot_light)
                    {
                        float temp_outercutoff = light_cutoff_controls[0]->getValue();
                        light_cutoff_controls[1]->setValue(temp_outercutoff);
                    } });
        layout->addLayout(hlayout);

        QStringList text = {"cutoff", "outercutoff"};
        std::vector<double> default_value = {10, 15};
        std::vector<double> steps = {1, 1};
        std::vector<std::vector<int>> range = {{1, 90}, {1, 90}}; // 正常specular_strength是0-1，不过开放一下看看效果
        for (int i = 0; i < text.size(); ++i)
        {
            light_cutoff_controls.push_back(new SliderWithLabel(text[i], range[i][0], range[i][1], steps[i], default_value[i], parent, i));

            QSize hintSize = light_cutoff_controls[i]->sizeHint();
            light_cutoff_controls[i]->setMinimumSize(hintSize);
            layout->addWidget(light_cutoff_controls[i]);
            connect(light_cutoff_controls[i], SIGNAL(slider_valueChanged()), this, SLOT(onSlideCutOffChanged()));
        }
    }
    spot_light_panel->adjustSize();
    return spot_light_panel;
}
void Spot_Light::update_Position()
{
    lightposition->setValue(position);
}
void Spot_Light::update_Direction()
{
    lightdirection->setValue(direction);
}
float Spot_Light::get_cutoff()
{
    cutoff = cos(cutoff_angle * 3.1415926 / 180.0);
    return cutoff;
}
float Spot_Light::get_outercutoff()
{
    outercutoff = cos(outercutoff_angle * 3.1415926 / 180.0);
    return outercutoff;
}
void Spot_Light::onLightColorChanged()
{
    QVector3D currentcolor = lightcolor->getColor();
    // qDebug() << "current spot light color is " << currentcolor;
    ambient = 0.1 * currentcolor;
    diffuse = 0.5 * currentcolor;
    specular = 1.0 * currentcolor;
    // qDebug() << "onLightColorChanged ,ambient: " << ambient << "diffuse: " << diffuse << "specular: " << specular;
}
void Spot_Light::onPositionChanged()
{
    double pos_x = lightposition->getValueX();
    double pos_y = lightposition->getValueY();
    double pos_z = lightposition->getValueZ();
    position = QVector3D(pos_x, pos_y, pos_z);
    // qDebug() << "current spot light position is " << position;
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}

void Spot_Light::onDirectionChanged()
{
    double pos_x = lightdirection->getValueX();
    double pos_y = lightdirection->getValueY();
    double pos_z = lightdirection->getValueZ();
    direction = QVector3D(pos_x, pos_y, pos_z);
    // qDebug() << "current spot light dirction is " << direction;
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}

void Spot_Light::onSoftButtonChanged(int state)
{
    if (state == Qt::Checked)
    {
        light_cutoff_controls[1]->enable();
    }
    else if (state == Qt::Unchecked)
    {
        light_cutoff_controls[1]->disable();
    }
    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}
void Spot_Light::onSlideCutOffChanged()
{
    float temp_cutoff = light_cutoff_controls[0]->getValue();
    float temp_outercutoff = light_cutoff_controls[1]->getValue();
    if (is_soft_spot_light && is_fix_soft_spot_light)
    {

        temp_outercutoff = temp_outercutoff < temp_cutoff ? temp_cutoff : temp_outercutoff;
        light_cutoff_controls[1]->setValue(temp_outercutoff); // 这里会触发两次信号槽
    }
    if (!is_soft_spot_light)
    {
        light_cutoff_controls[1]->setValue(temp_cutoff); // 这里会触发两次信号槽
    }
    cutoff_angle = light_cutoff_controls[0]->getValue();
    outercutoff_angle = light_cutoff_controls[1]->getValue();

    // qDebug() << " cutoff_angle is " << light_cutoff_controls[0]->getValue();
    // qDebug() << " outercutoff_angle is " << light_cutoff_controls[1]->getValue();

    cutoff = cos(cutoff_angle * 3.1415926 / 180.0);
    outercutoff = cos(outercutoff_angle * 3.1415926 / 180.0);

    // qDebug() << "real cutoff is " << cutoff;
    // qDebug() << "real outercutoff is " << outercutoff;

    if (Manager::get_instance()->mainWindow->ui->openGLWidget != nullptr)
    {
        Manager::get_instance()->mainWindow->ui->openGLWidget->update();
    }
}
