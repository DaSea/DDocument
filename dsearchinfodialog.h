#ifndef DSEARCHINFODIALOG_H
#define DSEARCHINFODIALOG_H

#include <QDialog>

#include "ddocinfosqlite.h"

namespace Ui {
class DSearchInfoDialog;
}

// class QSqlQueryModel;
class QStringListModel;
class DSearchInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DSearchInfoDialog(QWidget *parent = nullptr, DDocInfoSqlite* db=nullptr);
    virtual ~DSearchInfoDialog();

    void initQuery();

protected:
    bool eventFilter(QObject*, QEvent *event) override;

private:
    bool handleKeyPress(QObject* obj, QEvent* event);
    void search();

private:
    Ui::DSearchInfoDialog *ui;

    DDocInfoSqlite* db_;

    // QSqlQueryModel* queryModel_;
    QStringListModel* listModel_;
};

#endif // DSEARCHINFODIALOG_H
