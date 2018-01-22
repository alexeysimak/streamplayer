#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "StreamPlayerApp.h"
#include "StreamPlayerMain.h"
#include "StreamReceiver.h"

IMPLEMENT_APP(StreamPlayerApp);

bool StreamPlayerApp::OnInit()
{
    if(!StreamReceiver::Init()) {
        return false;
    }

    StreamPlayerFrame* frame = new StreamPlayerFrame(0L, _("StreamPlayer"));
    frame->Show();
    return true;
}

int StreamPlayerApp::OnExit()
{
    StreamReceiver::Shutdown();
    return 0;
}
