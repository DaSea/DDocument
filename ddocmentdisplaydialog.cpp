#include "ddocmentdisplaydialog.h"
#include "ui_ddocmentdisplaydialog.h"

#include <QKeyEvent>

DDocmentDisplayDialog::DDocmentDisplayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DDocmentDisplayDialog)
{
    ui->setupUi(this);
}

DDocmentDisplayDialog::~DDocmentDisplayDialog()
{
    delete ui;
}

void DDocmentDisplayDialog::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Escape:
        // 主要是作为tabwidget界面的子tab时，按下esc时，会调用reject()方法，导致不显示
        emit keyPressed(event->key());
        break;

    default:
        QDialog::keyPressEvent(event);
        break;
    }
}

