#include "stream.h"

void stream_open(struct Stream* stream, const char* name)
{
    stream->pos = 0;
    stream->file = fopen(name, "rb");
    fseek(stream->file, 0, SEEK_END);
    stream->size = ftell(stream->file);
    fseek(stream->file, 0, SEEK_SET);
}

void stream_close(struct Stream* stream)
{
    free(stream->file);
}

void stream_seek(struct Stream* stream, int offset)
{
    fseek(stream->file, offset, SEEK_CUR);
    stream->pos += offset;
}

int stream_read(struct Stream* stream, void *data, int count)
{
    int res = fread(data, 1, count, stream->file);
    stream->pos += res;
    return res;
}

char* stream_read_str(struct Stream* stream)
{
    int len;
    stream_read(stream, &len, sizeof(int));
    if (len == 0)
        return NULL;
    char *str = new char[len + 1]; // TODO: free str?
    stream_read(stream, str, len);
    str[len] = 0;
    return str;
}
