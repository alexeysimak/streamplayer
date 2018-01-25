#ifndef STREAMPLAYERMAIN_H
#define STREAMPLAYERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "StreamPlayerApp.h"
#include "VideoGLCanvas.h"
#include "StreamReceiver.h"

class StreamPlayerFrame: public wxFrame
{
public:
    StreamPlayerFrame(wxFrame *frame, const wxString& title);
    ~StreamPlayerFrame();

    void OnStreamError(wxCommandEvent& event);

private:
    enum
    {
        idMenuQuit = 1000,
        idMenuAbout,
        idButtonPlay,
        idButtonStop,
        idTextBoxURL,
        idPanelVideo
    };

    // event handlers
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
    wxPanel* m_panelVideo;
    VideoGLCanvas* m_videoCanvas;
    StreamReceiver* m_pStreamReceiver;
};


#endif // STREAMPLAYERMAIN_H
