#include "ddocmentsinfodialog.h"
#include "ui_ddocmentsinfodialog.h"
#include "dinfotableview.h"

#include <QHBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

#define PATH_ROLE (Qt::UserRole+1)

DDocmentsInfoDialog::DDocmentsInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DDocmentsInfoDialog)
{
    ui->setupUi(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    tableView_ = new DInfoTableView(ui->widget_view);
    layout->addWidget(tableView_);
    ui->widget_view->setLayout(layout);
    // connect(tableView_, SIGNAL(updateTitle(QString)), this, SIGNAL(updateTitle(QString)));

    iconFolder_ = QIcon::fromTheme("mefolder", QIcon(":/images/folder.png"));
    iconSubfolder_ = QIcon::fromTheme("subfolder", QIcon(":/images/subfolder.png"));
    treeModel_ = new QStandardItemModel(0, 1, this);
    treeModel_->setHeaderData(0, Qt::Horizontal, tr("Path"));
    ui->treeView_doc->setModel(treeModel_);
}

DDocmentsInfoDialog::~DDocmentsInfoDialog()
{
    delete ui;
}

void DDocmentsInfoDialog::fillTreeView(const DDocSetting& setting) {
    tableView_->setFilter(setting.filter());

    QStringList filters = setting.filter().split(',');
    QStringList folderExclude = setting.folderExclude().split(',');

    // clear tree model
    treeModel_->clear();
    foreach (QString pathitem, setting.docPathList()) {
        int index = pathitem.lastIndexOf('/');
        QString basepath = pathitem.right(pathitem.count()-index-1);
        qDebug()<<"Path base name:"<<basepath;

        QStandardItem* oneparent = new QStandardItem(iconFolder_, basepath);
        oneparent->setData(pathitem, PATH_ROLE);
        oneparent->setToolTip(pathitem);
        oneparent->setEditable(false);
        treeModel_->appendRow(oneparent);

        QDir dir(pathitem);
        foreach (QFileInfo fileinfo, dir.entryInfoList(QDir::Dirs, setting.sortFlag())) {
            QString basename = fileinfo.baseName();
            if (basename.isEmpty())
                continue;

            // if this dir is in exclude list
            if (folderExclude.contains(basename)) {
                continue;
            }

            QString path = fileinfo.absoluteFilePath();
            // if this dir has valid item, add it
            if (!hasValidItem(path, filters)) {
                continue;
            }

            QStandardItem* children = new QStandardItem(iconSubfolder_, basename);
            children->setEditable(false);
            oneparent->appendRow(children);
        }
        //tableView_->initDisplay(pathitem);
    }
    ui->treeView_doc->expandAll();
}

/*******************************************************************************
 * private function group
 ******************************************************************************/
void DDocmentsInfoDialog::on_treeView_doc_clicked(const QModelIndex &index) {
    QStandardItem* item = treeModel_->itemFromIndex(index);
    QString currName = item->text();
    qDebug()<<"on_treeView_doc_clicked-itemname"<<currName;
    if (currName.isEmpty()) {
        return ;
    }

    QStandardItem* parent = item->parent();
    if (nullptr == parent) {
        qDebug()<<"on_treeView_doc_clicked-current item has no parent!";
        tableView_->initDisplay(currName);
        return ;
    }

    QString parentname = parent->data(PATH_ROLE).toString();
    // QString parentname = parent->text();
    qDebug()<<"on_treeView_doc_clicked-parentname:"<<parentname;
    if (parentname.isEmpty()) {
        return ;
    }

    QString fullname = parentname + "/" + currName;
    QFileInfo info(fullname);
    if (!info.isDir()) {
        return ;
    }

    tableView_->initDisplay(fullname);

    // update title
    emit updateTitle(fullname);
}

bool DDocmentsInfoDialog::hasValidItem(const QString& path, QStringList& filters) {
    QDir dir(path);
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    if (dir.count() < 1) {
        return false;
    }

    return true;
}
