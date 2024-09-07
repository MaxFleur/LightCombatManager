#include "ChangeHPDialog.hpp"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

ChangeHPDialog::ChangeHPDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Change Character HP"));

    auto* const valueLabel = new QLabel(tr("Value:"));
    auto* const hpBox = new QSpinBox;
    hpBox->setRange(-10000, 10000);
    hpBox->setValue(0);

    auto* const valueLayout = new QHBoxLayout;
    valueLayout->addWidget(valueLabel);
    valueLayout->addWidget(hpBox);

    auto *const buttonBox = new QDialogButtonBox;
    auto *const okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    auto* const mainLayout = new QVBoxLayout;
    mainLayout->addLayout(valueLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(okButton, &QPushButton::clicked, this, [this, hpBox] {
        m_hpValue = hpBox->value();
        QDialog::accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
