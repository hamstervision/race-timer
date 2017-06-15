#ifndef NEWCOMPETITORDIALOG_H
#define NEWCOMPETITORDIALOG_H

#include <QDialog>
#include <vector>
#include <map>
#include "competitor.h"

namespace Ui {
class NewCompetitorDialog;
}

class NewCompetitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCompetitorDialog(QWidget *parent = nullptr);
    ~NewCompetitorDialog();

    Competitor* newCompetitor();

    void setRaceNumber(const int raceNumber);
    void setCategories(const std::vector<Category*> &categories);

    bool createAnother() const;

private slots:
    void on_saveAndNextBtn_clicked();

private:
    Ui::NewCompetitorDialog *ui;
    std::map<QString, Category*> m_categories;
    bool m_createAnother;
};

#endif // NEWCOMPETITORDIALOG_H
