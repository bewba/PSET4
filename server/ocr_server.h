#pragma once

#include "ocr.grpc.pb.h"
#include "worker_pool.h"
#include <tesseract/baseapi.h>
#include <mutex>

class OCRServiceImpl final : public ocr::OCRService::Service {
public:
    OCRServiceImpl();

    grpc::Status SendImage(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<ocr::OcrResult, ocr::ImageRequest>* stream
    ) override;

private:
    WorkerPool pool;
};
