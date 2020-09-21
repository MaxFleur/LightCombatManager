#include "../../include/GUI/TableWidget.h"

// Create the welcome widget at instantiation
TableWidget::TableWidget(CharacterHandlerRef charHandler, QWidget *parent)
        : m_char(charHandler) {
    createTable();
}

// This function creates the table of this widget
void TableWidget::createTable() {
    // Allocate the table widget
    tableWidget = new QTableWidget();
    // Set the number of rows to characters created
    tableWidget->setRowCount(m_char->getCharacters().size());
    // Four colums for name, hp, isNPC and additional information about a character
    tableWidget->setColumnCount(4);
    // Set the header infos
    tableHeader << "Name" << "HP" << "Is NPC" << "Additional information";
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    // Store the created character data in the table
    for(int i = 0; i < m_char->getCharacters().size(); i++) {
        // Store the name
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(m_char->getCharacters().at(i)->name)));
        // Same for hp
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_char->getCharacters().at(i)->hp)));
        // Check if the bool is true or false, return yes or no
        if(m_char->getCharacters().at(i)->isNPC) {
            tableWidget->setItem(i, 2, new QTableWidgetItem("Yes"));
        } else {
            tableWidget->setItem(i, 2, new QTableWidgetItem("No"));
        }
        // Store additional information
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(m_char->getCharacters().at(i)->additionalInf)));
    }
    // Set the numbers on the headers to not visible
    tableWidget->verticalHeader()->setVisible(false);
    // grid not visible
    tableWidget->setShowGrid(false);
    // Set to strect mode (TEMPORARY)
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Allocate the main layout, add the table widget to this
    tableLayout = new QVBoxLayout(this);
    tableLayout->addWidget(tableWidget);
}

TableWidget::~TableWidget() {
    // TODO: Delete the widget items
}
 
