#include "mainwindow.h"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QBuffer>
#include <QUuid>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    client = new OCRClient();
    connect(client, &OCRClient::resultReady, this, &MainWindow::handleResult);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    uploadButton = new QPushButton("Upload Images");
    progressBar = new QProgressBar();
    resultsList = new QListWidget();

    layout->addWidget(uploadButton);
    layout->addWidget(progressBar);
    layout->addWidget(resultsList);

    setCentralWidget(central);

    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadClicked);

    currentBatchId = QUuid::createUuid().toString();
}

void MainWindow::onUploadClicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Upload Images");

    for (QString path : files) {
        QFile f(path);
        f.open(QIODevice::ReadOnly);
        QByteArray data = f.readAll();

        QString imgId = QUuid::createUuid().toString();
        resultsList->addItem("Processing: " + imgId);

        totalImages++;
        progressBar->setMaximum(totalImages);

        client->sendImage(currentBatchId, imgId, data);
    }
}

void MainWindow::handleResult(QString imageId, QString text) {
    processedImages++;
    progressBar->setValue(processedImages);

    resultsList->addItem("Result (" + imageId + "): " + text);

    if (processedImages == totalImages) {
        totalImages = 0;
        processedImages = 0;
        currentBatchId = QUuid::createUuid().toString();
        resultsList->addItem("Batch complete.");
    }
}
