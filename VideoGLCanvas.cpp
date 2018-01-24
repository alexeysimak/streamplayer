#include "VideoGLCanvas.h"

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_GLCANVAS
    #error "OpenGL required: set wxUSE_GLCANVAS to 1 and rebuild the library"
#endif


wxBEGIN_EVENT_TABLE(VideoGLCanvas, wxGLCanvas)
    EVT_PAINT(VideoGLCanvas::OnPaint)
    EVT_SIZE( VideoGLCanvas::OnSize )
wxEND_EVENT_TABLE()

// function to draw the texture for cube faces
static wxImage DrawDice(int size, unsigned num)
{
    wxASSERT_MSG( num >= 1 && num <= 6, wxT("invalid dice index") );

    const int dot = size/16;        // radius of a single dot
    const int gap = 5*size/32;      // gap between dots

    wxBitmap bmp(size, size);
    wxMemoryDC dc;
    dc.SelectObject(bmp);
    dc.SetBackground(*wxGREEN_BRUSH);
    dc.Clear();
    dc.SetBrush(*wxBLACK_BRUSH);

    // the upper left and lower right points
    if ( num != 1 )
    {
        dc.DrawCircle(gap + dot, gap + dot, dot);
        dc.DrawCircle(size - gap - dot, size - gap - dot, dot);
    }

    // draw the central point for odd dices
    if ( num % 2 )
    {
        dc.DrawCircle(size/2, size/2, dot);
    }

    // the upper right and lower left points
    if ( num > 3 )
    {
        dc.DrawCircle(size - gap - dot, gap + dot, dot);
        dc.DrawCircle(gap + dot, size - gap - dot, dot);
    }

    // finally those 2 are only for the last dice
    if ( num == 6 )
    {
        dc.DrawCircle(gap + dot, size/2, dot);
        dc.DrawCircle(size - gap - dot, size/2, dot);
    }

    dc.SelectObject(wxNullBitmap);

    return bmp.ConvertToImage();
}

VideoGLCanvas::VideoGLCanvas(wxWindow *parent, int *attribList)
    : wxGLCanvas(parent, wxID_ANY, attribList, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

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
	delete m_context;
}

void VideoGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    // This is required even though dc is not used otherwise.
    wxPaintDC dc(this);

    wxGLCanvas::SetCurrent(*m_context);

    int w = GetSize().GetWidth();
    int h = GetSize().GetHeight();

    glViewport( 0, 0, w, h ); // use a screen size of WIDTH x HEIGHT
    glEnable(GL_TEXTURE_2D);

    const wxImage img( DrawDice( 512, 5 ) );

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.GetWidth(), img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetData());

    glBegin( GL_QUADS );
        glTexCoord2i( 0, 0 ); glVertex2i( 0, 0 );
        glTexCoord2i( 0, 1 ); glVertex2i( 0, h );
        glTexCoord2i( 1, 1 ); glVertex2i( w, h );
        glTexCoord2i( 1, 0 ); glVertex2i( w, 0 );
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glFlush();

    SwapBuffers();
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

void VideoGLCanvas::OnFrame(Frame* frame)
{

}

