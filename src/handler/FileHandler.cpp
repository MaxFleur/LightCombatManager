#include "FileHandler.hpp"

#include <QDebug>
#include <QFile>

#include <QtGlobal>

#include "AdditionalInfoData.hpp"

// Stores table data as csv
bool
FileHandler::saveTable(
    const QVector<QVector<QVariant> >& tableData,
    const QString&                     filename,
    unsigned int                       rowEntered,
    unsigned int                       roundCounter,
    const RuleSettings::Ruleset&       ruleset,
    bool                               rollAutomatically) const
{
    // Create file
    QFile file(filename);

    // Check if the device is open for writing
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream data(&file);

#if QT_VERSION_MAJOR > 5
        data.setEncoding(QStringConverter::Utf8);
#else
        data.setCodec("UTF-8");
#endif
        data.setGenerateByteOrderMark(false);

        QStringList strList;

        // Store the table data header
        strList << "Name" << "Initiative" << "INI modifier" << "HP" << "Is Enemy" << "Additional information";
        data << strList.join(";") + "\n";

        // Store main table data
        auto firstRow = true;
        for (const auto& row : tableData) {
            // Clear the list at the beginning of every row iteration
            strList.clear();

            for (auto i = 0; i < row.size(); i++) {
                if (i != row.size() - 1) {
                    strList << row.at(i).toString();
                } else {
                    auto addInfo = row.at(i).value<AdditionalInfoData::AdditionalInformation>();
                    // Use the dashes and plus signs for separation
                    auto savedAddInfoText = addInfo.mainInfo;
                    if (!addInfo.statusEffects.empty()) {
                        savedAddInfoText += "---";
                    }
                    for (const auto& statusEffect : addInfo.statusEffects) {
                        savedAddInfoText.append(statusEffect.name + "+" + QString::number(statusEffect.isPermanent) + "+" +
                                                QString::number(statusEffect.duration) + "--");
                    }
                    strList << savedAddInfoText;
                }
            }
            if (firstRow) {
                firstRow = false;
                strList << QString::number(rowEntered) << QString::number(roundCounter)
                        << QString::number(ruleset) << QString::number(rollAutomatically);
            }

            // Line break
            data << strList.join(";") + "\n";
        }

        file.close();
        return true;
    }
    return false;
}


// Open an existing csv table and stream its data
int
FileHandler::getCSVStatus(const QString& filename)
{
    // If the data can be read, import
    if (QFile importedCSV(filename); importedCSV.open(QFile::ReadOnly)) {
        QTextStream in(&importedCSV);
#if QT_VERSION_MAJOR > 5
        in.setEncoding(QStringConverter::Utf8);
#else
        in.setCodec("UTF-8");
#endif
        in.setGenerateByteOrderMark(false);
        m_data = QString();

        // Import file
        while (!in.atEnd()) {
            m_data.append(in.readLine() + "\n");
        }
        importedCSV.close();

        if (checkTableFormat(m_data)) {
            // Success
            return 0;
        }
        // Table in false format
        return 1;
    }
    // Unreadable data
    return 2;
}


// Checks if a table is in the correct format
bool
FileHandler::checkTableFormat(const QString& data) const
{
    if (data.isEmpty()) {
        return false;
    }

    const auto rowDataHeader = data.split("\n").at(0).split(";");
    const auto rowDataFirstRow = data.split("\n").at(1).split(";");

    // Test if the stored data has the correct header columns
    if (rowDataHeader.size() == 6
        && rowDataHeader[0] == "Name"
        && rowDataHeader[1] == "Initiative"
        && rowDataHeader[2] == "INI modifier"
        && rowDataHeader[3] == "HP"
        && rowDataHeader[4] == "Is Enemy"
        && rowDataHeader[5] == "Additional information"

        // 7th entry should contain the player on the move, 8th the round counter,
        // 9th the ruleset and 10th the roll automatically option
        && rowDataFirstRow.size() == 10) {
        return true;
    }
    return false;
}
