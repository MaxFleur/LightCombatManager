#include "AddCharacterDialog.hpp"


#include "UtilsGeneral.hpp"

#include "templates/TemplatesListWidget.hpp"

#include <QCheckBox>

#include <QDialogButtonBox>
#include <QGraphicsEffect>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QShortcut>
#include <QSpinBox>
#include <QTimer>
#include <QDirIterator>

class TemplatesListWidget;

AddCharacterDialog::AddCharacterDialog(QWidget *parent) :
    QDialog(parent)
{
    m_fileHandler = std::make_unique<CharFileHandler>();
    setWindowTitle(tr("Add new Character(s)"));

    auto *const nameLabel = new QLabel(tr("Name:"));
    m_nameEdit = new QLineEdit;
    m_nameEdit->setToolTip(tr("Set the name of the Character.\n"
                              "A Character can't be stored without a name."));

    auto *const iniLabel = new QLabel(tr("Initiative:"));
    m_iniBox = new QSpinBox;
    m_iniBox->setMinimum(-20);
    m_iniBox->setToolTip(tr("Set the initiative, including all modifiers."));

    auto *const randomIniButton = new QPushButton(tr("Random Initiative Value"));
    randomIniButton->setToolTip(tr("Roll a 20-sided dice to set the initiative."));

    auto *const iniModifierLabel = new QLabel(tr("Modifier:"));
    m_iniModifierBox = new QSpinBox;
    m_iniModifierBox->setMinimum(-10);
    m_iniModifierBox->setToolTip(tr("Set the initiative modifier."));

    m_rolledValueLabel = new QLabel;

    auto *const hpLabel = new QLabel(tr("Health Points:"));
    m_hpBox = new QSpinBox;
    m_hpBox->setRange(-10000, 10000);
    m_hpBox->setToolTip(tr("Set the Character's HP."));

    auto *const enemyLabel = new QLabel(tr("Is Enemy:"));
    m_enemyBox = new QCheckBox;
    m_enemyBox->setToolTip(tr("Set if the Character is an enemy."));

    auto *const addInfoLabel = new QLabel(tr("Additional Info:"));
    m_addInfoEdit = new QLineEdit;
    m_addInfoEdit->setToolTip(tr("Set additional information. \n"
                                 "Status Effects are added in the main table widget."));

    m_instanceNumberBox = new QSpinBox;
    m_instanceNumberBox->setRange(2, 10);
    m_instanceNumberBox->setEnabled(false);

    m_multipleEnabledBox = new QCheckBox(tr("Add Character multiple Times:"));
    m_multipleEnabledBox->setTristate(false);
    m_multipleEnabledBox->setCheckState(Qt::Unchecked);
    m_multipleEnabledBox->setToolTip(tr("If this is selected and 'Save' is pressed,\n"
                                        "the Character is added multiple times to the Table."));

    auto *const buttonBox = new QDialogButtonBox;
    m_templatesButtonBox = new QDialogButtonBox;

    auto *const openTemplatesButton = new QPushButton(tr("Templates"));

    m_storeTemplatesButton =new QPushButton(tr("Store"));

    auto *const saveButton = buttonBox->addButton(QDialogButtonBox::Save);

    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    m_hideTemplatesButton = new QPushButton(tr("<<"));
    auto *const applyTemplateButton = m_templatesButtonBox->addButton(QDialogButtonBox::Apply);
    m_removeTemplatesButton = new QPushButton(tr("Remove"));

    auto *const resetButton = new QPushButton(tr("Reset all entered Values"));

    okButton->setShortcut(Qt::Key_Return);

    saveButton->setShortcut(QKeySequence::Save);
    const auto saveShortcutText = "Save this Character (" + QKeySequence(QKeySequence::Save).toString() + ").";
    saveButton->setToolTip(tr(saveShortcutText.toLocal8Bit().constData()));

    m_templatesListWidget = new TemplatesListWidget(this);

    m_storeTemplatesButton->hide();
    m_templatesListWidget->hide();
    m_templatesButtonBox->hide();
    m_hideTemplatesButton->hide();
    m_removeTemplatesButton->hide();

    m_animatedLabel = new QLabel;
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);

    auto *const layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(m_nameEdit, 0, 1, 1, 3);
    layout->addWidget(m_templatesListWidget, 0, 4, 13, 4);

    layout->setRowMinimumHeight(1, 12);

    layout->addWidget(iniLabel, 2, 0);
    layout->addWidget(m_iniBox, 2, 1);
    layout->addWidget(iniModifierLabel, 2, 2);
    layout->addWidget(m_iniModifierBox, 2, 3);

    layout->addWidget(randomIniButton, 3, 0, 1, 2);
    layout->addWidget(m_rolledValueLabel, 3, 3);

    layout->setRowMinimumHeight(4, 12);

    layout->addWidget(hpLabel, 5, 0);
    layout->addWidget(m_hpBox, 5, 1);
    layout->addWidget(enemyLabel, 5, 2);
    layout->addWidget(m_enemyBox, 5, 3);

    layout->addWidget(addInfoLabel, 6, 0);
    layout->addWidget(m_addInfoEdit, 6, 1, 1, 3);

    layout->setRowMinimumHeight(7, 12);

    layout->addWidget(m_multipleEnabledBox, 8, 0, 1, 3);
    layout->addWidget(m_instanceNumberBox, 8, 3, 1, 1);

    layout->setRowMinimumHeight(9, 12);

    layout->addWidget(m_animatedLabel, 10, 0, 1, 2);

    layout->addWidget(resetButton, 11, 2, 1, 2);
    layout ->addWidget(m_storeTemplatesButton, 11, 0, 1, 1);

    layout->setRowMinimumHeight(12, 12);

    layout->addWidget(openTemplatesButton, 13, 0, 1, 1);
    layout->addWidget(buttonBox, 13, 1, 1, 3);

    layout->addWidget(m_hideTemplatesButton, 13, 4, 1, 1);
    layout->addWidget(m_templatesButtonBox, 13, 5, 1, 2);

    layout->addWidget(m_removeTemplatesButton,13, 7, 1, 1 );
    setLayout(layout);
    m_nameEdit->setFocus(Qt::TabFocusReason);

    connect(randomIniButton, &QPushButton::clicked, this, &AddCharacterDialog::randomButtonClicked);
    connect(m_iniBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] {
        m_iniWithoutModValue = m_iniBox->value() - m_iniModifierBox->value();
    });
    connect(m_iniModifierBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] {
        m_iniBox->setValue(m_iniWithoutModValue + m_iniModifierBox->value());
    });

    connect(m_storeTemplatesButton, &QPushButton::clicked, this, &AddCharacterDialog::storeTemplatesButtonClicked);

    connect(saveButton, &QPushButton::clicked, this, &AddCharacterDialog::saveButtonClicked);
    connect(resetButton, &QPushButton::clicked, this, &AddCharacterDialog::resetButtonClicked);
    connect(okButton, &QPushButton::clicked, this, &AddCharacterDialog::okButtonClicked);

    connect(openTemplatesButton, &QPushButton::clicked, this, &AddCharacterDialog::showTemplatesButtonClicked);
    connect(m_hideTemplatesButton, &QPushButton::clicked, this, &AddCharacterDialog::hideTemplatesButtonClicked);

    connect(applyTemplateButton, &QPushButton::clicked, this, &AddCharacterDialog::applyTemplateButtonClicked);
    connect(m_removeTemplatesButton, &QPushButton::clicked, this, &AddCharacterDialog::removeTemplateButtonClicked);
    //connect(saveTemplateButton, &QPushButton::clicked, this, &AddCharacterDialog::saveTemplateButtonClicked);

    connect(m_templatesListWidget, &TemplatesListWidget::itemDoubleClicked, this, &AddCharacterDialog::applyTemplateButtonClicked);

    //necessary??
    connect(m_templatesButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_timer, &QTimer::timeout, this, &AddCharacterDialog::animateLabel);
    connect(m_multipleEnabledBox, &QCheckBox::stateChanged, this, [this] {
        m_instanceNumberBox->setEnabled(m_multipleEnabledBox->checkState() == Qt::Checked);
    });
}


