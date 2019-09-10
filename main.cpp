#include "src/AVCBuffer.h"
#include "src/AVCPlayer.h"
#include <stdio.h>

#define MAX_BUFFER_SIZE (1024 * 1024)
#define BULK_SIZE 32768

int main(int argc, char **argv)
{
    if(!argv[1]){
        printf(">>>Filepath is NULL\n");
        printf(">>>For example: AVCPlayer xxx.h264\n");
        return 0;
    }
    const char *filename = argv[1];
    FILE* video_fp = fopen(filename, "rb");
    unsigned char data_buffer[MAX_BUFFER_SIZE];

    AVCBuffer* buffer = new AVCBuffer();
    buffer->SetBuffer(data_buffer);

    AVCPlayer* player = new AVCPlayer();
    player->InitPlayer();

    while(true) {
        unsigned char data[BULK_SIZE];
        int len = fread(data, 1, BULK_SIZE, video_fp);
        if (len <= 0){
            printf(">>>eos reach or data is null\n");
            break;
        }

        buffer->AppendBuffer(data, len);

        while(true) {
            int i = buffer->IsStartCode();
            if (i == 0)
            break;

            player->FeedOneNALU(buffer->GetBuffer(), i);
            buffer->DisposeOneNALU(i);
        }
    }
    printf(">>>releasing...\n");
    player->Dispose();
    delete buffer;
    buffer = NULL;
    delete player;
    player = NULL;
    fclose(video_fp);
    video_fp = NULL;
    return 0;
}
