#ifndef DSETTINGDIALOG_H
#define DSETTINGDIALOG_H

#include <QDialog>
#include "ddocsetting.h"

namespace Ui {
class DSettingDialog;
}

class QStringListModel;
class DSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DSettingDialog(QWidget *parent = nullptr);
    ~DSettingDialog();

public:
    void setSetting(DDocSetting& setting);
    DDocSetting getSetting() const;

private slots:
    void on_pushBtn_ok_clicked();
    void on_pushBtn_cancel_clicked();

    void on_pushBtn_addpath_clicked();

    void on_pushBtn_delpath_clicked();

private:
    void initGuiWithSetting();

private:
    Ui::DSettingDialog *ui;
    QStringListModel* pathModel_;
    DDocSetting setting_;
};

#endif // DSETTINGDIALOG_H
