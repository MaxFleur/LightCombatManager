#include "StatusEffectButton.hpp"

#include <QAction>
#include <QHBoxLayout>
#include <QMenu>
#include <QMouseEvent>

StatusEffectButton::StatusEffectButton(AdditionalInfoData::StatusEffect& statusEffect) :
    m_statusEffect(statusEffect)
{
    setButtonText();
    createMenu();
}


void
StatusEffectButton::setButtonText()
{
    auto text = m_statusEffect.name;
    if (!m_statusEffect.isPermanent) {
        text.append(" (" + QString::number(m_statusEffect.duration) + ")");
    }
    setText(text);
}


void
StatusEffectButton::changeDuration(int value)
{
    auto signedValue = (int) m_statusEffect.duration;
    signedValue += value;
    m_statusEffect.duration = std::max(signedValue, 1);
    setButtonText();

    emit effectChanged(m_statusEffect);
}


void
StatusEffectButton::createMenu()
{
    auto *const menu = new QMenu(this);

    if (!m_statusEffect.isPermanent) {
        auto *const increaseMenu = menu->addMenu(tr("Add Rounds..."));
        auto *const decreaseMenu = menu->addMenu(tr("Remove Rounds..."));

        for (int i = 1; i < 4; i++) {
            increaseMenu->addAction(QString::number(i), this, [this, i] {
                changeDuration(i);
            });
            decreaseMenu->addAction(QString::number(i), this, [this, i] {
                changeDuration(i * (-1));
            });
        }
    }

    menu->addAction(m_statusEffect.isPermanent ? tr("Make Temporary") : tr("Make Permanent"),
                    this, [this] {
        m_statusEffect.isPermanent = !m_statusEffect.isPermanent;
        emit effectChanged(m_statusEffect);
    });
    menu->addAction(tr("Remove"), this, [this] {
        emit removeCalled();
    });

    setMenu(menu);

    connect(menu, &QMenu::aboutToShow, this, [this] {
        emit menuCalled();
    });
}
