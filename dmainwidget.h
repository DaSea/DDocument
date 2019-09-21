#ifndef DMAINWIDGET_H
#define DMAINWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QThread>

#include "ddocsetting.h"
#include "ddocmentsinfodialog.h"
#include "ddocmentdisplaydialog.h"
#include "ddocinfosqlite.h"
#include "dsearchinfodialog.h"

namespace Ui {
class DMainWidget;
}


/**
 * @brief Put book info to db
 */
class ReadBookInfoThread:public QThread {
    Q_OBJECT

public:
    ReadBookInfoThread(QObject* par);
    void run() Q_DECL_OVERRIDE;

public:
    void setter(DDocSetting& setting, DDocInfoSqlite* database);

private:
    DDocSetting setting_;
    DDocInfoSqlite* searchDatabase_;
};

class QStandardItemModel;
class DMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DMainWidget(QWidget *parent = nullptr);
    ~DMainWidget();

public slots:
    void onUpdateTitle(QString title);

private slots:
    void on_pushBtn_home_clicked();
    void on_pushBtn_display_clicked();
    void on_pushBtn_search_clicked();
    void on_pushBtn_toggleview_clicked();
    void on_pushBtn_menu_clicked();

    // menu
    void settingClicked();
    void aboutClicked();
    void refreshDBCache();
    void showKeyBoard();

    void onReadThreadDestroy(QObject* obj);

    void onKeyPressed(int key);

private:
    void createDocInfoDB(QString& dbname);

private:
    Ui::DMainWidget *ui;
    DDocmentsInfoDialog* infoDlg_;
    DDocmentDisplayDialog* displayDlg_;
    DSearchInfoDialog* searchDlg_;
    // Background image
    QImage bgImage_;
    // setting
    DDocSetting setting_;
    // database
    DDocInfoSqlite* searchDatabase_;

    // Current running thread
    ReadBookInfoThread* currReadThread_;
};

#endif // DMAINWIDGET_H
