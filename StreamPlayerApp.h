#ifndef STREAMPLAYERAPP_H
#define STREAMPLAYERAPP_H

#include <wx/app.h>

class StreamPlayerApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

#endif // STREAMPLAYERAPP_H
