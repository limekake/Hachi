#ifndef PACKETS
#define PACKETS

struct RESPONSE_LOGIN
{
    int session_id;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(session_id);
    }
};

struct REQUEST_LOGIN
{
    int session_id;
    char username[20];

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(session_id, username);
    }
};

struct REQUEST_MOVE
{
    int x;
    int y;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(x, y);
    }
};

#endif