#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QListWidget>

#include "ocr_client.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onUploadClicked();
    void handleResult(QString imageId, QString text);

private:
    OCRClient *client;

    QPushButton *uploadButton;
    QProgressBar *progressBar;
    QListWidget *resultsList;

    QString currentBatchId;
    int totalImages = 0;
    int processedImages = 0;
};
