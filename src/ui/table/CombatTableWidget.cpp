#include "CombatTableWidget.hpp"

#include "AdditionalInfoData.hpp"
#include "AdditionalInfoWidget.hpp"
#include "UtilsGeneral.hpp"
#include "UtilsTable.hpp"

#include <QApplication>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLabel>
#include <QObject>
#include <QWidget>

CombatTableWidget::CombatTableWidget(std::shared_ptr<CharacterHandler> characterHandler,
                                     int                               mainWidgetWidth,
                                     QWidget *                         parent) :
    QTableWidget(parent),
    m_characterHandler(characterHandler)
{
    setColumnCount(NMBR_COLUMNS);

    QStringList tableHeader;
    tableHeader << tr("Name") << "INI" << "Mod" << "HP" << tr("Is Enemy") << tr("Additional Information") << "";

    setHorizontalHeaderLabels(tableHeader);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(true);
    verticalHeader()->setSectionsMovable(true);
    verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    setShowGrid(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setFocusPolicy(Qt::ClickFocus);

    setColumnWidth(Utils::Table::COL_NAME, mainWidgetWidth * WIDTH_NAME);
    setColumnWidth(Utils::Table::COL_INI, mainWidgetWidth * WIDTH_INI);
    setColumnWidth(Utils::Table::COL_MODIFIER, mainWidgetWidth * WIDTH_MODIFIER);
    setColumnWidth(Utils::Table::COL_HP, mainWidgetWidth * WIDTH_HP);
    setColumnWidth(Utils::Table::COL_ENEMY, mainWidgetWidth * WIDTH_ENEMY);

    connect(this, &QTableWidget::itemSelectionChanged, this, &CombatTableWidget::adjustAdditionalInfoWidgetPalette);
}


void
CombatTableWidget::resynchronizeCharacters()
{
    m_characterHandler->clearCharacters();

    for (auto i = 0; i < rowCount(); i++) {
        auto *const additionalInfoWidget = cellWidget(i, 5)->findChild<AdditionalInfoWidget *>();

        m_characterHandler->storeCharacter(item(i, Utils::Table::COL_NAME)->text(),
                                           item(i, Utils::Table::COL_INI)->text().toInt(),
                                           item(i, Utils::Table::COL_MODIFIER)->text().toInt(),
                                           item(i, Utils::Table::COL_HP)->text().toInt(),
                                           item(i, Utils::Table::COL_ENEMY)->checkState() == Qt::Checked,
                                           additionalInfoWidget->getAdditionalInformation());
    }
}


void
CombatTableWidget::setRowAndPlayer(QLabel *roundCounterLabel, QLabel *currentPlayerLabel, int rowEntered)
{
    selectionModel()->clearSelection();

    if (rowCount() == 0) {
        currentPlayerLabel->setText(QObject::tr("Current: None"));
        roundCounterLabel->setText(QObject::tr("Round 0"));
        return;
    }

    // Setting fonts may trigger unwished item setting events, so block the signals
    blockSignals(true);
    // Reset bold text rows to standard font
    for (auto i = 0; i < rowCount(); i++) {
        if (auto font = item(i, 0)->font(); font.bold()) {
            for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
                font.setBold(false);
                item(i, j)->setFont(font);
            }
            break;
        }
    }

    // Highlight selected row with bold fonts, but ignore the last columns (widgets)
    auto font = item(rowEntered, 0)->font();
    font.setBold(true);
    for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
        item(rowEntered, j)->setFont(font);
    }
    blockSignals(false);

    currentPlayerLabel->setText(QObject::tr("Current: ") + item(rowEntered, 0)->text());
}


void
CombatTableWidget::setTableRowColor(bool resetColor)
{
    blockSignals(true);

    m_rowsUncolored = resetColor;
    const auto color = [this](bool isEnemy) {
        if (m_rowsUncolored) {
            return QApplication::palette().color(QPalette::Base);
        }
        return isEnemy ? QColor(255, 194, 10, 60) : QColor(12, 123, 220, 60);
    };

    for (auto i = 0; i < rowCount(); i++) {
        const auto cellColor = color(item(i, Utils::Table::COL_ENEMY)->checkState() == Qt::Checked);

        for (auto j = 0; j < FIRST_FIVE_COLUMNS; j++) {
            item(i, j)->setBackground(cellColor);
        }

        QPalette palette;
        palette.setColor(QPalette::Base, cellColor);
        palette.setColor(QPalette::Text, palette.color(QPalette::Text));
        cellWidget(i, Utils::Table::COL_ADDITIONAL)->setAutoFillBackground(!m_rowsUncolored);
        cellWidget(i, Utils::Table::COL_ADDITIONAL)->setPalette(palette);
    }

    blockSignals(false);
}


