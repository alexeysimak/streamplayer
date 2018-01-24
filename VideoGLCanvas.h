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

private:
    void OnPaint(wxPaintEvent& event);
    void OnSize( wxSizeEvent& event );

    wxDECLARE_EVENT_TABLE();

private:
    wxGLContext* m_context;
};

#endif // VIDEOGLCANVAS_H
