#ifndef STREAM_H_
#define STREAM_H_

#include <cstdio>
#include <cstdlib>

struct Stream {
    FILE *file;
    int  pos;
    int  size;
};

void stream_open(struct Stream* stream, const char* name);
void stream_close(struct Stream* stream);
void stream_seek(struct Stream* stream, int offset);
int stream_read(struct Stream* stream, void *data, int count);
char* stream_read_str(struct Stream* stream);
#endif
