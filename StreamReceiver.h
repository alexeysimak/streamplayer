#ifndef STREAMRECEIVER_H
#define STREAMRECEIVER_H


class StreamReceiver
{
public:
    StreamReceiver();
    virtual ~StreamReceiver();

public:
    // must be called once at program start
    static bool Init();
    // must be called once at program exit
    static bool Shutdown();
};

#endif // STREAMRECEIVER_H
