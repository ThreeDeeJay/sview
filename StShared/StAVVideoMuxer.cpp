/**
 * Copyright © 2015-2025 Kirill Gavrilov <kirill@sview.ru>
 *
 * This code is licensed under MIT license (see docs/license-mit.txt for details).
 */

#include <StAV/StAVVideoMuxer.h>

#include <StAV/StAVPacket.h>
#include <StStrings/StLogger.h>
#include <StFile/StFileNode.h>

#include <vector>

StAVVideoMuxer::StAVVideoMuxer()
: myStereoFormat(StFormat_Mono) {
    //
}

StAVVideoMuxer::~StAVVideoMuxer() {
    close();
}

void StAVVideoMuxer::close() {
    for(size_t aCtxId = 0; aCtxId < myCtxListSrc.size(); ++aCtxId) {
        AVFormatContext*& aFormatCtx = myCtxListSrc[aCtxId];
        if(aFormatCtx != NULL) {
            avformat_close_input(&aFormatCtx);
        }
    }
    myCtxListSrc.clear();
}

bool StAVVideoMuxer::addFile(const StString& theFileToLoad) {
    StString aFileName, aDummy;
    StFileNode::getFolderAndFile(theFileToLoad, aDummy, aFileName);

    AVFormatContext* aFormatCtx = NULL;
    int avErrCode = avformat_open_input(&aFormatCtx, theFileToLoad.toCString(), NULL, NULL);
    if(avErrCode != 0) {
        signals.onError(StString("FFmpeg: Couldn't open video file '") + theFileToLoad
                      + "'\nError: " + stAV::getAVErrorDescription(avErrCode));
        if(aFormatCtx != NULL) {
            avformat_close_input(&aFormatCtx);
        }
        return false;
    }

    // retrieve stream information
    if(avformat_find_stream_info(aFormatCtx, NULL) < 0) {
        signals.onError(StString("FFmpeg: Couldn't find stream information in '") + theFileToLoad + "'");
        if(aFormatCtx != NULL) {
            avformat_close_input(&aFormatCtx);
        }
        return false;
    }

#ifdef ST_DEBUG
    av_dump_format(aFormatCtx, 0, theFileToLoad.toCString(), false);
#endif

    myCtxListSrc.add(aFormatCtx);
    return true;
}

class StAVOutContext {

        public:

    AVFormatContext* Context;

    /**
     * Empty constructor.
     */
    StAVOutContext() : Context(NULL), myFormat(NULL) {}

    /**
     * Determine the format.
     */
    bool findFormat(const char* theShortName,
                    const char* theFilename,
                    const char* theMimeType = NULL) {
        myFormat = av_guess_format(theShortName, theFilename, theMimeType);
        return myFormat != NULL;
    }

    /**
     * Create context.
     */
    bool create(const StString& theFile) {
        if(myFormat == NULL) {
            return false;
        }

        avformat_alloc_output_context2(&Context, const_cast<AVOutputFormat*>(myFormat), NULL, theFile.toCString());
        return Context != NULL;
    }

    /**
     * Destructor.
     */
    ~StAVOutContext() {
        if(Context == NULL) {
            return;
        }

        if(!(Context->oformat->flags & AVFMT_NOFILE)) {
            avio_close(Context->pb);
        }
        avformat_free_context(Context);
    }

        private:

    const AVOutputFormat* myFormat;

};

/**
 * Return string identifier for specified stereo format.
 */
const char* formatToMetadata(const StFormat theFormat) {
    switch(theFormat) {
        case StFormat_Mono:                 return "mono";
        case StFormat_SideBySide_RL:        return "right_left";
        case StFormat_SideBySide_LR:        return "left_right";
        case StFormat_TopBottom_RL:         return "bottom_top";
        case StFormat_TopBottom_LR:         return "top_bottom";
        case StFormat_Rows:                 return "row_interleaved_lr";
        case StFormat_Columns:              return "col_interleaved_lr";
        case StFormat_FrameSequence:        return "block_lr";
        case StFormat_AnaglyphRedCyan:      return "anaglyph_cyan_red";
        case StFormat_AnaglyphGreenMagenta: return "anaglyph_green_magenta";
        case StFormat_AUTO:
        case StFormat_SeparateFrames:
        case StFormat_AnaglyphYellowBlue:
        case StFormat_Tiled4x:
        case StFormat_NB:
            return NULL;
    }
    return NULL;
}

