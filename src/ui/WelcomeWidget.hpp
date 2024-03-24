#pragma once

#include <QPointer>
#include <QWidget>

class QSvgWidget;

// This class creates the welcome text
class WelcomeWidget : public QWidget {
public:
    explicit
    WelcomeWidget(QWidget *parent = 0);

private:
    void
    setSvgWidgetIcon();

    bool
    event(QEvent *event) override;

private:
    QPointer<QSvgWidget> m_svgWidget;
};
