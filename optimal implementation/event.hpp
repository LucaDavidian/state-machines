struct Event
{
    // reserved signals
    enum class Signal { ENTRY = 0, EXIT, RESERVED, SIG1, SIG2, SIG3 }; 
    Signal signal;
};