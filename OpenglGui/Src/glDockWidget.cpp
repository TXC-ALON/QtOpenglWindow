#include "glDockWidget.h"
#include "Manager.h"
glDockWidget::glDockWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::glDockWidget)
{
    ui->setupUi(this);
}

glDockWidget::~glDockWidget()
{
}
void glDockWidget::create_tool_example(QWidget *widget)
{
    QWidget *panel = new QWidget();
    int index = ui->toolBox->indexOf(ui->tool_example);
    // std::cout << "ui->toolBox is " << index << std::endl;
    ui->toolBox->removeItem(index);
    delete ui->tool_example;
    ui->tool_example = nullptr;
    QVBoxLayout *layout = new QVBoxLayout(panel);
    example_buttoms.clear();

    for (int i = 0; i < button_list.size(); i++)
    {
        example_buttoms.push_back(new QPushButton(ui->tool_example));
        example_buttoms[i]->setObjectName(button_list[i]);
        example_buttoms[i]->setText(QCoreApplication::translate(DockWidgetName.toUtf8().constData(), button_name_list[i].toUtf8().constData(), nullptr));
        layout->addWidget(example_buttoms[i]);
    }
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    layout->addItem(verticalSpacer);
    ui->tool_example = panel;
    if (ui->tool_example)
    {
        ui->toolBox->insertItem(0, ui->tool_example, QString::fromUtf8("示例"));
    }
    ui->toolBox->setCurrentIndex(0);
}

void glDockWidget::refresh_tool_control(GLWidget *widget)
{
    QWidget *newControl = widget->createControlPanel();

    if (newControl) // 如果新的控制面板创建成功
    {
        // qDebug() << "newControl Size before adjustSize():" << newControl->size();
        int index = ui->toolBox->indexOf(ui->tool_control);
        if (index != -1) // 确保ui->tool_control已经在toolBox中
        {
            ui->toolBox->removeItem(index);
            ui->tool_control->deleteLater();
        }
        // newControl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        ui->tool_control = newControl; // 更新ui->tool_control为新的控件
        // qDebug() << "1 ui->tool_control Size before adjustSize():" << ui->tool_control->size();

        ui->toolBox->insertItem(1, ui->tool_control, QString::fromUtf8("\347\244\272\344\276\213\345\217\202\346\225\260"));
        ui->toolBox->setCurrentIndex(1);
        // ui->tool_control->setFixedWidth(ui->tool_control->width() + 20);
        // qDebug() << "2 ui->tool_control Size before adjustSize():" << ui->tool_control->size();
        // qDebug() << "2 ui->toolBox Size before adjustSize():" << ui->toolBox->size();
        // qDebug() << "3 ui->toolBox Size before adjustSize():" << ui->toolBox->size();

        Manager::get_instance()->mainWindow->adjustSize();
        ui->toolBox->setFixedWidth(ui->toolBox->width() + 20);

        // qDebug() << "4 ui->tool_control Size  adjustSize():" << ui->tool_control->size();
        // qDebug() << "4 ui->toolBox Size  adjustSize():" << ui->toolBox->size();
    }
    else
    {
        qDebug() << DockWidgetName << " refresh_tool_control failed!";
    }
}
