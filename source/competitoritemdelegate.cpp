#include "competitoritemdelegate.h"

#include <QLineEdit>
#include <QComboBox>

CompetitorItemDelegate::CompetitorItemDelegate(CompetitorsModel *competitorsModel, CategoriesModel *categoriesModel, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_competitorsModel(competitorsModel)
    , m_categoriesModel(categoriesModel)
{
    Q_ASSERT(competitorsModel);
    Q_ASSERT(categoriesModel);
}

QWidget *CompetitorItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case CompetitorsModel::RaceNumberColumn:
        break;
    case CompetitorsModel::FirstNameColumn:
    case CompetitorsModel::LastNameColumn:
    case CompetitorsModel::MembershipIdColumn:
        return new QLineEdit(parent);
    case CompetitorsModel::CategoryColumn:
        {
            QComboBox *combo = new QComboBox(parent);
            combo->setEditable(false);
            return combo;
        }
    default:
        break;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CompetitorItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
    case CompetitorsModel::RaceNumberColumn:
        return; // Not editable
    case CompetitorsModel::FirstNameColumn:
    case CompetitorsModel::LastNameColumn:
    case CompetitorsModel::MembershipIdColumn:
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
            if (lineEdit)
                lineEdit->setText(m_competitorsModel->data(index).toString());
        }
        return;
    case CompetitorsModel::CategoryColumn:
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            if (!combo)
            {
                Q_ASSERT(false);
                break;
            }

            auto categories = m_categoriesModel->categories();
            for (auto category : categories)
            {
                combo->addItem(category->name(), category->id());
            }

            combo->setCurrentText(m_competitorsModel->data(index).toString());
        }
        return;
    default:
        break;
    }
    QStyledItemDelegate::setEditorData(editor, index);
}

void CompetitorItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_ASSERT(model == m_competitorsModel);

    switch (index.column())
    {
    case CompetitorsModel::RaceNumberColumn:
        break; // Not editable
    case CompetitorsModel::FirstNameColumn:
    case CompetitorsModel::LastNameColumn:
    case CompetitorsModel::MembershipIdColumn:
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
            if (lineEdit)
                m_competitorsModel->setData(index, lineEdit->text());
        }
        return;
    case CompetitorsModel::CategoryColumn:
        {
            QComboBox *combo = qobject_cast<QComboBox*>(editor);
            if (combo)
                m_competitorsModel->setData(index, combo->currentText());
        }
        return;
    default:
        break;
    }
    QStyledItemDelegate::setEditorData(editor, index);
}

void CompetitorItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
