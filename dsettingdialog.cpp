#include "dsettingdialog.h"
#include "ui_dsettingdialog.h"

#include <QStringListModel>
#include <QFileDialog>
#include <QDebug>

DSettingDialog::DSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DSettingDialog)
{
    ui->setupUi(this);
    ui->pushBtn_addpath->setIcon(QIcon(":/images/add.png"));
    ui->pushBtn_delpath->setIcon(QIcon(":/images/delete.png"));

    ui->lineEdit_filter->setPlaceholderText("*.pdf,*.chm,*.doc");
    ui->lineEdit_folderexclude->setPlaceholderText("docsets,build,lib");
}

DSettingDialog::~DSettingDialog()
{
    delete ui;
}

void DSettingDialog::setSetting(DDocSetting& setting) {
    setting_ = setting;
    // init gui display with setting
    initGuiWithSetting();
}

DDocSetting DSettingDialog::getSetting() const {
    return setting_;
}

void DSettingDialog::on_pushBtn_ok_clicked()
{
    QString filter = ui->lineEdit_filter->text();
    setting_.setFilter(filter);

    QString folder = ui->lineEdit_folderexclude->text();
    setting_.setFolderExclude(folder);
    QDialog::accept();
}

void DSettingDialog::on_pushBtn_cancel_clicked()
{
    QDialog::reject();
}

void DSettingDialog::on_pushBtn_addpath_clicked()
{
    // add path
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                    QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        qDebug()<<"Add path:"<<dir;
        if (setting_.addPath(dir)) {
            pathModel_->setStringList(setting_.docPathList());
        }
        //ui->listView_path->setModel(pathModel_);
    }
}

void DSettingDialog::on_pushBtn_delpath_clicked()
{
    QModelIndex curIndex = ui->listView_path->currentIndex();
    if (!curIndex.isValid()) {
        return ;
    }

    QString selpath = pathModel_->data(curIndex).toString();
    if (setting_.delPath(selpath)) {
        pathModel_->setStringList(setting_.docPathList());
    }
}
/*******************************************************************************
 * private function group
 ******************************************************************************/
void DSettingDialog::initGuiWithSetting() {
    pathModel_ = new QStringListModel(setting_.docPathList());
    ui->listView_path->setModel(pathModel_);

    ui->lineEdit_filter->setText(setting_.filter());
    ui->lineEdit_folderexclude->setText(setting_.folderExclude());
}