void
AddCharacterDialog::randomButtonClicked()
{
    const auto rand = Utils::General::rollDice();
    m_iniBox->setValue(rand + m_iniModifierBox->value());
    m_rolledValueLabel->setText(tr("Rolled a <b>") + QString::number(rand) + "</b>.");
}


void
AddCharacterDialog::animateLabel()
{
    // Create a small fading text if a character has been stored
    auto *const effect = new QGraphicsOpacityEffect;
    m_animatedLabel->setGraphicsEffect(effect);

    auto *const animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(LABEL_FADEOUT);
    animation->setStartValue(1.0);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}


void
AddCharacterDialog::saveButtonClicked()
{
    if (m_nameEdit->text().isEmpty()) {
        displayWarning(tr("Creation not possible!"), tr("No name has been set. Please set at least a name before storing the Character!"));
        return;
    }
    const auto numberOfInstances = m_multipleEnabledBox->checkState() == Qt::Checked ? m_instanceNumberBox->value() : 1;
    AdditionalInfoData additionalInfoData{ {}, m_addInfoEdit->text() };

    CharacterHandler::Character character(m_nameEdit->text(), m_iniBox->value(), m_iniModifierBox->value(), m_hpBox->value(),
                                          m_enemyBox->isChecked(), additionalInfoData);
    emit characterCreated(character, numberOfInstances);
    resetButtonClicked();
    m_nameEdit->setFocus(Qt::TabFocusReason);

    // Only set the label text after the first stored character,
    // otherwise it will be displayed constantly until something is stored
    if (!m_isFirstCharStored) {
        m_isFirstCharStored = true;
        m_animatedLabel->setText(tr("Character saved!"));
    }
    // Reset the graphics effect and kickoff the animation
    m_animatedLabel->setGraphicsEffect(0);
    m_timer->start(LABEL_SHOWN_DURATION);
}
void
AddCharacterDialog::displayWarning(const QString& title, const QString& text){
    QMessageBox::warning(this, title,
                             text);
}

