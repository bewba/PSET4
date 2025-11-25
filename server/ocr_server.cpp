#include "ocr_server.h"
#include <leptonica/allheaders.h>

OCRServiceImpl::OCRServiceImpl() : pool(4) {}  // 4 worker threads

grpc::Status OCRServiceImpl::SendImage(
        grpc::ServerContext* context,
        grpc::ServerReaderWriter<ocr::OcrResult, ocr::ImageRequest>* stream) 
{
    ocr::ImageRequest req;

    while (stream->Read(&req)) {
        std::string batch = req.batch_id();
        std::string imgId = req.image_id();
        std::string raw = req.data();


        pool.enqueue([stream, batch, imgId, raw]() {
            // Load image
            Pix *image = pixReadMem(reinterpret_cast<const unsigned char*>(raw.data()), raw.size());

            tesseract::TessBaseAPI api;
            api.Init(NULL, "eng");
            api.SetImage(image);

            char* outText = api.GetUTF8Text();

            ocr::OcrResult res;
            res.set_batch_id(batch);
            res.set_image_id(imgId);
            res.set_text(outText);

            stream->Write(res);

            api.End();
            pixDestroy(&image);
            delete[] outText;
        });
    }

    return grpc::Status::OK;
}
