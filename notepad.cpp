#include "notepad.h"
#include "ui_notepad.h"

QColor* NotePad::colorPickerColor;


NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NotePad)
{
     QColor firstColor = QColor("green");
    colorPickerColor = &firstColor;
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
}

NotePad::~NotePad()
{
    delete ui;
}


void NotePad::on_actionnew_triggered()
{
currentFile.clear();
ui->textEdit->setText(QString());
}


void NotePad::on_actionopen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open this file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","cannot open file " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in (&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}


void NotePad::on_actionSave_As_triggered()
{
    QString fileSaveName = QFileDialog::getSaveFileName(this,"Save as");
    QFile file(fileSaveName);
    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,"Warning","cannot save file " + file.errorString());
        return;
    }
    currentFile = fileSaveName;
    setWindowTitle(fileSaveName);
    QTextStream out (&file);
    QString text = ui->textEdit->toPlainText();
    out<< text;
    file.close();
}


void NotePad::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("my_Printer");
    QPrintDialog pDialog (&printer,this);
    if(pDialog.exec() == QDialog::Rejected){
        QMessageBox::warning(this,"Print Error", "cannot print " );
        return;
    }
    ui->textEdit->print(&printer);
}


void NotePad::on_actionExit_triggered()
{
    QApplication::quit();
}



void NotePad::on_actionCopy_triggered()
{
     ui->textEdit->copy();
}


void NotePad::on_actionPaste_triggered()
{
     ui->textEdit->paste();
}


void NotePad::on_actionUndo_triggered()
{
     ui->textEdit->undo();
}


void NotePad::on_actionRedo_triggered()
{
     ui->textEdit->redo();
}


void NotePad::on_actionCut_triggered()
{
     ui->textEdit->cut();
}


void NotePad::setHighlightToDefault(){
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();
    format.setBackground(Qt::transparent);
    cursor.setCharFormat(format);
    ui->textEdit->setTextCursor(cursor);
}

void NotePad::checkHighlight(QString text)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();
    QColor selectedColor = *colorPickerColor;
    format.setBackground(selectedColor);
    QTextCharFormat defaultformat = cursor.charFormat();
    defaultformat.setBackground(Qt::transparent);

    int pos = 0;

    while ((pos = ui->textEdit->toPlainText().indexOf(text, pos)) != -1) {
        QTextCursor highlightCursor(ui->textEdit->document());
        highlightCursor.setPosition(pos);
        highlightCursor.movePosition(QTextCursor::WordRight, QTextCursor::KeepAnchor, text.length());

        if (highlightCursor.charFormat().background() == Qt::blue) {
            // The text is already highlighted
            cursor.setCharFormat(defaultformat);
            return;
        }
        else{
            cursor.setCharFormat(format);

        }

        pos += text.length();
    }

    // The text is not already highlighted, so highlight it
    //cursor.setCharFormat(defaultformat);
    ui->textEdit->setTextCursor(cursor);
}

void NotePad::on_actionHighlight_triggered()
{
    QString selectedText = ui->textEdit->textCursor().selectedText();

    NotePad::checkHighlight(selectedText);

}




void NotePad::on_actioncolor_selection_triggered()
{
    // show the color picker dialog and get the selected color
        QColor color = QColorDialog::getColor(Qt::yellow, this);

        // set the text background color to the selected color
        if (color.isValid()) {
            colorPickerColor = &color;
        }
}

