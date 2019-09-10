#include "AVCBuffer.h"

void AVCBuffer::SetBuffer(unsigned char* buffer)
{
    mBuffer = buffer;
}

void AVCBuffer::AppendBuffer(unsigned char* buffer, int len)
{
    memcpy(mBuffer + mTotalLength, buffer, len);
    mTotalLength += len;
}

int AVCBuffer::IsStartCode()
{
    int count = 0;
    for (int i = 4; i < mTotalLength; i++) {
        switch(count) {
            case 0:
            case 1:
            case 2:
            if (mBuffer[i] == 0) {
                count++;
            } else {
                count = 0;
            }
            break;
            case 3:
            if (mBuffer[i] == 1) {
                return i - 3;
            } else {
                count = 0;
            }
        }
    }

    return 0;
}

unsigned char* AVCBuffer::GetBuffer()
{
    return mBuffer;
}

void AVCBuffer::DisposeOneNALU(int len)
{
    memmove(mBuffer, mBuffer + len, mTotalLength - len);
    mTotalLength -= len;
}
