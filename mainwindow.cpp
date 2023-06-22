#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    // Get the font metrics of the current font
    QFontMetrics metrics(ui->textEdit->font());

    // Set the tab stop width to 4 * the width of the space character
    ui->textEdit->setTabStopDistance(4 * metrics.horizontalAdvance(' '));


     new SyntaxHighlighter(ui->textEdit->document());



}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_actionNew_File_triggered()
{
    currentFile.clear();
    ui->textEdit->clear();

}


void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if(!file.open(QIODevice::ReadOnly|QFile::Text)){
        QMessageBox::warning(this, tr("Warning"), tr("Can't open the file") + file.errorString());
        return;
    }
    else{
        setWindowTitle(currentFile);
        QTextStream in(&file);
        QString text = in.readAll();
        ui->textEdit->setText(text);
        file.close();
    }

}


void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as: ");
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QFile::Text)){
        QMessageBox::warning(this, tr("Warning"), tr("Can't save the file") + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(currentFile);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if(pDialog.exec() == QDialog::Rejected){
        QMessageBox::warning(this, tr("Warning"), tr("Can't access printer"));
        return;
    }
    ui->textEdit->print(&printer);

}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionClear_triggered()
{
    ui->textEdit->clear();
}


void MainWindow::on_actionCompile_triggered()
{
    // Save the current contents of the text editor to a temporary file
    QString fileName = "output.cpp";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        // Error handling
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();

    file.close();

    // Compile the temporary file using g++
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);

    process.start("g++", QStringList() << "-o" << "output" << fileName);
    process.waitForFinished();

    // Display the output of the compilation process in a message box
    QString output(process.readAll());
    if (process.exitCode() == 0) {
        process.start("output");
        process.waitForFinished();
        output.append(process.readAll());
    }
    QString error = process.readAllStandardError();
    QMessageBox::information(this, "Output", output + "\n" + error);
    qInfo() << output;
}

