#ifndef _AVC_PLAYER_H_
#define _AVC_PLAYER_H_

#include <unistd.h>
#include <pthread.h>
#include <queue>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/ICrypto.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaCodec.h>
#include <media/stagefright/MediaCodecList.h>
#include <media/stagefright/MediaDefs.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/Surface.h>
#include <ui/DisplayInfo.h>

#define BG_WIDTH 1280
#define BG_HEIGHT 720

using namespace android;

class AVCPlayer{
    public:
        AVCPlayer(){}
        int InitPlayer();
        int FeedOneNALU(unsigned char* buffer, int size);
        void GetBackground();
        void Prepare();
        void Scale();
        void Render();
        static void* RenderThread(void* arg);
        void Dispose();

        sp<MediaCodec> mCodec;
        Vector<sp<ABuffer> > mInBuffers;
        Vector<sp<ABuffer> > mOutBuffers;
        sp<SurfaceComposerClient> mComposerClient;
        sp<SurfaceControl> mControl;
        sp<SurfaceControl> mControlBG;
        sp<Surface> mSurface;
        sp<ALooper> mLooper;
        sp<AMessage> mFormat;
    private:
        int mWidth;
        int mHeight;
        bool mRendering;
};
#endif //_AVC_PLAYER_H_
