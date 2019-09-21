#ifndef DSEARCHINFODIALOG_H
#define DSEARCHINFODIALOG_H

#include <QDialog>

#include "ddocinfosqlite.h"

namespace Ui {
class DSearchInfoDialog;
}

// class QSqlQueryModel;
class QStringListModel;
class QSqlQuery;
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

    //! 用query搜索记录填充listModel_
    void fillModelWithQuery(QSqlQuery* query);

private:
    Ui::DSearchInfoDialog *ui;

    DDocInfoSqlite* db_;

    // QSqlQueryModel* queryModel_;
    QStringListModel* listModel_;
};

#endif // DSEARCHINFODIALOG_H
