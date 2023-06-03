#pragma once

#include <QLineEdit>
#include <QPointer>

class QHBoxLayout;
class QLabel;

// This class displays additional information and status effects
class AdditionalInfoWidget : public QWidget {
    Q_OBJECT

public:
    AdditionalInfoWidget();

    void
    setMainInfoText(const QString& text)
    {
        m_additionalInfoLineEdit->setText(text);
    }

    const QString
    getMainInfoText()
    {
        return m_additionalInfoLineEdit->text();
    }

signals:
    void
    lineEditFocused();

    void
    mainInfoEdited();

private:
    bool
    eventFilter(QObject* object,
                QEvent*  event);

private:
    QPointer<QLineEdit> m_additionalInfoLineEdit;
};