bool StAVVideoMuxer::addStream(AVFormatContext* theContext,
                               AVStream*        theStream) {
#if(LIBAVFORMAT_VERSION_INT >= AV_VERSION_INT(59, 0, 100))
    // TODO
    (void )theContext; (void )theStream;
    return false;
#else
    AVCodecContext* aCodecCtxSrc = stAV::getCodecCtx(theStream);
    AVStream* aStreamOut = avformat_new_stream(theContext, aCodecCtxSrc->codec);
    if(aStreamOut == NULL) {
        signals.onError(StString("Failed allocating output stream."));
        return false;
    }
    AVCodecContext* aCodecCtxNew = stAV::getCodecCtx(aStreamOut);
    if(avcodec_copy_context(aCodecCtxNew, aCodecCtxSrc) < 0) {
        signals.onError(StString("Failed to copy context from input to output stream codec context."));
        return false;
    }
    av_dict_copy(&aStreamOut->metadata, theStream->metadata, AV_DICT_DONT_OVERWRITE);
//  myIsAttachedPic = (theStream->disposition & AV_DISPOSITION_ATTACHED_PIC) != 0;
    if(aCodecCtxSrc->codec_type == AVMEDIA_TYPE_VIDEO) {
        aStreamOut->sample_aspect_ratio   = theStream->sample_aspect_ratio;
        aCodecCtxNew->sample_aspect_ratio = aStreamOut->sample_aspect_ratio;
    }

    if(theContext->oformat->flags & AVFMT_GLOBALHEADER) {
        aCodecCtxNew->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    return true;
#endif
}

bool StAVVideoMuxer::save(const StString& theFile) {
    if(myCtxListSrc.isEmpty()
    || theFile.isEmpty()) {
        return false;
    }

    StString aFormatName = myCtxListSrc[0]->iformat->name;
    const char* aFormatStr = formatToMetadata(myStereoFormat);

    std::vector<StRemuxContext> aSrcCtxList;
    //StArrayList<StRemuxContext> aSrcCtxList;
    unsigned int aStreamCount = 0;

    StAVOutContext aCtxOut;
    if(!aCtxOut.findFormat(NULL, theFile.toCString())) {
        signals.onError(StString("Unable to find a suitable output format for '") + theFile + "'.");
        return false;
    } else if(!aCtxOut.create(theFile)) {
        signals.onError(StString("Could not create output context."));
        return false;
    }

    for(size_t aCtxId = 0; aCtxId < myCtxListSrc.size(); ++aCtxId) {
        StRemuxContext aCtxSrc;
        aCtxSrc.Context = myCtxListSrc[aCtxId];
        if(aCtxId == 0) {
            av_dict_copy(&aCtxOut.Context->metadata, aCtxSrc.Context->metadata, AV_DICT_DONT_OVERWRITE);
            av_dict_set(&aCtxOut.Context->metadata, "STEREO_MODE", aFormatStr, 0);
        }
        for(unsigned int aStreamId = 0; aStreamId < aCtxSrc.Context->nb_streams; ++aStreamId) {
            aCtxSrc.Streams.add((unsigned int )-1);
            AVStream* aStreamSrc = aCtxSrc.Context->streams[aStreamId];
            if(stAV::getCodecType(aStreamSrc) == AVMEDIA_TYPE_VIDEO) {
                if(addStream(aCtxOut.Context, aStreamSrc)) {
                    aCtxSrc.Streams[aStreamId] = aStreamCount++;
                }
            }
        }
        aSrcCtxList.push_back(aCtxSrc);
    }

    // add audio streams after video
    for(size_t aCtxId = 0; aCtxId < myCtxListSrc.size(); ++aCtxId) {
        StRemuxContext& aCtxSrc = aSrcCtxList[aCtxId];
        for(unsigned int aStreamId = 0; aStreamId < aCtxSrc.Context->nb_streams; ++aStreamId) {
            AVStream* aStreamSrc = aCtxSrc.Context->streams[aStreamId];
            if(stAV::getCodecType(aStreamSrc) == AVMEDIA_TYPE_AUDIO
            && addStream(aCtxOut.Context, aStreamSrc)) {
                aCtxSrc.Streams[aStreamId] = aStreamCount++;
            }
        }
    }

    // add other streams (subtitles) at the end
    for(size_t aCtxId = 0; aCtxId < myCtxListSrc.size(); ++aCtxId) {
        StRemuxContext& aCtxSrc = aSrcCtxList[aCtxId];
        for(unsigned int aStreamId = 0; aStreamId < aCtxSrc.Context->nb_streams; ++aStreamId) {
            AVStream* aStreamSrc = aCtxSrc.Context->streams[aStreamId];
            if(stAV::getCodecType(aStreamSrc) != AVMEDIA_TYPE_VIDEO
            && stAV::getCodecType(aStreamSrc) != AVMEDIA_TYPE_AUDIO
            && addStream(aCtxOut.Context, aStreamSrc)) {
                aCtxSrc.Streams[aStreamId] = aStreamCount++;
            }
        }
    }

    av_dump_format(aCtxOut.Context, 0, theFile.toCString(), 1);
    if(!(aCtxOut.Context->oformat->flags & AVFMT_NOFILE)) {
        const int aState = avio_open2(&aCtxOut.Context->pb, theFile.toCString(), AVIO_FLAG_WRITE, NULL, NULL);
        if(aState < 0) {
            signals.onError(StString("Could not open output file '") + theFile + "' (" + stAV::getAVErrorDescription(aState) + ")");
            return false;
        }
    }

    int aState = avformat_write_header(aCtxOut.Context, NULL);
    if(aState < 0) {
        signals.onError(StString("Error occurred when opening output file (") + stAV::getAVErrorDescription(aState) + ").");
        return false;
    }

    StAVPacket aPacket;
    for(;;) {
        size_t aNbEmpty = 0;
        for(size_t aCtxId = 0; aCtxId < aSrcCtxList.size(); ++aCtxId) {
            StRemuxContext& aCtxSrc = aSrcCtxList[aCtxId];
            if(!aCtxSrc.State) {
                ++aNbEmpty;
                continue;
            }

            if(av_read_frame(aCtxSrc.Context, aPacket.getAVpkt()) < 0) {
                aCtxSrc.State = false;
                ++aNbEmpty;
                continue;
            }

            unsigned int aStreamOutIndex = aCtxSrc.Streams[aPacket.getStreamId()];
            if(aStreamOutIndex == (unsigned int )-1) {
                continue;
            }

            AVStream* aStreamIn  = aCtxSrc.Context->streams[aPacket.getStreamId()];
            AVStream* aStreamOut = aCtxOut.Context->streams[aStreamOutIndex];

            const AVRounding aRoundParams = AVRounding(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX);
            aPacket.getAVpkt()->pts      = av_rescale_q_rnd(aPacket.getPts(), aStreamIn->time_base, aStreamOut->time_base, aRoundParams);
            aPacket.getAVpkt()->dts      = av_rescale_q_rnd(aPacket.getDts(), aStreamIn->time_base, aStreamOut->time_base, aRoundParams);
            aPacket.getAVpkt()->duration = static_cast<int >(av_rescale_q(aPacket.getDuration(), aStreamIn->time_base, aStreamOut->time_base));
            aPacket.getAVpkt()->pos      = -1;

            aState = av_interleaved_write_frame(aCtxOut.Context, aPacket.getAVpkt());
            if(aState < 0) {
                signals.onError(StString("Error muxing packet (") + stAV::getAVErrorDescription(aState) + ").");
                return false;
            }
            aPacket.free();
        }
        if(aNbEmpty == aSrcCtxList.size()) {
            break;
        }
    }
    av_write_trailer(aCtxOut.Context);
    return true;
}
