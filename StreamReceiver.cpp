#include "StreamReceiver.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};


StreamReceiver::StreamReceiver()
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
/*
    avformat_network_deinit();
*/
    return true;
}
