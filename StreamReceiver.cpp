#include "StreamReceiver.h"
#include <wx/utils.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};

// ----------------------------------------------------------------------------
// StreamReceiver implementation
// ----------------------------------------------------------------------------

StreamReceiver::StreamReceiver(IFrameHandler* handler)
: m_pFrameHandler(handler)
, m_pWorkerThread(NULL)
, m_bStopWorkerThread(false)
{
}


StreamReceiver::~StreamReceiver()
{
}


bool StreamReceiver::Init()
{
    // init ffmpeg staff
    av_register_all();
    avcodec_register_all();
    avformat_network_init();

    return true;
}


bool StreamReceiver::Shutdown()
{
    avformat_network_deinit();
    return true;
}

bool StreamReceiver::Start(const char* url)
{
    if ( m_pWorkerThread != NULL ) {
        return false; // already started
    }

    m_sStreamURL.assign(url);
    m_bStopWorkerThread = false;
    m_pWorkerThread = new WorkerThread(this);
    m_pWorkerThread->Run();

    return true;
}

bool StreamReceiver::Stop()
{
    if ( m_pWorkerThread == NULL ) {
        return true; // already stopped
    }

    m_bStopWorkerThread = true;
    m_pWorkerThread->Wait();

    delete m_pWorkerThread;
    m_pWorkerThread = NULL;

    return true;
}

void StreamReceiver::WorkerThreadProc()
{
    AVFormatContext *ifcx = NULL;
    AVCodecContext *iccx = NULL;
    AVCodec *icodec = NULL;
    AVFrame *pFrame = NULL;
    AVFrame *pFrameRGB = NULL;

    int numBytes;
    uint8_t *buffer = NULL;
    struct SwsContext *img_convert_ctx = NULL;

    AVPacket pkt;

    //open rtsp
    AVDictionary *opts = 0;
    av_dict_set(&opts, "rtsp_transport", "tcp", 0); // use tcp
    if (avformat_open_input(&ifcx, m_sStreamURL.c_str(), NULL, &opts) != 0) {
        m_pFrameHandler->OnError("Failed to open RTSP stream");
        return;
    }

    if (avformat_find_stream_info(ifcx, NULL) < 0) {
        m_pFrameHandler->OnError("Failed to find stream info");
        avformat_close_input(&ifcx);
        return;
    }

    snprintf(ifcx->filename, sizeof(ifcx->filename), "%s", m_sStreamURL.data().AsChar());

    //search video stream
    int i_index = -1;
    for (unsigned int ix = 0; ix < ifcx->nb_streams; ix++) {
        iccx = ifcx->streams[ix]->codec;
        if (iccx->codec_type == AVMEDIA_TYPE_VIDEO) {
            i_index = ix;
            break;
        }
    }

    if (i_index < 0) {
        m_pFrameHandler->OnError("Failed to find input video stream");
        avformat_close_input(&ifcx);
        return;
    }

    icodec = avcodec_find_decoder(iccx->codec_id);
    if (icodec == NULL) {
        m_pFrameHandler->OnError("Unsupported codec in stream");
        avformat_close_input(&ifcx);
        return;
    }

    avcodec_open2(iccx, icodec, NULL);

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, iccx->width, iccx->height);
    buffer = (uint8_t*)av_malloc(numBytes);
    avpicture_fill((AVPicture *)pFrameRGB, buffer, AV_PIX_FMT_RGB24, iccx->width, iccx->height);
    img_convert_ctx = sws_getCachedContext(img_convert_ctx, iccx->width, iccx->height,
        iccx->pix_fmt, iccx->width, iccx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    av_init_packet(&pkt);

    bool bFirstKeyFound = false;
    while (!m_bStopWorkerThread && av_read_frame(ifcx, &pkt) >= 0) {

        if (pkt.stream_index == i_index) { //packet is video

            // Make sure we start on a key frame
            if (!bFirstKeyFound && !(pkt.flags & AV_PKT_FLAG_KEY)) {
                continue;
            }
            bFirstKeyFound = true;

            int got_picture = 0;
            int ret = avcodec_decode_video2(iccx, pFrame, &got_picture, &pkt);
            if (ret < 0) {
                m_pFrameHandler->OnError("Failed to decode video packet");
                break;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,
                    0, iccx->height, pFrameRGB->data, pFrameRGB->linesize);

                Frame frame;
                frame.data = pFrameRGB->data[0];
                frame.width = iccx->width;
                frame.height = iccx->height;
                frame.datasize = iccx->width * iccx->height * 24 / 8;

                m_pFrameHandler->OnFrame(&frame);
            }
        }

        av_free_packet(&pkt);
        av_init_packet(&pkt);
    }

    av_read_pause(ifcx);
    avformat_close_input(&ifcx);
    av_free_packet(&pkt);
    av_frame_free(&pFrame);
    av_frame_free(&pFrameRGB);
}