void
AddCharacterDialog::resetButtonClicked()
{
    // Reset all displayed values
    m_nameEdit->clear();
    m_iniBox->setValue(0);
    m_iniWithoutModValue = 0;
    m_iniModifierBox->setValue(0);
    m_rolledValueLabel->setText("");
    m_hpBox->setValue(0);
    m_enemyBox->setChecked(false);
    m_addInfoEdit->clear();

    m_multipleEnabledBox->setCheckState(Qt::Unchecked);
    m_instanceNumberBox->setValue(2);
}


void
AddCharacterDialog::okButtonClicked()
{
    if (!m_nameEdit->text().isEmpty()) {
        saveButtonClicked();
    }
    QDialog::accept();
}

void
AddCharacterDialog::storeTemplatesButtonClicked(){
    if (m_nameEdit->text().isEmpty()) {
        displayWarning(tr("Creation not possible!"), tr("No name has been set. Please set at least a name before caching the Character!"));
        return;
    }
    CharacterHandler::Character character = CharacterHandler::Character( m_nameEdit->text(),
                                                                         m_iniBox->value(),
                                                                         m_iniModifierBox->value(),
                                                                         m_hpBox->value(),
                                                                         m_enemyBox->isChecked(),
                                                                         AdditionalInfoData{ {}, m_addInfoEdit->text()});
    if (!m_fileHandler->writeToFile(character, character.name + ".char")) {
        displayWarning(tr("Action not possible!"), tr("The Character could not be saved!"));
        return;
    }
    if (!m_templatesListWidget->addCharacter(character)) {
        displayWarning(tr("Action not possible!"), tr("The Character is already in the list!"));
        return;
    }
}
void
AddCharacterDialog::showTemplatesButtonClicked()
{
    m_templatesListWidget->show();
    m_templatesButtonBox->show();
    m_hideTemplatesButton->show();
    m_storeTemplatesButton->show();
    m_removeTemplatesButton->show();
    if (m_templatesListWidget->count() == 0) loadTemplates();
    QDialog::adjustSize();
}
void
AddCharacterDialog::hideTemplatesButtonClicked()
{
    m_templatesListWidget->hide();
    m_templatesButtonBox->hide();
    m_hideTemplatesButton->hide();
    m_storeTemplatesButton->hide();
    m_removeTemplatesButton->hide();

    QDialog::adjustSize();
}
void
AddCharacterDialog::applyTemplateButtonClicked() {
    auto const selectedItems = m_templatesListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        displayWarning(tr("Action not possible!"), tr("Please select a Character from the list to apply it to the current Character!"));
        return;
    }
    auto const character = selectedItems.first()->data(Qt::UserRole).value<CharacterHandler::Character>();
    m_nameEdit->setText(character.name);
    m_iniBox->setValue(character.initiative);
    m_iniModifierBox->setValue(character.modifier);
    m_hpBox->setValue(character.hp);
    m_enemyBox->setChecked(character.isEnemy);
    m_addInfoEdit->setText(character.additionalInfoData.mainInfoText);
}
void
AddCharacterDialog::loadTemplates()
{
    QDirIterator it(m_fileHandler->getDirSettings().saveDir, {"*.char"}, QDir::Files);
    while (it.hasNext()) {
        it.next();
        QString fileName = it.fileName();
        switch (auto const code = m_fileHandler->getStatus(fileName); code) {
            case 0:
            {
                auto const characterObject = m_fileHandler->getData();
                CharacterHandler::Character character;
                character.name = characterObject["name"].toString();
                character.initiative = characterObject["initiative"].toInt();
                character.modifier = characterObject["modifier"].toInt();
                character.hp = characterObject["hp"].toInt();
                character.isEnemy = characterObject["is_enemy"].toBool();
                character.additionalInfoData.mainInfoText = characterObject["additional_info"].toString();
                if (!m_templatesListWidget->addCharacter(character)) {
                    displayWarning(tr("Action not possible!"), tr("The Character is already in the list!"));
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
            {
                break;
            }
            default:
                break;
        }
    }

}
void
AddCharacterDialog::removeTemplateButtonClicked()
{
    auto const selectedItems = m_templatesListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        displayWarning(tr("Action not possible!"), tr("Please select a Character from the list to remove the current Character!"));
        return;
    }
    auto const character = selectedItems.first()->data(Qt::UserRole).value<CharacterHandler::Character>();
    if (!m_templatesListWidget->removeCharacter(character)) {
        displayWarning(tr("Action not possible!"), tr("Could not remove Character!"));
        return;
    }
    //TODO: check assumption about char name identity and uniqueness
    if (!m_fileHandler->removeCharacter(character.name + ".char")) {
        displayWarning(tr("Action not possible!"), tr("The Character could not be removed!"));
    }
}