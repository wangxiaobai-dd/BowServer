#ifndef BUFFER_H
#define BUFFER_H

#define INIT_BUFFER_SIZE 65536

// 数据缓冲区
struct buffer 
{
    char* data;          // 实际缓冲
    int readIndex;       // 缓冲读取位置
    int writeIndex;      // 缓冲写入位置
    int total_size;      // 总大小

    int buffer_writeable_size();
    int buffer_readable_size();
    int buffer_front_spare_size();
    void make_room(int size);
    
    void buffer_append(void* data, int size);
    void buffer_append_char(char c);
    void buffer_append_string(char* data);

    int buffer_socket_read(int fd);
    char buffer_read_char();

    static struct buffer* buffer_new();
    static void buffer_free(struct buffer* buffer);
};





//读buffer数据
char buffer_read_char(struct buffer *buffer);

//查询buffer数据
char * buffer_find_CRLF(struct buffer * buffer);

#endif
