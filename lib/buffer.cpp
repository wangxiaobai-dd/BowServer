#include "buffer.h"
#include "common.h"

struct buffer* buffer::buffer_new() 
{
    struct buffer* buf = (struct buffer*)malloc(sizeof(struct buffer));
    if (!buf)
        return nullptr;

    buf->data = (char*)malloc(INIT_BUFFER_SIZE);
    buf->total_size = INIT_BUFFER_SIZE;
    buf->readIndex = 0;
    buf->writeIndex = 0;
    return buf;
}

void buffer::buffer_free(struct buffer* buf) 
{
    free(buf->data);
    free(buf);
}

// 可写长度
int buffer::buffer_writeable_size() 
{
    return total_size - writeIndex;
}

// 可读长度
int buffer::buffer_readable_size()
{
    return writeIndex - readIndex;
}

// 已读数据
int buffer::buffer_front_spare_size() 
{
    return readIndex;
}

// 空间调整
void buffer::make_room(int size)
{
    if (buffer_writeable_size() >= size) 
        return;
    // 如果front_spare和writeable的大小加起来可以容纳数据，则把可读数据往前面拷贝
    if (buffer_front_spare_size() + buffer_writeable_size() >= size) 
    {
        int readable = buffer_readable_size();
        int i;
	for (i = 0; i < readable; i++) 
	    memcpy(data + i, data + readIndex + i, 1);
        readIndex = 0;
        writeIndex = readable;
    } 
    else 
    {
        //扩大缓冲区
        void* tmp = realloc(data, total_size + size);
        if (tmp == nullptr)
            return;
        data = (char*)tmp;
        total_size += size;
    }
}

void buffer::buffer_append(void* data, int size) 
{
    if (data) 
    {
        make_room(size);
        memcpy(data + writeIndex, data, size);
        writeIndex += size;
    }
}

void buffer::buffer_append_char(char c) 
{
    make_room(1);
    //拷贝数据到可写空间中
    data[writeIndex++] = c;
}

void buffer::buffer_append_string(char* data) 
{
    if (data) 
    {
        int size = strlen(data);
        buffer_append(data, size);
    }
}

int buffer::buffer_socket_read(int fd) 
{
    char additional_buffer[INIT_BUFFER_SIZE];
    struct iovec vec[2];
    int max_writable = buffer_writeable_size();
    vec[0].iov_base = data + writeIndex;
    vec[0].iov_len = max_writable;
    vec[1].iov_base = additional_buffer;
    vec[1].iov_len = sizeof(additional_buffer);
    int result = readv(fd, vec, 2);
    if (result < 0)
        return -1;
    else if (result <= max_writable)
        writeIndex += result;
    else
    {
        writeIndex = total_size;
        buffer_append(additional_buffer, result - max_writable);
    }
    return result;
}

char buffer::buffer_read_char()
{
    char c = data[readIndex];
    readIndex++;
    return c;
}

