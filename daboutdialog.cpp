#include "daboutdialog.h"
#include "ui_daboutdialog.h"

DAboutDialog::DAboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DAboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("About person document manager"));

    QString strAbout(tr("Ctrl+f: open search dialog \n" \
                        "In search dialog, input string, and press enter to search, \n" \
                        "up and down to select file, ctrl+return to open file. \n" \
                        "In table view, double click to open file, or use right click."));
    ui->plainTextEdit->setPlainText(strAbout);
    ui->plainTextEdit->setEnabled(false);
}

DAboutDialog::~DAboutDialog()
{
    delete ui;
}
