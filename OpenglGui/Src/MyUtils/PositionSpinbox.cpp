#include "PositionSpinbox.h"
#include <QVBoxLayout>

PositionSpinbox::PositionSpinbox(double minimum, double maximum, double step, double _default_value, QString name, QWidget *parent, QStringList _label_names)
    : QWidget(parent), minimum(minimum), maximum(maximum), step(step), default_value(_default_value), label_names(_label_names)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(1);
    if (name != "")
    {
        // QString titie = "\n" + name;
        QString titie = name;
        QLabel *Label = new QLabel(titie, this);
        // Label->setStyleSheet("QLabel { font-weight: bold; }");
        layout->addWidget(Label);
    }
    bool hlayout = true;
    if (hlayout)
    {
        QHBoxLayout *horizon_layout = new QHBoxLayout();
        horizon_layout->setContentsMargins(1, 1, 1, 1);
        horizon_layout->setSpacing(1);
        for (int i = 0; i < label_names.size(); i++)
        {
            QHBoxLayout *h_layout = new QHBoxLayout();
            layout->setContentsMargins(1, 1, 1, 1);
            h_layout->setSpacing(1);
            QLabel *label = new QLabel(label_names[i], this);
            QDoubleSpinBox *dspinbox = new QDoubleSpinBox(this);
            dspinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
            dspinbox->setMinimumSize(QSize(80, 25)); // 设置最小尺寸
            dspinbox->setMinimum(minimum);
            dspinbox->setMaximum(maximum);
            dspinbox->setSingleStep(step);
            dspinbox->setValue(_default_value);
            dspinbox->setAlignment(Qt::AlignRight); // 设置文本对齐方式为右对齐

            labels.push_back(label);
            dspinboxs.push_back(dspinbox);

            h_layout->addWidget(label);
            h_layout->addWidget(dspinbox);

            horizon_layout->addLayout(h_layout);
        }
        layout->addLayout(horizon_layout);
    }
    else
    {
        for (int i = 0; i < label_names.size(); i++)
        {
            QHBoxLayout *h_layout = new QHBoxLayout();

            QLabel *label = new QLabel(label_names[i], this);
            QDoubleSpinBox *dspinbox = new QDoubleSpinBox(this);
            dspinbox->setMinimum(minimum);
            dspinbox->setMaximum(maximum);
            dspinbox->setSingleStep(step);
            dspinbox->setValue(_default_value);
            dspinbox->setAlignment(Qt::AlignRight); // 设置文本对齐方式为右对齐

            labels.push_back(label);
            dspinboxs.push_back(dspinbox);

            h_layout->addWidget(label);
            h_layout->addWidget(dspinbox);

            layout->addLayout(h_layout);
        }
    }
    this->adjustSize();

    // 连接信号和槽
    connect(dspinboxs[0], SIGNAL(valueChanged(double)), this, SLOT(onValueChangedX(double)));
    connect(dspinboxs[1], SIGNAL(valueChanged(double)), this, SLOT(onValueChangedY(double)));
    connect(dspinboxs[2], SIGNAL(valueChanged(double)), this, SLOT(onValueChangedZ(double)));
}

double PositionSpinbox::getValueX() const
{
    return dspinboxs[0]->value();
}

double PositionSpinbox::getValueY() const
{
    return dspinboxs[1]->value();
}

double PositionSpinbox::getValueZ() const
{
    return dspinboxs[2]->value();
}

bool PositionSpinbox::setValueX(double value)
{
    if (value >= minimum && value <= maximum)
    {
        dspinboxs[0]->setValue(value);
        return true;
    }
    return false;
}

bool PositionSpinbox::setValueY(double value)
{
    if (value >= minimum && value <= maximum)
    {
        dspinboxs[1]->setValue(value);
        return true;
    }
    return false;
}

bool PositionSpinbox::setValueZ(double value)
{
    if (value >= minimum && value <= maximum)
    {
        dspinboxs[2]->setValue(value);
        return true;
    }
    return false;
}

bool PositionSpinbox::setValue(QVector3D value)
{
    return setValueX(value.x()) && setValueY(value.y()) && setValueZ(value.z());
}

void PositionSpinbox::setEnabled(bool enabled)
{
    // qDebug() << "enabled is " << enabled;
    isEnabled = enabled;
    QString styleSheet = enabled ? "" : "background-color: #D3D3D3; color: #808080;"; // 灰色背景和文字

    for (QDoubleSpinBox *spinBox : dspinboxs)
    {
        spinBox->setEnabled(enabled);
        spinBox->setStyleSheet(styleSheet); // 设置样式表
    }
    // for (QLabel *label : labels)
    // {
    //     label->setEnabled(enabled);
    //     label->setStyleSheet(styleSheet); // 设置样式表
    // }
}

void PositionSpinbox::onValueChangedX(double value)
{
    emit spinbox_valueChanged();
    emit spinbox_valueChangedX(value);
}

void PositionSpinbox::onValueChangedY(double value)
{
    emit spinbox_valueChanged();
    emit spinbox_valueChangedY(value);
}

void PositionSpinbox::onValueChangedZ(double value)
{
    emit spinbox_valueChanged();
    emit spinbox_valueChangedZ(value);
}
