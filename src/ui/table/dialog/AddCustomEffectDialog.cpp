#include "AddCustomEffectDialog.hpp"

#include "UtilsGeneral.hpp"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>

AddCustomEffectDialog::AddCustomEffectDialog(const QList<QString>& otherEffects, QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Add Custom Effect"));

    auto* const lineEdit = new QLineEdit;

    auto *const buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    auto* const mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this, otherEffects, lineEdit] {
        if (otherEffects.contains(lineEdit->text())) {
            Utils::General::displayWarningMessageBox(this, tr("Effect already exists!"),
                                                     tr("The effect already exists. Please provide a different name!"));
            return;
        }

        m_name = lineEdit->text();
        QDialog::accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
