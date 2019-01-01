#ifndef DDOCMENTSINFODIALOG_H
#define DDOCMENTSINFODIALOG_H

#include <QDialog>
#include <QIcon>

#include "ddocsetting.h"

namespace Ui {
class DDocmentsInfoDialog;
}

class DInfoTableView;
class QStandardItemModel;
class QStandardItem;
class QTableView;
class QMenu;
class DDocmentsInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DDocmentsInfoDialog(QWidget *parent = nullptr);
    ~DDocmentsInfoDialog();

    void fillTreeView(const DDocSetting& setting);

signals:
    void updateTitle(QString title);

private slots:
    void on_treeView_doc_clicked(const QModelIndex &index);

private:
    bool hasValidItem(const QString& path, QStringList& filters);

private:
    Ui::DDocmentsInfoDialog *ui;

    QStandardItemModel* treeModel_;
    DInfoTableView* tableView_;

    // folder icon
    QIcon iconFolder_;
    QIcon iconSubfolder_;

    // open select file
    QMenu* rightMenu_;
};

#endif // DDOCMENTSINFODIALOG_H
