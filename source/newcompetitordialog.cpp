#include "newcompetitordialog.h"
#include "ui_newcompetitordialog.h"
#include <climits>

NewCompetitorDialog::NewCompetitorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewCompetitorDialog)
    , m_createAnother(false)
{
    ui->setupUi(this);
    ui->raceNumberSpin->setRange(1, INT_MAX);
}

NewCompetitorDialog::~NewCompetitorDialog()
{
    delete ui;
}

Competitor* NewCompetitorDialog::newCompetitor()
{
    Competitor *competitor = new Competitor();
    competitor->setRaceNumber(ui->raceNumberSpin->value());
    competitor->setFirstName(ui->firstNameEdit->text());
    competitor->setLastName(ui->lastNameEdit->text());

    auto it = m_categories.find(ui->categoryCombo->currentText());
    if (it == m_categories.end())
    {
        Q_ASSERT(false);
        delete competitor;
        return nullptr;
    }

    competitor->setCategory(it->second);
    competitor->setMembershipId(ui->membershipIdEdit->text());
    return competitor;
}

void NewCompetitorDialog::setRaceNumber(const int raceNumber)
{
    ui->raceNumberSpin->setValue(raceNumber);
}

void NewCompetitorDialog::setCategories(const std::vector<Category *> &categories)
{
    ui->categoryCombo->clear();
    m_categories.clear();

    for (auto category : categories)
    {
        QString name(category->name());

        // Shouldn't be duplicate category names, but handle it just in case...
        if (m_categories.find(name) != m_categories.end())
        {
            Q_ASSERT(false);
            int id = 1;
            bool inserted = false;
            while(!inserted)
            {
                QString uniqueName = QString("%1(%2)").arg(name).arg(QString::number(id));
                if (m_categories.find(uniqueName) == m_categories.end())
                {
                    name = uniqueName;
                    inserted = true;
                }
                else
                    ++id;
            }
        }

        m_categories[name] = category;
        ui->categoryCombo->addItem(name);
    }
}

bool NewCompetitorDialog::createAnother() const
{
    return m_createAnother;
}

void NewCompetitorDialog::on_saveAndNextBtn_clicked()
{
    m_createAnother = true;
    accept();
}
