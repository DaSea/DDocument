#ifndef DINFOTABLEVIEW_H
#define DINFOTABLEVIEW_H

#include <QTableView>

class QStandardItemModel;
class DInfoTableView : public QTableView
{
    Q_OBJECT

public:
    DInfoTableView(QWidget* parent=nullptr);
    ~DInfoTableView();

    QStandardItemModel* getModel() const { return tableModel_; }

    void initDisplay(const QString& filepath);

    void setFilter(const QString& filter) { filter_ = filter; }

signals:
    void updateTitle(QString title);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void initTableView();

    virtual void mouseDoubleClickEvent(QMouseEvent* event);

private slots:
    void openFile();

private:
    QMenu* rightMenu_;
    QStandardItemModel* tableModel_;
    QString filter_;
};

#endif // DINFOTABLEVIEW_H
