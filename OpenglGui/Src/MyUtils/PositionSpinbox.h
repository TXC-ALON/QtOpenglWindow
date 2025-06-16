#ifndef POSITIONSPINBOX_H
#define POSITIONSPINBOX_H
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QString>
#include <QStringList>
#include <QDoubleSpinBox>
#include <QVector3D>
class PositionSpinbox : public QWidget
{
    Q_OBJECT

public:
    // 构造函数声明
    explicit PositionSpinbox(double minimum, double maximum, double step, double _default_value = 0.0,
                             QString name = "", QWidget *parent = nullptr, QStringList _label_names = {"X", "Y", "Z"});

    // 获取滑块的值
    double getValueX() const;
    double getValueY() const;
    double getValueZ() const;
    bool setValueX(double value);
    bool setValueY(double value);
    bool setValueZ(double value);
    bool setValue(QVector3D value);
    void setEnabled(bool enabled);
signals:
    void spinbox_valueChangedX(double value);
    void spinbox_valueChangedY(double value);
    void spinbox_valueChangedZ(double value);
    void spinbox_valueChanged();
private slots:
    // 滑块值变化时的槽函数
    void onValueChangedX(double value);
    void onValueChangedY(double value);
    void onValueChangedZ(double value);

private:
    QStringList label_names = {"X", "Y", "Z"};
    QList<QLabel *> labels;
    QList<QDoubleSpinBox *> dspinboxs;

    double minimum;
    double maximum;
    double step;
    double default_value;
    bool isEnabled;
};

#endif // POSITIONSPINBOX_H
