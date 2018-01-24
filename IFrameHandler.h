#ifndef IFRAMEHANDLER_H_INCLUDED
#define IFRAMEHANDLER_H_INCLUDED

struct Frame
{
    enum Type { BMP };

    Type type;
    unsigned char* data;
    unsigned int size;
};


class IFrameHandler
{
public:
    virtual void OnFrame(Frame* frame) = 0;
};


#endif // IFRAMEHANDLER_H_INCLUDED
