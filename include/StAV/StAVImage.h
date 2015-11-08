/**
 * Copyright © 2011-2015 Kirill Gavrilov <kirill@sview.ru>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file license-boost.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __StAVImage_h_
#define __StAVImage_h_

#include <StImage/StImageFile.h>
#include <StAV/StAVFrame.h>

struct AVInputFormat;
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;

// define StHandle template specialization
class StAVImage;
ST_DEFINE_HANDLE(StAVImage, StImageFile);

class StAVFrameCounter;
ST_DEFINE_HANDLE(StAVFrameCounter, StBufferCounter);

/**
 * This class implements image load/save operation using libav* libraries.
 */
class StAVImage : public StImageFile {

        public:

    /**
     * Should be called at application start.
     * Short-link to the stLibAV::init().
     */
    ST_CPPEXPORT static bool init();

    /**
     * Resize image using swscale library from FFmpeg.
     * There are several restriction:
     * - Destination image should have the same format (this method is for scaling, not conversion).
     * - Memory should be properly aligned.
     */
    ST_CPPEXPORT static bool resize(const StImage& theImageFrom,
                                    StImage&       theImageTo);

        public:

    /**
     * Initialize empty image.
     */
    ST_CPPEXPORT StAVImage();

    /**
     * Destructor.
     */
    ST_CPPEXPORT virtual ~StAVImage();

    /**
     * Close currently opened image context and release memory.
     */
    ST_CPPEXPORT virtual void close();

    /**
     * Decode image from specified file or memory pointer.
     */
    ST_CPPEXPORT virtual bool load(const StString& theFilePath,
                                   ImageType       theImageType = ST_TYPE_NONE,
                                   uint8_t* theDataPtr = NULL, int theDataSize = 0);

    /**
     * Save image to specified path.
     */
    ST_CPPEXPORT virtual bool save(const StString& theFilePath,
                                   ImageType       theImageType,
                                   StFormat        theSrcFormat = StFormat_AUTO);

        private:

    ST_LOCAL static int getAVPixelFormat(const StImage& theImage);

    /**
     * Close currently opened image context and release memory.
     */
    ST_LOCAL void closeAvCtx();

        private:

    AVInputFormat*   myImageFormat; //!< image format
    AVFormatContext* myFormatCtx;   //!< file context
    AVCodecContext*  myCodecCtx;    //!< codec context
    AVCodec*         myCodec;       //!< codec
    StAVFrame        myFrame;

};

/**
 * Pass reference-counted AVFrame (e.g. AVBuffer) into StImage without data copying.
 */
class StAVFrameCounter : public StBufferCounter {

        public:

    /**
     * Default constructor.
     */
    ST_CPPEXPORT StAVFrameCounter();

    /**
     * Wrapper constructor.
     */
    ST_CPPEXPORT StAVFrameCounter(AVFrame* theFrame);

    /**
     * Create the new reference (e.g. increment counter).
     * If theOther has the same type, than the ref counter will be reused.
     * Otherwise then new counter will be allocated.
     */
    ST_CPPEXPORT virtual void createReference(StHandle<StBufferCounter>& theOther) const;

    /**
     * Release current reference.
     */
    ST_CPPEXPORT virtual void releaseReference();

    /**
     * Release reference counter.
     */
    ST_CPPEXPORT virtual ~StAVFrameCounter();

        private:

    AVFrame* myFrame; //!< frame
    bool     myToRelease;

};

#endif // __StAVImage_h_
