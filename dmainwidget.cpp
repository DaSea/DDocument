#include "dmainwidget.h"
#include "ui_dmainwidget.h"
#include "dsettingdialog.h"
#include "ddocmentsinfodialog.h"
#include "dsearchinfodialog.h"
#include "daboutdialog.h"

#include <QIcon>
#include <QMenu>
#include <QTableView>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QStandardPaths>

#include <QDebug>

/*****************************************************************************************
 * ReadBookInfoThread function group:读取shape的线程
 *****************************************************************************************/
ReadBookInfoThread::ReadBookInfoThread(QObject* par) : QThread(par) {
}

void ReadBookInfoThread::run() {
    // delete all db item
    // start a thread to fill
    searchDatabase_->clear();
    searchDatabase_->fillSearchIndex(setting_);
}

void ReadBookInfoThread::setter(DDocSetting& setting, DDocInfoSqlite* database) {
    setting_ = setting;
    searchDatabase_ = database;
}

/*******************************************************************************
 * public function group
 *******************************************************************************/
DMainWidget::DMainWidget(QWidget* parent):
    QWidget(parent),
    ui(new Ui::DMainWidget)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Person document manager"));
    this->setWindowIcon(QIcon(":/images/ddocument.svg"));
    this->showMaximized();

    // Set background image
    bgImage_.load(":/images/mainbg.png");
    setAutoFillBackground(true);   // need to set
    QPalette pal(palette());
    QBrush   bgbrush(bgImage_.scaled(QSize(1920, 1080), Qt::KeepAspectRatioByExpanding, Qt::FastTransformation));
    pal.setBrush(QPalette::Window, bgbrush);
    setPalette(pal);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    infoDlg_ = new DDocmentsInfoDialog(this);
    layout->addWidget(infoDlg_);
    ui->widget_main->setLayout(layout);
    connect(infoDlg_, SIGNAL(updateTitle(QString)),
            this, SLOT(onUpdateTitle(QString)));

    ui->pushBtn_home->setIcon(QIcon(":/images/home.png"));
    ui->pushBtn_display->setIcon(QIcon(":/images/display.png"));
    ui->pushBtn_search->setIcon(QIcon(":/images/search.png"));
    ui->pushBtn_toggleview->setIcon(QIcon(":/images/view.png"));
    ui->pushBtn_menu->setIcon(QIcon(":/images/menu.png"));


    QString configpath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    setting_ = DDocSetting(configpath + "/DDocument/config.ini");
    infoDlg_->fillTreeView(setting_);

    QString dbname = configpath + "/DDocument/ddocument.db";
    qDebug()<<configpath<<"-"<<dbname;
    createDocInfoDB(dbname);

    // search dlg
    searchDlg_ = nullptr;
    ui->pushBtn_search->setShortcut(QKeySequence(tr("Ctrl+f", "Search")));
    ui->pushBtn_search->setShortcutEnabled(true);

    currReadThread_ = nullptr;
}

DMainWidget::~DMainWidget()
{
    delete ui;
    // need save setting
    setting_.sync();
    if (nullptr != searchDatabase_) {
        delete searchDatabase_;
        searchDatabase_ = nullptr;
    }
}

void DMainWidget::onUpdateTitle(QString title) {
    ui->label_title->setText(title);
}

void DMainWidget::on_pushBtn_home_clicked()
{}

void DMainWidget::on_pushBtn_display_clicked()
{}

void DMainWidget::on_pushBtn_search_clicked()
{
    // 刷新线程已经存在的话，则不能打开，因为searchDatabase_被占用
    if (nullptr == searchDlg_) {
        searchDlg_ = new DSearchInfoDialog(this, searchDatabase_);
    }
    searchDlg_->initQuery();
    searchDlg_->exec();
}

void DMainWidget::on_pushBtn_toggleview_clicked()
{}

void DMainWidget::on_pushBtn_menu_clicked()
{
    QMenu* menu = new QMenu();
    menu->addAction(QIcon(":/images/setting.png"), tr("Setting"), this, SLOT(settingClicked()));
    menu->addAction(QIcon(":/images/refresh.png"), tr("Refresh db cache"), this, SLOT(refreshDBCache()));
    // menu->addAction(QIcon(":/images/keyshortcut.png"), tr("Keyboard shortcuts"), this, SLOT(showKeyBoard()));
    menu->addAction(QIcon(":/images/about.png"), tr("About"), this, SLOT(aboutClicked()));

    if (nullptr != menu) {
        menu->exec(QCursor::pos());
        delete menu;
    }
}

void DMainWidget::settingClicked() {
    DSettingDialog dialog;
    dialog.setSetting(setting_);
    if (QDialog::Accepted == dialog.exec()) {
        DDocSetting setting = dialog.getSetting();
        // if (setting_ == setting) {
        if (setting_.isSameDocPathList(setting)) {
            qDebug()<<"Has same setting!";
            return ;
        }

        setting_ = setting;
        // need read dir
        infoDlg_->fillTreeView(setting_);
        setting_.sync();

        // start read thread
        refreshDBCache();
    }
}

void DMainWidget::aboutClicked() {
    // QMessageBox::about(this, tr("About person document manager"),
    // tr("This is a person documnet manager!"));
    DAboutDialog aboutDlg(this);
    aboutDlg.exec();
}

void DMainWidget::refreshDBCache() {
#if 0
    // start read thread
    if (nullptr != currReadThread_) {
        if (currReadThread_->isRunning()) {
            QMessageBox::warning(this, tr("Warning"), tr("Read threa is running!"));
            return ;
        }
        currReadThread_->terminate();
    }

    ReadBookInfoThread* readThread = new ReadBookInfoThread(NULL);
    //线程结束后调用deleteLater来销毁分配的内存
    connect(readThread,&QThread::finished ,readThread,&QObject::deleteLater);
    connect(readThread,&QObject::destroyed,this, &DMainWidget::onReadThreadDestroy);
    readThread->setter(setting_, searchDatabase_);
    currReadThread_ = readThread;

    readThread->start();
#endif
    searchDatabase_->clear();
    searchDatabase_->fillSearchIndex(setting_);
}

void DMainWidget::showKeyBoard() {
    // 暂时放在设置的里面，
}

void DMainWidget::onReadThreadDestroy(QObject* obj) {
    if (currReadThread_ == obj) {
        currReadThread_ = nullptr;
    }
}

/*******************************************************************************
 * private function group
 ******************************************************************************/
void DMainWidget::createDocInfoDB(QString& dbname) {
    // first judge whether db is exist
    searchDatabase_ = new DDocInfoSqlite(dbname);
}
