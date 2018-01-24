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
    while ( !m_bStopWorkerThread ) {
        wxSleep(1);
    }
}

