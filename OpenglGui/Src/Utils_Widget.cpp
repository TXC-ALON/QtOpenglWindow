#include "Utils_Widget.h"

QWidget *createButtomsControl(QWidget *parent, QStringList button_list, QStringList button_name_list, QList<QPushButton *> &buttonReferences)
{
    QWidget *buttomscontrol = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(buttomscontrol);
    layout->setContentsMargins(5, 5, 5, 5); // 设置边距为5像素
    layout->setSpacing(3);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    buttomscontrol->setLayout(layout); // 确保设置布局
    for (int i = 0; i < button_list.size(); i++)
    {

        buttonReferences.push_back(new QPushButton(buttomscontrol));
        QSize minSize(60, 30);
        QSize hintSize = buttonReferences[i]->sizeHint();
        buttonReferences[i]->setMinimumSize(qMax(minSize.width(), hintSize.width()),
                                            qMax(minSize.height(), hintSize.height()));
        buttonReferences[i]->setObjectName(button_list[i]);
        buttonReferences[i]->setText(button_name_list[i]);
        layout->addWidget(buttonReferences[i]);
    }
    buttomscontrol->adjustSize();
    layout->addItem(verticalSpacer);
    buttomscontrol->setGeometry(10, 10, buttomscontrol->width(), buttomscontrol->height());
    return buttomscontrol;
}
