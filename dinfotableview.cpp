#include "dinfotableview.h"

#include <QMenu>
#include <QContextMenuEvent>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHeaderView>
#include <QDesktopServices>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

DInfoTableView::DInfoTableView(QWidget* parent):QTableView (parent),
    filter_("*.pdf,*.doc,*.chm"){
    initTableView();
}

DInfoTableView::~DInfoTableView() {

}

void DInfoTableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!rightMenu_->actions().isEmpty() )
    {
        rightMenu_->popup(event->globalPos());
    }
}

void DInfoTableView::mouseDoubleClickEvent(QMouseEvent* event) {
    // int hitRow = this->rowAt(event->pos().y());
    int hitRow = this->currentIndex().row();
    if (-1 == hitRow) {
        qDebug()<<"DInfoTableView::mouseDoubleClickEvent-Invalid hit pos-y coordinate!";
        return ;
    }

    QStandardItem* currentItem  = tableModel_->item(hitRow, 1);
    qDebug()<<"DInfoTableView::mouseDoubleClickEvent:"<<currentItem->text();
    QString fullname = currentItem->data(Qt::DisplayRole).toString();
    qDebug()<<"DInfoTableView::openFile - sel file name:"<<fullname;

    QDesktopServices::openUrl(QUrl::fromLocalFile(fullname));
    // emit updateTitle(fullname);
}

void DInfoTableView::initTableView() {
    this->setGridStyle(Qt::SolidLine);

    // 设置表头
    tableModel_ = new QStandardItemModel(this);
    tableModel_->setColumnCount(2);
    tableModel_->setHeaderData(0, Qt::Horizontal, tr("Name"));
    tableModel_->setHeaderData(1, Qt::Horizontal, tr("Path"));
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->horizontalHeader()->setStretchLastSection(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows); // 选中整行
    this->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection); // 只能单选
    this->setEditTriggers(QTableView::EditTrigger::NoEditTriggers);
    this->setAlternatingRowColors(true);
    this->setModel(tableModel_);

    // 需要添加右键菜单
    rightMenu_ = new QMenu( this );
    // mShowAdd  = new QAction(tr("Length Or Area Field"),rightMenu_);
    // mShowAdd->setCheckable(true);
    // mShowAdd->setChecked(false);
    // connect( mShowAdd ,SIGNAL(triggered(bool)),this,SLOT(showAddFields()));
    rightMenu_->addAction(tr("Open File"),this,SLOT(openFile()));
    // rightMenu_->addAction(tr("Open"), this, SLOT(selectAll()));
    // rightMenu_->addSeparator();
    // rightMenu_->addAction(mShowAdd);
}

void DInfoTableView::openFile() {
    // get current select file and open it with external programmer
    int hitRow = this->currentIndex().row();
    if (-1 == hitRow) {
        qDebug()<<"DInfoTableView::openfile - Invalid row!";
        return ;
    }

    QStandardItem* currentItem  = tableModel_->item(hitRow, 1);
    QString fullname = currentItem->data(Qt::DisplayRole).toString();
    qDebug()<<"DInfoTableView::openFile - sel file name:"<<fullname;

    QDesktopServices::openUrl(QUrl::fromLocalFile(fullname));
}

void DInfoTableView::initDisplay(const QString& filepath) {
    QDir dir(filepath);
    // filter from setting
    QStringList filters = filter_.split(',');
    // filters << "*.pdf" << "*.chm" << "*.doc";

    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    if (dir.count() < 1) {
        return ;
    }

    int row = 0;
    QFileInfoList filelist = dir.entryInfoList();
    tableModel_->removeRows(0, tableModel_->rowCount());
    foreach (QFileInfo fileinfo, filelist) {
        QStandardItem *item = new QStandardItem();
        item->setText(fileinfo.baseName());
        tableModel_->setItem(row, 0, item);
        tableModel_->setItem(row, 1, new QStandardItem(fileinfo.absoluteFilePath()));
        ++row;
    }
}
