#include "editlrc.h"
#include "ui_editlrc.h"

EditLrc::EditLrc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditLrc)
{
    ui->setupUi(this);
    QFont defaultFont("consolas",14);
    ui->plainTextEdit->setFont(defaultFont);
    connect(ui->plainTextEdit,SIGNAL(textChanged()),this,SLOT(modified()));

    save = new QShortcut(QKeySequence("Ctrl+s"),this);
    saveAs = new QShortcut(QKeySequence("Ctrl+Shift+s"),this);
    open = new QShortcut(QKeySequence("Ctrl+o"),this);
    insert = new QShortcut(QKeySequence("Ctrl+i"),this);
    fastForward = new QShortcut(QKeySequence("F12"),this);
    rewind = new QShortcut(QKeySequence("F11"),this);
    connect(save,SIGNAL(activated()),this,SLOT(on_actionSave_triggered()));
    connect(saveAs,SIGNAL(activated()),this,SLOT(on_actionSave_as_triggered()));
    connect(open,SIGNAL(activated()),this,SLOT(on_actionOpen_triggered()));
    connect(insert,SIGNAL(activated()),this,SLOT(on_actionInsert_triggered()));
    connect(fastForward,SIGNAL(activated()),this,SLOT(on_actionFastForward_triggered()));
    connect(rewind,SIGNAL(activated()),this,SLOT(on_actionRewind_triggered()));

}

void EditLrc::setParentWindow(MainWindow *p)
{
    parentWindow = p;
}

bool EditLrc::judgeSave()
{
    if(ui->plainTextEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this,"Warning","The text has been modified.Do you want to save your changes?",
                                   QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
        {
            on_actionSave_triggered();
            return true;
        }
        if(ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void EditLrc::setCurrentFile(const QString &fileName)
{
    currentName = fileName;
    ui->plainTextEdit->document()->setModified(false);
    this->setWindowModified(false);
    fileName.isEmpty()?this->setWindowFilePath("Untitled.lrc"):this->setWindowFilePath(fileName);
}

bool EditLrc::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::critical(this,"Error","Cannot write file");
        return false;
    }
    else
    {
        QTextStream out(&file);
        out<<ui->plainTextEdit->toPlainText();
        setCurrentFile(fileName);
        return true;
    }
}

void EditLrc::loadFile(const QString &fileName)
{
    QFile file(fileName);
        if(!file.open(QFile::ReadOnly|QFile::Text))
        {
            QMessageBox::critical(this,
                                  "Error",
                                  "Cannot read file"
                                  );
            return;
        }
        else
        {
            QTextStream in(&file);
            ui->plainTextEdit->setPlainText(in.readAll());
            setCurrentFile(fileName);
        }
}

void EditLrc::closeEvent(QCloseEvent *event)
{
    if(judgeSave())
    {
        event->accept();
    }
    if(ui->plainTextEdit->document()->isEmpty())
    {
        event->accept();
    }
}

EditLrc::~EditLrc()
{
    delete ui;
}

void EditLrc::on_actionNew_triggered()
{
    if(judgeSave())
    {
        ui->plainTextEdit->clear();
        setCurrentFile("");
    }
}

void EditLrc::on_actionOpen_triggered()
{
    if(judgeSave())
    {
        QString temp = QFileDialog::getOpenFileName(this);
        if(!temp.isEmpty())
            loadFile(temp);
    }
}

void EditLrc::on_actionSave_triggered()
{
    if(currentName.isEmpty())
    {
        return on_actionSave_as_triggered();
    }
    else
        saveFile(currentName);
}

void EditLrc::on_actionSave_as_triggered()
{
    QString temp = QFileDialog::getSaveFileName(this);
    if(!temp.isEmpty())
    {
        saveFile(temp);
    }
}

void EditLrc::on_actionClose_triggered()
{
    if(judgeSave())
    {
        this->close();
    }
    if(ui->plainTextEdit->document()->isEmpty())
    {
        this->close();
    }
}

void EditLrc::on_actionInsert_triggered()
{
    ui->plainTextEdit->insertPlainText(parentWindow->getTimeLabel());
//    QTextCursor cursor(ui->plainTextEdit->textCursor());
//    cursor.movePosition(QTextCursor::StartOfLine);
//    cursor.movePosition(QTextCursor::Down);
    ui->plainTextEdit->moveCursor(QTextCursor::StartOfLine);
    ui->plainTextEdit->moveCursor(QTextCursor::Down);
}

void EditLrc::on_actionFastForward_triggered()
{
    if((parentWindow->player->duration()-parentWindow->player->position())>5000)
        parentWindow->player->setPosition(parentWindow->player->position()+5000);
    else
        parentWindow->on_actionStop_triggered();
}

void EditLrc::on_actionRewind_triggered()
{
    if(parentWindow->player->position()>5000)
        parentWindow->player->setPosition(parentWindow->player->position()-5000);
    else
        parentWindow->player->setPosition(0);
}

void EditLrc::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,QFont("consolas",14),this,"Choose a font");

    if(ok)
    {
        ui->plainTextEdit->setFont(font);
    }
    else
    {
        return;
    }
}

void EditLrc::modified()
{
    this->setWindowTitle(currentName+'*');
}
