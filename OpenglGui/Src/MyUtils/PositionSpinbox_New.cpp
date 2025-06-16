#include "PositionSpinbox_New.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

PositionSpinbox_New::PositionSpinbox_New(QString _name, QStringList _label_names, std::vector<double> _default_values, std::vector<double> _steps, std::vector<std::vector<double>> _range, QWidget *parent)
    : QWidget(parent), name(_name), label_names(_label_names), default_values(_default_values), steps(_steps), range(_range)
{
    // 检查参数数量是否一致
    if (_label_names.size() != default_values.size() || _label_names.size() != steps.size() || _label_names.size() != range.size())
    {
        qCritical() << "PositionSpinbox_New: 参数数量不匹配。";
        return; // 或者抛出异常
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(1);
    if (!name.isEmpty())
    {
        QLabel *Label = new QLabel(name, this);
        layout->addWidget(Label);
    }

    QHBoxLayout *horizon_layout = new QHBoxLayout();
    horizon_layout->setContentsMargins(1, 1, 1, 1);
    horizon_layout->setSpacing(1);

    for (size_t i = 0; i < label_names.size(); i++)
    {
        QHBoxLayout *h_layout = new QHBoxLayout();
        h_layout->setContentsMargins(1, 1, 1, 1);
        h_layout->setSpacing(1);

        QLabel *label = new QLabel(label_names[i], this);
        QDoubleSpinBox *dspinbox = new QDoubleSpinBox(this);
        dspinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        dspinbox->setMinimumSize(QSize(80, 25)); // 设置最小尺寸
        dspinbox->setMinimum(range[i][0]);
        dspinbox->setMaximum(range[i][1]);
        dspinbox->setSingleStep(steps[i]);
        dspinbox->setValue(default_values[i]);
        dspinbox->setAlignment(Qt::AlignRight); // 设置文本对齐方式为右对齐

        labels.push_back(label);
        dspinboxs.push_back(dspinbox);

        h_layout->addWidget(label);
        h_layout->addWidget(dspinbox);

        horizon_layout->addLayout(h_layout);

        // 动态创建信号和槽的连接
        connect(dspinbox, &QDoubleSpinBox::valueChanged, [this, i](double value)
                { onValueChanged(i, value); });
    }

    layout->addLayout(horizon_layout);
    this->adjustSize();
}

PositionSpinbox_New::~PositionSpinbox_New()
{
}

double PositionSpinbox_New::getValueByIndex(int index) const
{
    if (index >= 0 && index < dspinboxs.size())
    {
        return dspinboxs[index]->value();
    }
    return 0.0; // 或者抛出异常
}

bool PositionSpinbox_New::setValueByIndex(int index, double value)
{
    if (index >= 0 && index < dspinboxs.size())
    {
        // 阻断信号
        dspinboxs[index]->blockSignals(true);

        // 设置值
        dspinboxs[index]->setValue(value);

        // 恢复信号
        dspinboxs[index]->blockSignals(false);
        return true;
    }
    return false;
}

void PositionSpinbox_New::setEnabled(bool enabled)
{
    for (QDoubleSpinBox *spinBox : dspinboxs)
    {
        spinBox->setEnabled(enabled);
    }
}

void PositionSpinbox_New::onValueChanged(int index, double value)
{
    emit spinbox_valueChanged();
    emit spinbox_valueChangedByIndex(index, value);
}
