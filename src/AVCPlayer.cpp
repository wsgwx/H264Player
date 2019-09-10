#include "AVCPlayer.h"
#include <android/native_window.h>

int AVCPlayer::InitPlayer()
{
    mWidth = BG_WIDTH;
    mHeight = BG_HEIGHT;
    mRendering = true;

    ProcessState::self()->startThreadPool();
    DataSource::RegisterDefaultSniffers();

    mFormat = new AMessage;
    mLooper = new ALooper;
    mLooper->start();

    mComposerClient = new SurfaceComposerClient;
    CHECK_EQ(mComposerClient->initCheck(), (status_t)OK);
    
    mControl = mComposerClient->createSurface(
        String8("A Surface"),
        mWidth,
        mHeight,
        PIXEL_FORMAT_RGB_565,
        0);
    CHECK(mControl != NULL);
    CHECK(mControl->isValid());
    
    SurfaceComposerClient::openGlobalTransaction();
    CHECK_EQ(mControl->setLayer(INT_MAX), (status_t)OK);
    CHECK_EQ(mControl->show(), (status_t)OK);
    
    SurfaceComposerClient::closeGlobalTransaction();
    mSurface = mControl->getSurface();
    CHECK(mSurface != NULL);

    GetBackground();
    Prepare();
    
    pthread_t tid;
    pthread_create(&tid, NULL, RenderThread, this);
    
    return 0;
}

void AVCPlayer::GetBackground()
{
    mControlBG = mComposerClient->createSurface(
        String8("A Surface"),
        mWidth,
        mHeight,
        PIXEL_FORMAT_RGB_565,
        0);

    CHECK(mControlBG != NULL);
    CHECK(mControlBG->isValid());

    SurfaceComposerClient::openGlobalTransaction();
    CHECK_EQ(mControlBG->setLayer(INT_MAX - 1), (status_t)OK);
    CHECK_EQ(mControlBG->show(), (status_t)OK);
    SurfaceComposerClient::closeGlobalTransaction();

    sp<Surface> surface = mControlBG->getSurface();
    CHECK(surface != NULL);
    ANativeWindow_Buffer ob;
    surface->lock(&ob,NULL);
    surface->unlockAndPost();
}

void AVCPlayer::Prepare()
{
    mCodec = MediaCodec::CreateByType(
        mLooper, "video/avc", false);

    sp<AMessage> format = new AMessage;
    format->setString("mime", "video/avc");
    format->setInt32("width", mWidth);
    format->setInt32("height", mHeight);

    mCodec->configure(
        format,
        mSurface,
        NULL,
        0);

    CHECK_EQ((status_t)OK, mCodec->start());
    CHECK_EQ((status_t)OK, mCodec->getInputBuffers(&mInBuffers));
    CHECK_EQ((status_t)OK, mCodec->getOutputBuffers(&mOutBuffers));
}

void AVCPlayer::Scale()
{
    mCodec->getOutputFormat(&mFormat);

    int width, height;
    if (mFormat->findInt32("width", &width) &&
        mFormat->findInt32("height", &height)) {
            float scale_x = (BG_WIDTH + 0.0) / width;
            float scale_y = (BG_HEIGHT + 0.0) / height;
            float scale = (scale_x < scale_y) ? scale_x : scale_y;

            scale = (scale > 1) ? 1 : scale;

            if (scale < 1) {
                int new_width = width * scale;
                int new_height = height * scale;

                new_width = (new_width > BG_WIDTH) ? BG_WIDTH : new_width;
                new_height = (new_height > BG_HEIGHT) ? BG_HEIGHT : new_height;

                width = new_width;
                height = new_height;
            }

            if (width > BG_WIDTH)
            width = BG_WIDTH;

            if (height > BG_HEIGHT)
            height = BG_HEIGHT;

            if (width != mWidth || height != mHeight) {
                mWidth = width;
                mHeight = height;

                int x = (BG_WIDTH - width) / 2;
                int y = (BG_HEIGHT - height) / 2;

                SurfaceComposerClient::openGlobalTransaction();
                mControl->setSize(width, height);
                mControl->setPosition(x, y);
                SurfaceComposerClient::closeGlobalTransaction();
            }
        }
}

void AVCPlayer::Render()
{
    size_t index, offset, size;
    int64_t pts;
    uint32_t flags;

    int err;
    do {
        Scale();

        err = mCodec->dequeueOutputBuffer(
            &index, &offset, &size, &pts, &flags);

        if (err == OK) {
            mCodec->renderOutputBufferAndRelease(
                index);
        }
    } while(err == OK
            || err == INFO_FORMAT_CHANGED
            || err == INFO_OUTPUT_BUFFERS_CHANGED);
}

void* AVCPlayer::RenderThread(void* arg)
{
    AVCPlayer* player = (AVCPlayer*)arg;

    while(player->mRendering) {
        player->Render();
    }

    return NULL;
}

int AVCPlayer::FeedOneNALU(unsigned char* buffer, int size)
{
    size_t index;

    int err = mCodec->dequeueInputBuffer(&index, -1ll);
    CHECK_EQ(err, (status_t)OK);

    const sp<ABuffer> &dst = mInBuffers.itemAt(index);

    CHECK_LE(size, dst->capacity());

    dst->setRange(0, size);
    memcpy(dst->data(), buffer, size);

    err = mCodec->queueInputBuffer(
        index,
        0,
        size,
        0ll,
        0);
    
    return err;
}

void AVCPlayer::Dispose()
{
    mCodec->stop();
    mCodec->reset();
    mCodec->release();
    mLooper->stop();

    mRendering = false;
    SurfaceComposerClient::openGlobalTransaction();
    CHECK_EQ(mControl->hide(), (status_t)OK);
    CHECK_EQ(mControlBG->hide(), (status_t)OK);
    SurfaceComposerClient::closeGlobalTransaction();

    mComposerClient->dispose();
    mControl->clear();
    mControlBG->clear();
}
