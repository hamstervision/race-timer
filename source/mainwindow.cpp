#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newcompetitordialog.h"
#include "categoryitemdelegate.h"
#include "competitoritemdelegate.h"
#include "racetimingitemdelegate.h"
#include "raceresultsitemdelegate.h"

#include <QDateTime>
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <set>

static const int CategoryTableNameColumn        = 0;
static const int CategoryTableTimeColumn        = 1;

static const int EntriesTableRaceNumColumn      = 0;
static const int EntriesTableFirstNameColumn    = 1;
static const int EntriesTableSurnameColumn      = 2;
static const int EntriesTableCategoryColumn     = 3;
static const int EntriesTableTINumberColumn     = 4;

static const QString RaceTimerRootTag("raceTimer");
static const QString FileFilter = "Race Timer Data (*.racedata)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_categoriesModel(this)
    , m_competitorsModel(&m_categoriesModel, this)
    , m_raceTimingModel(&m_competitorsModel, this)
    , m_raceResultsModel(&m_competitorsModel, &m_raceTimingModel, this)
    , m_fontAwesome(nullptr)
    , m_isShuttingDown(false)
{
    ui->setupUi(this);

    m_fontAwesome = new QtAwesome(this);
    if (!m_fontAwesome || !m_fontAwesome->initFontAwesome())
    {
        Q_ASSERT(false);
    }

    setCentralWidget(ui->mainTabCtrl);

    ui->entriesTable->setModel(&m_competitorsModel);
    ui->entriesTable->horizontalHeader()->setDefaultSectionSize(200);
    ui->entriesTable->verticalHeader()->hide();
    ui->entriesTable->setItemDelegate(new CompetitorItemDelegate(&m_competitorsModel, &m_categoriesModel, this));

    ui->categoriesTable->setModel(&m_categoriesModel);
    ui->categoriesTable->horizontalHeader()->setDefaultSectionSize(400);
    ui->categoriesTable->verticalHeader()->hide();
    ui->categoriesTable->setItemDelegate(new CategoryItemDelegate(&m_categoriesModel, this));

    ui->addCategory->setIcon(m_fontAwesome->icon(fa::plus));
    ui->removeCategory->setIcon(m_fontAwesome->icon(fa::minus));
    ui->updateStartTime->setIcon(m_fontAwesome->icon(fa::clocko));

    ui->addEntrant->setIcon(m_fontAwesome->icon(fa::plus));

    ui->removeCategory->setEnabled(false);
    ui->updateStartTime->setEnabled(false);

    QFont timingFont("Century Gothic", 20, QFont::Bold);
    int timingFontSize = ui->competitorFinishedButton->height();
    timingFont.setPixelSize(timingFontSize);

    ui->timingLabel->setFont(timingFont);
    ui->competitorFinishedButton->setFont(timingFont);
    ui->competitorFinishedButton->setIcon(m_fontAwesome->icon(fa::clocko));
    ui->competitorFinishedButton->setIconSize(QSize(timingFontSize, timingFontSize));

    timingFont.setPointSize(50);
    ui->raceNumberEdit->setFont(timingFont);

    ui->timingTable->setModel(&m_raceTimingModel);
    ui->timingTable->verticalHeader()->hide();
    ui->timingTable->setItemDelegate(new RaceTimingItemDelegate(&m_raceTimingModel, this));

    ui->resultsTable->setModel(&m_raceResultsModel);
    ui->resultsTable->verticalHeader()->hide();
    ui->resultsTable->setItemDelegate(new RaceResultsItemDelegate(&m_raceResultsModel, this));

    QObject::connect(ui->addCategory, SIGNAL(clicked(bool)), this, SLOT(onAddCategory(bool)));
    QObject::connect(ui->removeCategory, SIGNAL(clicked(bool)), this, SLOT(onRemoveCategory(bool)));
    QObject::connect(ui->updateStartTime, SIGNAL(clicked(bool)), this, SLOT(onUpdateTime(bool)));
    QObject::connect(ui->categoriesTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onCategorySelectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(&m_raceTimingModel, SIGNAL(resultsChanged()), &m_raceResultsModel, SLOT(onResultsChanged()));
    QObject::connect(&m_categoriesModel, SIGNAL(categoriesChanged()), &m_competitorsModel, SLOT(onCategoriesChanged()));
    QObject::connect(&m_categoriesModel, SIGNAL(categoriesChanged()), this, SLOT(onCategoriesChanged()));
    QObject::connect(ui->raceNumberEdit, SIGNAL(valueAccepted()), this, SLOT(on_competitorFinishedButton_clicked()));
    QObject::connect(ui->categoriesTable, SIGNAL(deleteSelected()), this, SLOT(onDeleteSelectedCategory()));
    QObject::connect(ui->entriesTable, SIGNAL(deleteSelected()), this, SLOT(onDeleteSelectedCompetitor()));
    QObject::connect(ui->timingTable, SIGNAL(deleteSelected()), this, SLOT(onDeleteSelectedResult()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFilename(const QString &filename)
{
    m_filename = filename;
}

void MainWindow::onAddCategory(bool /*checked = false*/)
{
    m_categoriesModel.addCategory();
    ui->categoriesTable->resizeRowsToContents();
}

void MainWindow::onRemoveCategory(bool /*checked = false*/)
{
    m_categoriesModel.removeCategory(ui->categoriesTable->currentIndex());
}

void MainWindow::onUpdateTime(bool /*checked = false*/)
{
    QDateTime now(QDateTime::currentDateTimeUtc());
    QModelIndex index(m_categoriesModel.index(ui->categoriesTable->currentIndex().row(), CategoriesModel::StartTimeColumnId));
    m_categoriesModel.setData(index, now);
}

void MainWindow::onCategoriesChanged()
{
    if (m_isShuttingDown)
        return;

    uint currentId = Category::InvalidCategoryId;

    if (ui->resultsCombo->count() > 0)
        currentId = ui->resultsCombo->currentData().toUInt();

    ui->resultsCombo->clear();

    ui->resultsCombo->addItem("-All-", Category::InvalidCategoryId);
    int currentIndex = 0;

    int index = 1;
    std::vector<Category*> categories(m_categoriesModel.categories());
    for (auto category : categories)
    {
        ui->resultsCombo->addItem(category->name(), category->id());
        if (category->id() == currentId)
            currentIndex = index;

        ++index;
    }

    if (!categories.empty())
        ui->resultsCombo->setCurrentIndex(currentIndex);

    m_raceResultsModel.setCategory(m_categoriesModel.category(currentId));
}

void MainWindow::onCategorySelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    bool enable = !selected.indexes().empty();
    ui->removeCategory->setEnabled(enable);
    ui->updateStartTime->setEnabled(enable);
}

void MainWindow::on_addEntrant_clicked()
{
    NewCompetitorDialog *dlg = new NewCompetitorDialog(this);
    dlg->setRaceNumber((int)m_competitorsModel.competitorCount() + 1);
    dlg->setCategories(m_categoriesModel.categories());
    if (dlg->exec() == QDialog::Accepted)
    {
        Competitor *newCompetitor = dlg->newCompetitor();
        if (!newCompetitor)
        {
            Q_ASSERT(false);
            return;
        }

        m_competitorsModel.addCompetitor(dlg->newCompetitor());

        if (dlg->createAnother())
        {
            // Run again (async)
            QMetaObject::invokeMethod(this, "on_addEntrant_clicked", Qt::QueuedConnection);
        }

        ui->entriesTable->resizeRowsToContents();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    if (dirty())
        return;

    m_filename = QFileDialog::getOpenFileName(this, "Open Race Data", QString(), FileFilter);
    if (!m_filename.isEmpty())
    {
        QFile xmlFile(m_filename);
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            Q_ASSERT(false);
            return;
        }

        QDomDocument doc(RaceTimerRootTag);
        if (!doc.setContent(&xmlFile))
        {
            xmlFile.close();
            Q_ASSERT(false);
            return;
        }

        xmlFile.close();

        QDomElement root = doc.documentElement();

        m_categoriesModel.deserialise(root);
        m_competitorsModel.deserialise(root);
        m_raceTimingModel.deserialise(root);

        ui->categoriesTable->resizeRowsToContents();
        ui->entriesTable->resizeRowsToContents();
    }
}

void MainWindow::on_actionSave_triggered()
{
    save(false);
}

bool MainWindow::save(bool forcePrompt)
{
    if (m_filename.isEmpty() || forcePrompt)
    {
        m_filename = QFileDialog::getSaveFileName(this, "Save Race Data", QString(), FileFilter);
        if (m_filename.isEmpty())
            return false;
    }

    QFile xmlFile(m_filename);
    if (!xmlFile.open(QIODevice::WriteOnly))
    {
        Q_ASSERT(false);
        return false;
    }

    QDomDocument doc(RaceTimerRootTag);
    QDomElement root = doc.createElement(RaceTimerRootTag);

    if (!m_categoriesModel.serialise(doc, root)
            || !m_competitorsModel.serialise(doc, root)
            || !m_raceTimingModel.serialise(doc, root))
    {
        return false;
    }

    doc.appendChild(root);

    QTextStream ts(&xmlFile);
    ts << doc.toString();

    xmlFile.close();

    return true;
}

bool MainWindow::dirty()
{
    return (m_categoriesModel.dirty() || m_competitorsModel.dirty() || m_raceTimingModel.dirty())
            && QMessageBox::question(this, "Discard Unsaved Data", "Any unsaved changes will be lost. Continue?") != QMessageBox::Yes;
}

void MainWindow::on_competitorFinishedButton_clicked()
{
    uint raceNumber = Result::UnknownRaceNumber;
    QString numberText(ui->raceNumberEdit->text());
    if (!numberText.isEmpty())
        raceNumber = numberText.toUInt();

    m_raceTimingModel.addResult(raceNumber, QDateTime::currentDateTime());
}

void MainWindow::on_actionAddResult_triggered()
{
    on_competitorFinishedButton_clicked();
}

void MainWindow::on_mainTabCtrl_currentChanged(int index)
{
    Q_UNUSED(index);
    //ui->actionAddResult->setEnabled(ui->mainTabCtrl->currentWidget() == ui->raceTab);
}


void MainWindow::on_resultsCombo_currentIndexChanged(int index)
{
    uint categoryId = ui->resultsCombo->itemData(index).toUInt();
    Category *newCategory = m_categoriesModel.category(categoryId);
    m_raceResultsModel.setCategory(newCategory);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
    m_isShuttingDown = event->isAccepted();
}

void MainWindow::on_actionNew_triggered()
{
    if (dirty())
        return;

    m_categoriesModel.clear();
    m_competitorsModel.clear();
    m_raceTimingModel.clear();
    m_raceResultsModel.clear();
}

void MainWindow::onDeleteSelectedCategory()
{
    if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the selected category/categories?") == QMessageBox::Yes)
    {
        QModelIndexList selectedIndeces(ui->categoriesTable->selectionModel()->selectedIndexes());
        for (auto index : selectedIndeces)
        {
            m_categoriesModel.removeCategory(index);
        }
    }
}

void MainWindow::onDeleteSelectedCompetitor()
{
    if (QMessageBox::question(this, "Confirm Clear", "Are you sure you want to clear the selected competitor(s)?") == QMessageBox::Yes)
    {
        QModelIndexList selectedIndeces(ui->entriesTable->selectionModel()->selectedIndexes());
        for (auto index : selectedIndeces)
        {
            m_competitorsModel.removeRow(index.row());
        }
    }
}

void MainWindow::onDeleteSelectedResult()
{
    if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete the selected result(s)?") == QMessageBox::Yes)
    {
        QModelIndexList selectedIndeces(ui->timingTable->selectionModel()->selectedIndexes());
        for (auto index : selectedIndeces)
        {
            m_raceTimingModel.removeRow(index.row());
        }
    }
}
