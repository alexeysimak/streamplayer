#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H

#include "IFrameHandler.h"
#include <wx/thread.h>

class StreamReceiver
{
public:
    StreamReceiver(IFrameHandler* handler);
    virtual ~StreamReceiver();

public:
    // must be called once at program start
    static bool Init();

    // must be called once at program exit
    static bool Shutdown();

    // start receiving RTSP stream
    bool Start(const char* url);

    // stop receiving RTSP stream
    bool Stop();

private:
    class WorkerThread : public wxThread
    {
        public:
            WorkerThread(StreamReceiver* owner) : wxThread(wxTHREAD_JOINABLE), m_pOwner(owner) {}

        protected:
            virtual ExitCode Entry()
            {
                m_pOwner->WorkerThreadProc();
                return (wxThread::ExitCode)0;
            }

        private:
            StreamReceiver* m_pOwner;
    };

private:
    void WorkerThreadProc();

private:
    IFrameHandler* m_pFrameHandler;
    WorkerThread* m_pWorkerThread;
    bool m_bStopWorkerThread;

};

#endif // STREAMRECEIVER_H
