#ifndef EDITLRC_H
#define EDITLRC_H

#include <QMainWindow>
#include <QFontDialog>
#include <QShortcut>
#include "mainwindow.h"

class MainWindow;
namespace Ui {
class EditLrc;
}

class EditLrc : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditLrc(QWidget *parent = 0);
    void setParentWindow(MainWindow *p);

    QString currentName;
    bool judgeSave();
    void setCurrentFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void loadFile(const QString &fileName);

    QShortcut *save;
    QShortcut *saveAs;
    QShortcut *open;
    QShortcut *insert;
    QShortcut *fastForward;
    QShortcut *rewind;

    void closeEvent(QCloseEvent *event);
    ~EditLrc();

    MainWindow *parentWindow;
public slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionClose_triggered();

    void on_actionInsert_triggered();

    void on_actionFastForward_triggered();

    void on_actionRewind_triggered();

    void on_actionFont_triggered();

    void modified();

private slots:
    void on_actionPlay_triggered();

    void on_actionStop_triggered();

private:
    Ui::EditLrc *ui;
};

#endif // EDITLRC_H
