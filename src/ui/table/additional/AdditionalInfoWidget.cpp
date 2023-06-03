#include "AdditionalInfoWidget.hpp"

#include <QEvent>
#include <QHBoxLayout>

AdditionalInfoWidget::AdditionalInfoWidget()
{
    m_additionalInfoLineEdit = new QLineEdit;
    m_additionalInfoLineEdit->installEventFilter(this);

    auto* const mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_additionalInfoLineEdit);

    setLayout(mainLayout);

    connect(m_additionalInfoLineEdit, &QLineEdit::returnPressed, this, [this] {
        m_additionalInfoLineEdit->clearFocus();
        emit mainInfoEdited();
    });
}


bool
AdditionalInfoWidget::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_additionalInfoLineEdit && event->type() == QEvent::FocusIn) {
        emit lineEditFocused();
        return false;
    }
    return false;
}
