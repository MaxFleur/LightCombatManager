#include "WelcomeWidget.hpp"

#include "UtilsGeneral.hpp"

#include <QApplication>
#include <QLabel>
#include <QSvgWidget>
#include <QVBoxLayout>

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_svgWidget = new QSvgWidget;
    setSvgWidgetIcon();

    auto *const welcomeLabel = new QLabel(tr("Welcome to Light Combat Manager! \n Start a new Combat ('File' -> 'New') "
                                             "or open an already existing Combat ('File' -> 'Open...')."));
    welcomeLabel->setAlignment(Qt::AlignCenter);

    auto* const versionLabel = new QLabel("v2.2");
    versionLabel->setToolTip(tr("New settings, insert Tables into an existing Combat,\n"
                                "change HP for multiple Characters at once and more!"));
    versionLabel->setAlignment(Qt::AlignRight);

    auto *const layout = new QVBoxLayout(this);
    layout->addWidget(m_svgWidget);
    layout->addWidget(welcomeLabel);
    layout->addWidget(versionLabel);
    layout->setAlignment(m_svgWidget, Qt::AlignHCenter);
    setLayout(layout);
}


void
WelcomeWidget::setSvgWidgetIcon()
{
    const auto isSystemInDarkMode = Utils::General::isSystemInDarkMode();
    m_svgWidget->load(isSystemInDarkMode ? QString(":/icons/logos/main_light.svg") : QString(":/icons/logos/main_dark.svg"));
    const auto svgWidgetWidth = Utils::General::getStringWidth("Welcome to Light Combat Manager! ");
    m_svgWidget->setFixedSize(svgWidgetWidth, svgWidgetWidth * 0.8);
}


bool
WelcomeWidget::event(QEvent *event)
{
    if (event->type() == QEvent::ApplicationPaletteChange || event->type() == QEvent::PaletteChange) {
        setSvgWidgetIcon();
    }
    return QWidget::event(event);
}
