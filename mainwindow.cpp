#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer;

    this->showMaximized();

    ui->statusBar->addPermanentWidget(ui->timeLabel);
    ui->timeLabel->setText("00:00:00/00:00:00");
    ui->statusBar->addPermanentWidget(ui->mainToolBar);
    ui->statusBar->addPermanentWidget(ui->label);
    ui->statusBar->addPermanentWidget(ui->horizontalSlider);
    ui->statusBar->addPermanentWidget(ui->label_2);
    ui->statusBar->addPermanentWidget(ui->horizontalSlider_2);

    connect(player,&QMediaPlayer::positionChanged,this,&MainWindow::on_positionChanged);
    connect(player,&QMediaPlayer::durationChanged,this,&MainWindow::on_durationChanged);
    connect(player,&QMediaPlayer::volumeChanged,this,&MainWindow::on_volumeChanged);

    ui->horizontalSlider->setSingleStep(2);
    player->setVolume(50);

    shortCutForHSlider = new QShortcut(QKeySequence(Qt::Key_Right),this);
    shortCutForHSlider2 = new QShortcut(QKeySequence(Qt::Key_Left),this);
    shortCutForVSlider = new QShortcut(QKeySequence(Qt::Key_Up),this);
    shortCutForVSlider2 = new QShortcut(QKeySequence(Qt::Key_Down),this);
    connect(shortCutForHSlider,SIGNAL(activated()),this,SLOT(leapForward()));
    connect(shortCutForHSlider2,SIGNAL(activated()),this,SLOT(leapBackward()));
    connect(shortCutForVSlider,SIGNAL(activated()),this,SLOT(volumeUp()));
    connect(shortCutForVSlider2,SIGNAL(activated()),this,SLOT(volumeDown()));

    QFrame *frame = new QFrame;
    frame->setObjectName("myFrame");
    frame->resize(this->size());
    frame->setStyleSheet("QFrame#myFrame{border-image:url(:/icon/Icon/music.jpg);}");
    frame->show();
    this->setCentralWidget(frame);

    ui->horizontalSlider->setStyleSheet("QSlider::groove:horizontal{border: 1px solid #165708;"
                              "background: #8f8f8f;height: 3px;border-radius: 2px;"
                              "padding-left:-1px;padding-right:-1px;}"
                              "QSlider::handle:horizontal{border:2px solid #454343;"
                              "background:#2af5b9;width:9px;margin-top: -4px;margin-bottom: -4px;border-radius: 4px;} "
                              "QSlider::sub-page:horizontal {background:#2af5b9;border: 1px solid #4A708B;height: 10px;border-radius: 2px;}"
                              "QSlider::add-page:horizontal {background: #615f5f;border: 0px solid #2af5b9;height: 10px;border-radius: 2px;}");
    ui->horizontalSlider_2->setStyleSheet("QSlider::groove:horizontal{border: 1px solid #165708;"
                                "background: #8f8f8f;height: 3px;border-radius: 2px;"
                                "padding-left:-1px;padding-right:-1px;}"
                                "QSlider::handle:horizontal{border:2px solid #454343;"
                                "background:#FFFF33;width:9px;margin-top: -4px;margin-bottom: -4px;border-radius: 4px;} "
                                "QSlider::sub-page:horizontal {background:#FFFF33;border: 1px solid #4A708B;height: 10px;border-radius: 2px;}"
                                "QSlider::add-page:horizontal {background: #615f5f;border: 0px solid #2af5b9;height: 10px;border-radius: 2px;}");

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getTimeLabel()
{
    qint64 temp = player->position();
    //用最笨的方法设置时间标签样式，但是反应速度远远快于调用QTime里面的函数
    QString a,b,c;
    if(temp/60000>=10)
    {
        a=QString::number(temp/60000);
    }
    else
    {
        a='0'+QString::number(temp/60000);
    }
    if((temp/1000)%60>=10)
    {
        b=QString::number((temp/1000)%60);
    }
    else
    {
        b='0'+QString::number((temp/1000)%60);
    }
    if((temp%1000)/10>=10)
    {
        c=QString::number((temp%1000)/10);
    }
    else
    {
        c='0'+QString::number((temp%1000)/10);
    }
    return "["+a+":"+b+"."+c+"]";//返回的是[xx:xx.xx]标准lrc歌词时间戳
}

void MainWindow::on_actionOpen_triggered()
{
    name = QFileDialog::getOpenFileName(this,"Open A Media File","/","Media File (*.mp3 *.ape *.wav)");
    player->setMedia(QUrl::fromLocalFile(name));
    player->play();
    ui->actionPlay->setIcon(QIcon(":/icon/Icon/Pause.png"));
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
    ui->actionPlay->setIcon(QIcon(":/icon/Icon/Play.png"));
    player->setMedia(QUrl::fromLocalFile(name));
    player->pause();
}

void MainWindow::on_actionPlay_triggered()
{
    if(player->state()==QMediaPlayer::PlayingState)
    {
        player->pause();
        ui->actionPlay->setIcon(QIcon(":/icon/Icon/Play.png"));
    }
    else
        if(player->state()==QMediaPlayer::PausedState)
        {
            player->play();
            ui->actionPlay->setIcon(QIcon(":/icon/Icon/Pause.png"));
        }
    else
            if(player->state()==QMediaPlayer::StoppedState)
            {
                if(!player->currentMedia().isNull())
                {
                    player->play();
                    ui->actionPlay->setIcon(QIcon(":/icon/Icon/Pause.png"));
                }
            }
}



void MainWindow::on_positionChanged(qint64 position)
{
    ui->horizontalSlider->setValue(position);
    qint64 current = player->position();
    qint64 total = player->duration();
    QTime currentTime((current/3600000),(current/60000)%60,(current/1000)%60);
    QTime totalTime((total/3600000),(total/60000)%60,(total/1000)%60);
    QString str = currentTime.toString("hh:mm:ss")+'/'+totalTime.toString("hh:mm:ss");
    ui->timeLabel->setText(str);

    if(player->mediaStatus()==QMediaPlayer::EndOfMedia)
    {
        ui->actionPlay->setIcon(QIcon(":/icon/Icon/Play.png"));
        player->setMedia(QUrl::fromLocalFile(name));
        player->pause();
    }
}

void MainWindow::on_durationChanged(qint64 position)
{
    ui->horizontalSlider->setMaximum(position);
}

void MainWindow::on_volumeChanged(int volume)
{
    ui->horizontalSlider_2->setValue(volume);
}

//void MainWindow::on_horizontalSlider_sliderMoved(int position)
//{
//    player->setPosition(position);
//}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    player->setPosition(value);
}


void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
    player->setVolume(position);
}

void MainWindow::leapForward()
{
    if(player->duration()-player->position()>5000)
    {
        player->setPosition(player->position()+5000);
    }
    else
    {
        on_actionStop_triggered();
    }
}

void MainWindow::leapBackward()
{
    if(player->position()>5000)
    {
        player->setPosition(player->position()-5000);
    }
    else
    {
        player->setPosition(0);
    }
}

void MainWindow::volumeUp()
{
    if(player->volume()<100)
    {
        player->setVolume(player->volume()+10);
    }
}

void MainWindow::volumeDown()
{
    if(player->volume()>0)
    {
        player->setVolume(player->volume()-10);
    }
}


void MainWindow::on_horizontalSlider_sliderPressed()
{
    player->pause();
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    player->play();
}

void MainWindow::on_actionEditLrc_triggered()
{
    edit = new EditLrc;
    edit->setParentWindow(this);
    edit->on_actionNew_triggered();
    edit->show();
}
