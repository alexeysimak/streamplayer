#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "StreamPlayerMain.h"


BEGIN_EVENT_TABLE(StreamPlayerFrame, wxFrame)
    EVT_CLOSE(StreamPlayerFrame::OnClose)
    EVT_MENU(idMenuQuit, StreamPlayerFrame::OnQuit)
    EVT_MENU(idMenuAbout, StreamPlayerFrame::OnAbout)
    EVT_BUTTON(idButtonPlay, StreamPlayerFrame::OnPlay)
    EVT_BUTTON(idButtonStop, StreamPlayerFrame::OnStop)
END_EVENT_TABLE()

StreamPlayerFrame::StreamPlayerFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    SetClientSize(800, 600);
    SetMinClientSize(wxSize(400, 300));
    CentreOnScreen();

    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);

    wxBoxSizer* topsizer = new wxBoxSizer( wxVERTICAL );
    this->SetSizer(topsizer);

    wxBoxSizer* hsizer = new wxBoxSizer(wxHORIZONTAL);
    topsizer->Add(hsizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);

    // Add buttons
    wxBitmap bitmap;

    bitmap.LoadFile("./res/btn_play.png", wxBITMAP_TYPE_PNG);
    m_buttonPlay = new wxBitmapButton();
    m_buttonPlay->Create(this, idButtonPlay, bitmap, wxDefaultPosition, wxSize(27, 27));
    m_buttonPlay->SetToolTip("Play");
    m_buttonPlay->Enable();
    hsizer->Add(m_buttonPlay, wxSTRETCH_NOT);

    bitmap.LoadFile("./res/btn_stop.png", wxBITMAP_TYPE_PNG);
    m_buttonStop = new wxBitmapButton();
    m_buttonStop->Create(this, idButtonStop, bitmap, wxDefaultPosition, wxSize(27, 27));
    m_buttonStop->SetToolTip("Stop");
    m_buttonStop->Disable();
    hsizer->Add(m_buttonStop, wxSTRETCH_NOT);

    // Add textbox
    m_textboxURL = new wxTextCtrl();
    m_textboxURL->Create(this, idTextBoxURL, "rtsp://184.72.239.149/vod/mp4:BigBuckBunny_175k.mov");
    hsizer->Add(m_textboxURL, wxEXPAND);

    m_windowVideo = new wxPanel();
    m_windowVideo->Create(this, idWindowVideo, wxDefaultPosition, wxSize(200, 200));
    m_windowVideo->SetBackgroundColour(*wxBLUE);
    topsizer->Add(m_windowVideo, 1, wxEXPAND|wxALL, 5);

    wxBoxSizer* videoSizer = new wxBoxSizer(wxVERTICAL);
    m_windowVideo->SetSizer(videoSizer);

//    wxPanel* panel = new wxPanel();
//    panel->Create(m_windowVideo, -1, wxDefaultPosition, wxSize(200, 200));
//    panel->SetBackgroundColour(*wxGREEN);
//    videoSizer->Add(panel, 1, wxEXPAND);

    m_videoCanvas = new VideoGLCanvas(m_windowVideo);
    videoSizer->Add(m_videoCanvas, 1, wxEXPAND|wxALL, 5 );

    // create a status bar
    CreateStatusBar(2);
    SetStatusText(_("State: Not implemented"),0);
}


StreamPlayerFrame::~StreamPlayerFrame()
{
}

void StreamPlayerFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void StreamPlayerFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void StreamPlayerFrame::OnAbout(wxCommandEvent &event)
{
    wxMessageBox("StreamPlayer demo 1.0", _("About"));
}

void StreamPlayerFrame::OnPlay(wxCommandEvent &event)
{
    m_buttonPlay->Disable();
    m_buttonStop->Enable();
    m_textboxURL->Disable();
}

void StreamPlayerFrame::OnStop(wxCommandEvent &event)
{
    m_buttonPlay->Enable();
    m_buttonStop->Disable();
    m_textboxURL->Enable();
}

