#ifndef VIDEOGLCANVAS_H
#define VIDEOGLCANVAS_H

#include <wx/glcanvas.h>
#include "IFrameHandler.h"

class VideoGLCanvas : public wxGLCanvas
                    , public IFrameHandler
{
public:
    VideoGLCanvas(wxWindow *parent, int *attribList = NULL);
    virtual ~VideoGLCanvas();

    // IFrameHandler implementation
    virtual void OnFrame(Frame* frame);
    virtual void OnError(const char* error);
    virtual void OnLogMsg(const char* msg);

    void Clear();

private:
    // event handlers
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnNewVideoFrame(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

private:
    void AllocateFrameBuffer(int size);

private:
    wxGLContext* m_pGLContext;
    unsigned char* m_pFrameBuffer;
    int m_nFrameBufferSize;
    Frame m_CurFrame;
    wxMutex m_FrameMutex;
};

#endif // VIDEOGLCANVAS_H
