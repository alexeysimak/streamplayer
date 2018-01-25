#ifndef IFRAMEHANDLER_H_INCLUDED
#define IFRAMEHANDLER_H_INCLUDED

struct Frame
{
    int width;
    int height;
    unsigned char* data;
    int datasize;
};


class IFrameHandler
{
public:
    virtual void OnFrame(Frame* frame) = 0;
    virtual void OnError(const char* error) = 0;
    virtual void OnLogMsg(const char* msg) = 0;
};


#endif // IFRAMEHANDLER_H_INCLUDED
