
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_image_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("choose"), "", tr("Images(*.png *.jpg *.jpeg *.bmp *.gif"));
    if (QString::compare(filename, QString()) != 0)
    {
        QImage image;
        bool valid = image.load(filename);
        if (valid)
        {
            image = image.scaledToWidth(ui->lbl_image->width(), Qt::SmoothTransformation);
            ui->lbl_image->setPixmap(QPixmap::fromImage(image));
        }
    }
    QString dbName("C:/Users/Dell/Desktop/imageloading/sqlite.db");
    //    QFile::remove( dbName ); // delete sqlite file if it exists from a previous run
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    db.open();

    // Generate an image (in this case a screenshot) and store it in a QByteArray
    //    QScreen *screen = app.primaryScreen();
    //    QPixmap inPixmap = screen->grabWindow( 0 );

    QPixmap *inPixmap = new QPixmap(filename);
    //     QPixmap qImage2 = qImage2->scaled(this->width(),this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //    QPixmap inPixmap;
    QByteArray inByteArray;
    QBuffer inBuffer(&inByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    inPixmap->save(&inBuffer, "PNG"); // write inPixmap into inByteArray in PNG format

    // Alternatively, load an image file directly into a QByteArray
    //     QFile file("screenshot.png");
    //     if (!file.open(QIODevice::ReadOnly))
    //         return 0;
    //     QByteArray inByteArray = file.readAll();

    // Insert image bytes into the database
    // Note: make sure to wrap the :placeholder in parenthesis
    QSqlQuery query = QSqlQuery(db);
    //    query.exec( "CREATE TABLE IF NOT EXISTS imgTable ( filename TEXT, imagedata BLOB )" );

    query.prepare("INSERT INTO imgTable (filename, imagedata) VALUES ('added', :imageData)");
    query.bindValue(":imageData", inByteArray);
    if (!query.exec())
        qDebug() << "Error inserting image into table:\n"
                 << query.lastError();

    db.close();
}

void MainWindow::on_btn_clicked2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("choose"), "", tr("Images(*.png *.jpg *.jpeg *.bmp *.gif"));
    if (QString::compare(filename, QString()) != 0)
    {
        QImage image;
        bool valid = image.load(filename);
        if (valid)
        {
            image = image.scaledToWidth(ui->lbl_image2->width(), Qt::SmoothTransformation);
            ui->lbl_image2->setPixmap(QPixmap::fromImage(image));
        }
    }
}


void MainWindow::on_click_on_clicked()
{
        QString dbName("C:/Users/Dell/Desktop/imageloading/sqlite.db");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName);
        db.open();
        QSqlQuery qry;
        qry.prepare("select * from imgTable where filename='added'");
        if(!qry.exec())
        {
            qDebug()<<"Not selected";
        }
        if(qry.exec())
        {
            while(qry.next()){
                QString filename=qry.value(0).toString();
                qDebug()<<filename;
                QByteArray outByteArray = qry.value(1).toByteArray();
                QPixmap outPixmap = QPixmap();
                outPixmap.loadFromData( outByteArray );


                // Display image

                 outPixmap = outPixmap.scaledToWidth(ui->lbl_image3->width(), Qt::SmoothTransformation);
                ui->lbl_image3->setPixmap( outPixmap );

                    db.close();

            }




        }






}
