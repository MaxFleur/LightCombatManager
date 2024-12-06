#include "ChangeStatDialog.hpp"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QVBoxLayout>

ChangeStatDialog::ChangeStatDialog(bool changeHP, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(changeHP ? tr("Change Character HP") : tr("Add Info Text"));

    auto* const valueLabel = new QLabel(changeHP ? tr("Value:") : tr("Text:"));

    auto* const widgetLayout = new QHBoxLayout;
    widgetLayout->addWidget(valueLabel);

    if (changeHP) {
        auto* const hpBox = new QSpinBox;
        hpBox->setRange(-10000, 10000);
        hpBox->setValue(0);

        connect(hpBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] (int value) {
            m_variant = QVariant::fromValue(value);
        });
        widgetLayout->addWidget(hpBox);
    } else {
        auto* const lineEdit = new QLineEdit;

        connect(lineEdit, &QLineEdit::textChanged, this, [this] (const QString& text) {
            m_variant = QVariant::fromValue(text);
        });
        widgetLayout->addWidget(lineEdit);
    }

    auto *const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* const mainLayout = new QVBoxLayout;
    mainLayout->addLayout(widgetLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
