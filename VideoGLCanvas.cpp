#include "VideoGLCanvas.h"
#include "StreamPlayerMain.h"
// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif

wxDEFINE_EVENT(wxNewVideoFrameEvent, wxCommandEvent);
wxDEFINE_EVENT(wxStreamErrorEvent, wxCommandEvent);

wxBEGIN_EVENT_TABLE(VideoGLCanvas, wxGLCanvas)
    EVT_PAINT(VideoGLCanvas::OnPaint)
    EVT_SIZE( VideoGLCanvas::OnSize )
    EVT_COMMAND(wxID_ANY, wxNewVideoFrameEvent, VideoGLCanvas::OnNewVideoFrame)
    EVT_COMMAND(wxID_ANY, wxStreamErrorEvent, VideoGLCanvas::OnStreamError)
wxEND_EVENT_TABLE()


VideoGLCanvas::VideoGLCanvas(wxWindow *parent, wxFrame* pMainFrame, int *attribList)
: wxGLCanvas(parent, wxID_ANY, attribList, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
, m_pMainFrame(pMainFrame)
, m_pFrameBuffer(NULL)
, m_nFrameBufferSize(0)
{
    memset(&m_CurFrame, 0, sizeof(Frame));

    m_pGLContext = new wxGLContext(this);

    int w = GetSize().GetWidth();
    int h = GetSize().GetHeight();

    glViewport( 0, 0, w, h ); // use a screen size of WIDTH x HEIGHT
    glEnable( GL_TEXTURE_2D );     // Enable 2D texturing

    glMatrixMode( GL_PROJECTION );     // Make a simple 2D projection on the entire window
    glLoadIdentity();
    glOrtho( 0.0, w, h, 0.0, 0.0, 100.0 );

    glMatrixMode( GL_MODELVIEW );    // Set the matrix mode to object modeling
    glLoadIdentity();

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear the window
}

VideoGLCanvas::~VideoGLCanvas()
{
	delete m_pGLContext;
    if (m_pFrameBuffer) {
        delete[] m_pFrameBuffer;
        m_pFrameBuffer = NULL;
    }
}

void VideoGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    // This is required even though dc is not used otherwise.
    wxPaintDC dc(this);

    wxGLCanvas::SetCurrent(*m_pGLContext);

    int w = GetSize().GetWidth();
    int h = GetSize().GetHeight();

    glViewport( 0, 0, w, h ); // use a screen size of WIDTH x HEIGHT
    glEnable(GL_TEXTURE_2D);

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear the window

    m_FrameMutex.Lock();

    if ( m_CurFrame.datasize != 0 ) {

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_CurFrame.width, m_CurFrame.height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_CurFrame.data);
        glBegin( GL_QUADS );
            glTexCoord2i( 0, 0 ); glVertex2i( 0, 0 );
            glTexCoord2i( 0, 1 ); glVertex2i( 0, h );
            glTexCoord2i( 1, 1 ); glVertex2i( w, h );
            glTexCoord2i( 1, 0 ); glVertex2i( w, 0 );
        glEnd();

    }

    glDisable(GL_TEXTURE_2D);

    glFlush();

    SwapBuffers();

    m_FrameMutex.Unlock();
}

void VideoGLCanvas::OnSize( wxSizeEvent& event )
{
    wxSize size = GetSize();
    glViewport( 0, 0, size.GetWidth(), size.GetHeight() );
    glMatrixMode( GL_PROJECTION );     // Make a simple 2D projection on the entire window
    glLoadIdentity();
    glOrtho( 0.0, size.GetWidth(), size.GetHeight(), 0.0, 0.0, 100.0 );
    Refresh();
}

void VideoGLCanvas::OnNewVideoFrame(wxCommandEvent& event)
{
    Refresh();
}

void VideoGLCanvas::OnStreamError(wxCommandEvent& event)
{
    ((StreamPlayerFrame*)m_pMainFrame)->OnStreamError(event);
}

void VideoGLCanvas::OnFrame(Frame* frame)
{
    m_FrameMutex.Lock();

    AllocateFrameBuffer(frame->datasize);
    memcpy(m_pFrameBuffer, frame->data, frame->datasize);
    m_CurFrame.data = m_pFrameBuffer;
    m_CurFrame.width = frame->width;
    m_CurFrame.height = frame->height;
    m_CurFrame.datasize = frame->datasize;

    m_FrameMutex.Unlock();

    // notify UI thread that video panel need to be repainted
    wxCommandEvent event(wxNewVideoFrameEvent);
    wxPostEvent(this, event);
}

void VideoGLCanvas::OnError(const char* error)
{
    // send message to the UI thread
    wxCommandEvent event(wxStreamErrorEvent);
    event.SetString( error );
    wxPostEvent(this, event);
}

void VideoGLCanvas::AllocateFrameBuffer(int size)
{
    if (size > m_nFrameBufferSize) {
        if (m_pFrameBuffer) {
            delete[] m_pFrameBuffer;
        }
        m_pFrameBuffer = new unsigned char[size];
        m_nFrameBufferSize = size;
    }
}

void VideoGLCanvas::Clear()
{
    m_FrameMutex.Lock();
    memset(&m_CurFrame, 0, sizeof(Frame));
    m_FrameMutex.Unlock();
    Refresh();
}