void
CombatTableWidget::setIniColumnTooltips(bool resetToolTip)
{
    blockSignals(true);

    for (auto i = 0; i < rowCount(); i++) {
        if (resetToolTip) {
            item(i, 1)->setToolTip(QString());
        } else {
            const auto rolledValue = item(i, 1)->text().toInt() - item(i, 2)->text().toInt();
            item(i, 1)->setToolTip("Calculation: Rolled Value " + QString::number(rolledValue) + ", Modifier " + item(i, 2)->text());
        }
    }

    blockSignals(false);
}


void
CombatTableWidget::setStatusEffectInWidget(QVector<AdditionalInfoData::StatusEffect> statusEffects, int row)
{
    auto* const additionalInfoWidget = cellWidget(row, Utils::Table::COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
    additionalInfoWidget->setStatusEffects(statusEffects);
}


void
CombatTableWidget::adjustStatusEffectRoundCounter(bool decrease)
{
    for (auto i = 0; i < rowCount(); i++) {
        auto* const additionalInfoWidget = cellWidget(i, Utils::Table::COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>();
        additionalInfoWidget->adjustEffectDuration(decrease);
    }
}


QVector<QVector<QVariant> >
CombatTableWidget::tableDataFromWidget()
{
    QVector<QVector<QVariant> > tableData;
    for (auto i = 0; i < rowCount(); i++) {
        QVector<QVariant> rowValues;

        for (auto j = 0; j < FIRST_FOUR_COLUMNS; j++) {
            rowValues.push_back(item(i, j)->text());
        }

        rowValues.push_back(item(i, Utils::Table::COL_ENEMY)->checkState() == Qt::Checked);

        QVariant variant;
        variant.setValue(cellWidget(i, Utils::Table::COL_ADDITIONAL)->findChild<AdditionalInfoWidget *>()->getAdditionalInformation());
        rowValues.push_back(variant);

        tableData.push_back(rowValues);
    }

    return tableData;
}


QVector<QVector<QVariant> >
CombatTableWidget::tableDataFromCharacterVector()
{
    QVector<QVector<QVariant> > tableData;
    for (const auto& character : m_characterHandler->getCharacters()) {
        QVector<QVariant> charValues;

        QVariant additionalInfoVariant;
        additionalInfoVariant.setValue(character.additionalInfoData);
        charValues << character.name << character.initiative << character.modifier
                   << character.hp << character.isEnemy << additionalInfoVariant;

        tableData.push_back(charValues);
    }

    return tableData;
}


unsigned int
CombatTableWidget::getHeight() const
{
    auto height = 0;
    for (int i = 0; i < rowCount(); i++) {
        height += rowHeight(i);
    }
    return height + TABLE_HEIGHT_BUFFER;
}


void
CombatTableWidget::keyPressEvent(QKeyEvent *event)
{
    // Ignore arrow keys
    if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
        event->ignore();
        return;
    }
    QTableWidget::keyPressEvent(event);
}


void
CombatTableWidget::adjustAdditionalInfoWidgetPalette()
{
    // If the table rows are colored and a row is selected, the selection color will mix
    // with the cell widget color because of the non-maximum alpha values.
    // So we set the alpha value for selected rows to 0 to circumvent this problem
    if (m_rowsUncolored) {
        return;
    }
    // Reset all colors because other rows might have been selected before
    setTableRowColor(m_rowsUncolored);

    for (const auto selectedRows = selectionModel()->selectedRows(); const auto& selectedModel : selectedRows) {
        const auto row = selectedModel.row();

        auto palette = cellWidget(row, Utils::Table::COL_ADDITIONAL)->palette();
        auto baseColor = palette.color(QPalette::Base);
        auto highlightedTextColor = palette.color(QPalette::HighlightedText);

        baseColor.setAlpha(0);
        palette.setColor(QPalette::Base, baseColor);
        palette.setColor(QPalette::Text, highlightedTextColor);
        cellWidget(row, Utils::Table::COL_ADDITIONAL)->setPalette(palette);
    }
}
