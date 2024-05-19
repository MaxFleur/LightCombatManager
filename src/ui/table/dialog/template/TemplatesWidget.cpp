#include "TemplatesWidget.hpp"

#include <QDirIterator>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>

#include "UtilsGeneral.hpp"

TemplatesWidget::TemplatesWidget(QWidget* parent) :
    QWidget(parent)
{
    m_charFileHandler = std::make_unique<CharFileHandler>();

    m_templatesListWidget = new TemplatesListWidget;

    auto* const templatesButtonBox = new QDialogButtonBox;
    auto *const applyTemplateButton = templatesButtonBox->addButton(QDialogButtonBox::Apply);
    auto* const removeTemplatesButton = templatesButtonBox->addButton(tr("Remove"), QDialogButtonBox::ActionRole);
    // Recreate sizes as if it was the main grid layout for creating a new Character
    auto* const layout = new QGridLayout;
    layout->addWidget(m_templatesListWidget, 0, 0, 13, 3);
    layout->addWidget(templatesButtonBox, 13, 1, 1, 1);
    layout->addWidget(removeTemplatesButton, 13, 2, 1, 1);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    connect(applyTemplateButton, &QPushButton::clicked, this, &TemplatesWidget::applyButtonClicked);
    connect(removeTemplatesButton, &QPushButton::clicked, this, &TemplatesWidget::removeButtonClicked);
}


void
TemplatesWidget::loadTemplates()
{
    QDirIterator it(m_charFileHandler->getDirectoryString(), { "*.char" }, QDir::Files);

    while (it.hasNext()) {
        it.next();
        const auto fileName = it.fileName();

        switch (auto const code = m_charFileHandler->getStatus(fileName); code) {
        case 0:
        {
            const auto characterObject = m_charFileHandler->getData();
            CharacterHandler::Character character(characterObject["name"].toString(), characterObject["initiative"].toInt(),
                                                  characterObject["modifier"].toInt(), characterObject["hp"].toInt(),
                                                  characterObject["is_enemy"].toBool(),
                                                  AdditionalInfoData{ {}, characterObject["additional_info"].toString() });
            if (!m_templatesListWidget->addCharacter(character)) {
                Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("The Character is already in the list!"));
            }
            break;
        }
        case 1:
        {
            QMessageBox::critical(this, tr("Wrong Table format!"),
                                  tr("The loading of the Table failed because the Table has the wrong format."));
            break;
        }
        case 2:
        default:
            break;
        }
    }
}


void
TemplatesWidget::addCharacter(const CharacterHandler::Character& character)
{
    if (!m_templatesListWidget->addCharacter(character)) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("The Character is already in the list!"));
    }
    if (!m_charFileHandler->writeToFile(character)) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("The Character could not be saved!"));
    }
}


void
TemplatesWidget::applyButtonClicked()
{
    if (m_templatesListWidget->selectedItems().isEmpty()) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"),
                                                 tr("Please select a Character from the list to apply it to the current Character!"));
        return;
    }

    const auto character = m_templatesListWidget->selectedItems().first()->data(Qt::UserRole).value<CharacterHandler::Character>();
    emit characterLoaded(character);
}


void
TemplatesWidget::removeButtonClicked()
{
    if (m_templatesListWidget->selectedItems().isEmpty()) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"),
                                                 tr("Please select a Character from the list to remove the current Character!"));
        return;
    }

    const auto character = m_templatesListWidget->selectedItems().first()->data(Qt::UserRole).value<CharacterHandler::Character>();
    if (!m_templatesListWidget->removeCharacter(character)) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("Could not remove Character!"));
        return;
    }
    if (!m_charFileHandler->removeCharacter(character.name + ".char")) {
        Utils::General::displayWarningMessageBox(this, tr("Action not possible!"), tr("The Character could not be removed!"));
    }
}
