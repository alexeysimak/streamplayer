#ifndef VIDEOGLCANVAS_H
#define VIDEOGLCANVAS_H

#include <wx/glcanvas.h>
#include "IFrameHandler.h"

class VideoGLCanvas : public wxGLCanvas
                    , public IFrameHandler
{
public:
    VideoGLCanvas(wxWindow* parent, wxFrame* pMainFrame, int *attribList = NULL);
    virtual ~VideoGLCanvas();

    // IFrameHandler implementation
    virtual void OnFrame(Frame* frame);
    virtual void OnError(const char* error);

    // must be called from UI thread
    void Clear();

private:
    // event handlers
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnNewVideoFrame(wxCommandEvent& event);
    void OnStreamError(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();

private:
    void AllocateFrameBuffer(int size);

private:
    wxFrame* m_pMainFrame;
    wxGLContext* m_pGLContext;
    unsigned char* m_pFrameBuffer;
    int m_nFrameBufferSize;
    Frame m_CurFrame;
    wxMutex m_FrameMutex;
};

#endif // VIDEOGLCANVAS_H
