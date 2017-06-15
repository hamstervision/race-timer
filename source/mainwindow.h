#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dropdowndelegate.h"
#include "datedelegate.h"
#include "categoriesmodel.h"
#include "competitorsmodel.h"
#include "racetimingmodel.h"
#include "raceresultsmodel.h"

#include "QtAwesome/QtAwesome.h"

#include <QTableWidgetItem>
#include <QCloseEvent>
#include <map>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setFilename(const QString &filename);

private slots:
    void onAddCategory(bool checked = false);
    void onRemoveCategory(bool checked = false);
    void onUpdateTime(bool checked = false);
    void onCategoriesChanged();

    void onCategorySelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_addEntrant_clicked();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();

    void on_competitorFinishedButton_clicked();

    void on_actionAddResult_triggered();

    void on_mainTabCtrl_currentChanged(int index);

    void on_resultsCombo_currentIndexChanged(int index);

    void on_actionNew_triggered();

    void onDeleteSelectedCategory();
    void onDeleteSelectedCompetitor();
    void onDeleteSelectedResult();

protected: // QWidget
    void closeEvent(QCloseEvent *event);

protected:
    void UpdateCategories();

    bool save(bool forcePrompt);
    bool dirty();

private:
    Ui::MainWindow *ui;

    CompetitorsModel m_competitorsModel;
    CategoriesModel m_categoriesModel;
    RaceTimingModel m_raceTimingModel;
    RaceResultsModel m_raceResultsModel;
    QString m_filename;
    QtAwesome *m_fontAwesome;
    bool m_isShuttingDown;
};

#endif // MAINWINDOW_H
