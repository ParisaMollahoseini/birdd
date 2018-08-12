#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QGraphicsPixmapItem>
#include<QMessageBox>
#include<QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    firstthings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::firstthings()
{
    gmover=new gameover;

    barriers.resize(8);

    ui->gv->setFixedSize(900,900);//view pos
    ui->gv->setGeometry(0,0,900,900);

   mybird = new bird();
   mybird->time->restart();
   mybird->setFlag(QGraphicsItem::ItemIsFocusable);
   mybird->QGraphicsEllipseItem::setFocus();
//
    ui->gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//
    for (int i=0; i<barriers.size(); i++)
    {
        barriers[i] = new barrier();
    }



   //scene

   sc=new QGraphicsScene;
    sc->addItem(mybird);

    for (int i=0; i<barriers.size(); i++)
    {
        sc->addItem(barriers[i]);
    }

    sc->setSceneRect(0,0,900,900);//scene pos
    QPixmap pix(":/game-background.jpg");

    sc->addPixmap(pix);
    mybird->setZValue(2);

    for (int i=0; i<barriers.size(); i++)
    {
        barriers[i]->setZValue(2);
    }
    //scene
    ui->gv->setScene(sc);
    //qDebug()<<"view "<<mybird->QGraphicsEllipseItem::brush().color();
    ui->gv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     mybird->setRect(80, 140, 40, 35);
    //mybird->setRect(ui->gv->pos().x(),ui->gv->pos().y(),25,25);
    //mybird->setX(30);mybird->setY(50);
    barrierSize();
   // qDebug()<<"birddddd  x:"<<mybird->QGraphicsEllipseItem::x();
    mybird->QGraphicsEllipseItem::setBrush(Qt::blue);
    mybird->update();

    ui->gv->setRenderHint(QPainter::Antialiasing);

    sc->update();
    ui->gv->update();
    mybird->start();

    for (int i=0; i<barriers.size(); i++)
    {
        barriers[i]->update();
        barriers[i]->start();
    }

      //connects
    connect(mybird,SIGNAL(end()),this,SLOT(endprogram()));
    connect(gmover,SIGNAL(emitsignalofstartingmainwindow()),this,SLOT(startagain()));
    //connects

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(check()));
    timer->start(10);
}

void MainWindow::barrierSize()
{
    barriers[0]->setRect(290, 0, 80, 250);
    barriers[1]->setRect(290, 450, 80, 450);
    barriers[2]->setRect(470, 0, 80, 350);
    barriers[3]->setRect(470, 550, 80, 350);
    barriers[4]->setRect(650, 0, 80, 500);
    barriers[5]->setRect(650, 700, 80, 200);
    barriers[6]->setRect(830, 0, 80, 400);
    barriers[7]->setRect(830, 600, 80, 300);
}

void MainWindow::check()
{
    for (int i=0; i<barriers.size(); i++)
    {
        if (barriers[i]->collidesWithItem(mybird))
        {
            qDebug()<<i;
            this->endprogram();
        }
    }

}

void MainWindow::startagain()
{
    qDebug()<<"omad...";
    ui=new Ui::MainWindow;
    ui->setupUi(this);
    firstthings();
    show();
}

void MainWindow::endprogram()
{
    mybird->score=mybird->time->elapsed()/1000;
    qDebug()<<"time is"<<mybird->time->elapsed();
    if(mybird->score>mybird->bestscore)
        mybird->bestscore=mybird->score;

    close();

    gmover->start(mybird->score,mybird->bestscore);

    timer->stop();
    mybird->timer->stop();

    for (int i=0; i<barriers.size(); i++)
    {
        barriers[i]->timer->stop();
    }

}
