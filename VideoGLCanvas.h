#ifndef VIDEOGLCANVAS_H
#define VIDEOGLCANVAS_H

#include "wx/glcanvas.h"

class VideoGLCanvas : public wxGLCanvas
{
public:
    VideoGLCanvas(wxWindow *parent, int *attribList = NULL);
    virtual ~VideoGLCanvas();

private:
    void OnPaint(wxPaintEvent& event);
    void OnSize( wxSizeEvent& event );

    wxDECLARE_EVENT_TABLE();

private:
    wxGLContext* m_context;
};

#endif // VIDEOGLCANVAS_H
