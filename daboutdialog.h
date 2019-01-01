#ifndef DABOUTDIALOG_H
#define DABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class DAboutDialog;
}

class DAboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DAboutDialog(QWidget *parent = nullptr);
    ~DAboutDialog();

private:
    Ui::DAboutDialog *ui;
};

#endif // DABOUTDIALOG_H
