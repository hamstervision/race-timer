#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT

public:
    TableView(QWidget *parent = nullptr);

signals:
    void deleteSelected();

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // TABLEVIEW_H
