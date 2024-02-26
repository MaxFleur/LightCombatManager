#pragma once

#include <QProxyStyle>
#include <QStyleOption>

// Style used to center checkboxes inside the combat table
class CheckBoxStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit
    CheckBoxStyle(QStyle *style = nullptr) : QProxyStyle(style)
    {
    }

    [[nodiscard]] QRect
    subElementRect(QStyle::SubElement  element,
                   const QStyleOption *option,
                   const QWidget *     widget = nullptr) const override
    {
        auto rectangle = QProxyStyle::subElementRect(element, option, widget);
        if (element == QStyle::SE_ItemViewItemCheckIndicator) {
            rectangle.moveCenter(option->rect.center());
        }
        return rectangle;
    }
};
