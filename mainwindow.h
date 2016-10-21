#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//greaterThan(QT_MAJOR_VERSION, 4):
#include <QMainWindow>
#include <QMultimedia>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QTime>
#include <QMediaContent>
#include <QMediaResource>
#include <QMediaPlaylist>
#include <QStandardItemModel>
#include <QStringListIterator>
#include <QtWidgets>
#include <QAction>

#include "editlrc.h"
class EditLrc;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMediaPlayer *player;
    EditLrc *edit;
    QString name;
    QShortcut *shortCutForHSlider;
    QShortcut *shortCutForHSlider2;
    QShortcut *shortCutForVSlider;
    QShortcut *shortCutForVSlider2;

//    QStandardItemModel *playListModel;
//    QStringList path;
//    QMediaPlaylist *playList;
    QFrame *frame;

    QString getTimeLabel();

public slots:
    void on_actionOpen_triggered();

    void on_actionStop_triggered();

    void on_actionPlay_triggered();

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void on_volumeChanged(int volume);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void leapForward();
    void leapBackward();
    void volumeUp();
    void volumeDown();

//    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_actionEditLrc_triggered();

private slots:
    void on_actionSkinOne_triggered();

    void on_actionSkinTwo_triggered();

    void on_actionSkinThree_triggered();

    void on_actionSkinFour_triggered();

    void on_actionSkinFive_triggered();

    void on_actionShortCut_triggered();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
