#include "../include/FileHandler.h" 

FileHandler::FileHandler() {
} 

// Stores a table with characters as a csv file
void FileHandler::saveTable(QTableWidget *tableWidget, QString filename) {
    
    QFile file(filename);
    
    if (file.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream data( &file );
        QStringList strList;
            
        for(int r = 0; r < tableWidget->rowCount(); ++r ) {
            strList.clear();
            for( int c = 0; c < tableWidget->columnCount(); ++c ) {
                QTableWidgetItem* item = tableWidget->item(r,c);        //Load items
                if (!item || item->text().isEmpty()) {                  //Test if there is something at item(r,c)
                    tableWidget->setItem(r,c,new QTableWidgetItem("0"));//IF there is nothing write 0
                }
                strList << "\" " + tableWidget->item( r, c )->text() + "\" ";
            }
            data << strList.join( ";" ) + "\n";
        }
        file.close();
    }
}

// Open an existing csv table
// Currently returns nothing
// TODO
QTableWidget* FileHandler::openTable() {
    return nullptr;
}
