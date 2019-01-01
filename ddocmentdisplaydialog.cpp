#include "ddocmentdisplaydialog.h"
#include "ui_ddocmentdisplaydialog.h"

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
