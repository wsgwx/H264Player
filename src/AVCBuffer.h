#ifndef _AVC_BUFFER_H_
#define _AVC_BUFFER_H_

#include <string.h>

class AVCBuffer{
    public:
        AVCBuffer(){
            mTotalLength = 0;
        }
        void SetBuffer(unsigned char* buffer);
        void AppendBuffer(unsigned char* buffer, int len);
        int IsStartCode();
        unsigned char* GetBuffer();
        void DisposeOneNALU(int len);
    
    private:
        unsigned char* mBuffer;
        int mTotalLength;
};
#endif //_AVC_BUFFER_H_
