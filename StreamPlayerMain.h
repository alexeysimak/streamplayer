#ifndef STREAMPLAYERMAIN_H
#define STREAMPLAYERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "StreamPlayerApp.h"
#include "VideoGLCanvas.h"

class StreamPlayerFrame: public wxFrame
{
public:
    StreamPlayerFrame(wxFrame *frame, const wxString& title);
    ~StreamPlayerFrame();

private:
    enum
    {
        idMenuQuit = 1000,
        idMenuAbout,
        idButtonPlay,
        idButtonStop,
        idTextBoxURL,
        idWindowVideo
    };
    void OnClose(wxCloseEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPlay(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()

private:
    wxBitmapButton* m_buttonPlay;
    wxBitmapButton* m_buttonStop;
    wxTextCtrl* m_textboxURL;
    wxPanel* m_windowVideo;
    VideoGLCanvas* m_videoCanvas;
};


#endif // STREAMPLAYERMAIN_H
