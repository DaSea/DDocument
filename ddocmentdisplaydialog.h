#ifndef DDOCMENTDISPLAYDIALOG_H
#define DDOCMENTDISPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class DDocmentDisplayDialog;
}

class DDocmentDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DDocmentDisplayDialog(QWidget *parent = nullptr);
    ~DDocmentDisplayDialog();

private:
    Ui::DDocmentDisplayDialog *ui;
};

#endif // DDOCMENTDISPLAYDIALOG_H
