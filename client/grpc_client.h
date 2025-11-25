#pragma once

#include "ocr.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <QObject>
#include <QByteArray>

class OCRClient : public QObject {
    Q_OBJECT

public:
    OCRClient();
    void sendImage(QString batchId, QString imageId, QByteArray data);

signals:
    void resultReady(QString imageId, QString text);

private:
    std::unique_ptr<ocr::OCRService::Stub> stub;
    std::shared_ptr<grpc::Channel> channel;

    std::unique_ptr<grpc::ClientReaderWriter<ocr::ImageRequest, ocr::OcrResult>> stream;

    void listenForResponses();
};
