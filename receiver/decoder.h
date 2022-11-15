#ifndef DECODER_H
#define DECODER_H

#include <string.h>
#include <QImage>

extern "C"
{
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libavutil/opt.h"
#include "libavutil/channel_layout.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/fifo.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/dict.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/avstring.h"
#include "libavutil/imgutils.h"
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersrc.h"
#include "libavfilter/buffersink.h"
#include "libswscale/swscale.h"
}

class Decoder
{
public:
    Decoder();
    ~Decoder();

    void initDecoder();

    void decodeOneFrame(uint8_t *p_data_in, int n_size, QImage& image);

private:
    AVCodec *m_pCodec = nullptr; // AVCodec是包含了编解码器信息的结构体；
    AVCodecContext *m_pCodeCtx = nullptr; //  AVCodecContext里包含的信息很多，像编解码器的类型，附加信息，视频宽高，参考帧等等；
    AVCodecParserContext *m_pCodecParserCtx = nullptr;
    AVFormatContext *m_pFormat = nullptr; //
    AVFrame *m_pFrame = nullptr;  // AVFrame存储着码流数据，解码后的原始数据都保存在这里；
    AVPacket m_pPacket; //    AVPacket存储压缩编码数据相关信息；

    AVCodecID codec_id = AV_CODEC_ID_H264; //   AVCodecID指定码流对应标准；

    AVFrame *m_pFrameRGB = nullptr;
    uint8_t * rgbBuffer = nullptr;
    SwsContext *img_convert_ctx = nullptr;
};


#endif // DECODER_H
