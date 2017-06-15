#include "tableview.h"

#include <QKeyEvent>

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{

}

void TableView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        emit deleteSelected();
        return;
    default:
        break;
    }

    QTableView::keyPressEvent(event);
}
