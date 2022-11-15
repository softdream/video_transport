#include "decoder.h"
#include <QDebug>

Decoder::Decoder()
{

}

Decoder::~Decoder()
{

}

void Decoder::initDecoder()
{
    avcodec_register_all();

    m_pCodec = avcodec_find_decoder(codec_id);
    if( !m_pCodec ){
        qDebug()<<"avcodec_find_decoder failed !";
        return;
    }

    m_pCodeCtx = avcodec_alloc_context3( m_pCodec );
    if( !m_pCodeCtx ){
        qDebug()<<"avcode_alloc_context3 failed !";
        return;
    }

    m_pCodecParserCtx = av_parser_init(codec_id);
    if( !m_pCodecParserCtx ){
        qDebug()<<"avcode_alloc_context3 failed !";
        return;
    }

    int ret = avcodec_open2(m_pCodeCtx, m_pCodec, NULL);
    if( ret < 0 ){
        qDebug() <<"acodec_open2 failed !"<<ret;
        return ;
    }

    m_pFrame = av_frame_alloc();
    if( !m_pFrame ){
        qDebug() <<"av_frame_alloc failed !";
        return ;
    }

    av_init_packet(&m_pPacket);

    img_convert_ctx = sws_getContext(640, 640, AV_PIX_FMT_YUV420P, 640, 640, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    if( !img_convert_ctx ){
        qDebug()<<"sws_getContext failed !";
        return ;
    }

    m_pFrameRGB = av_frame_alloc();
    if( !m_pFrameRGB ){
        qDebug() <<"av_frame_alloc failed !";
        return;
    }

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, 640,640);
    rgbBuffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) m_pFrameRGB, rgbBuffer, AV_PIX_FMT_RGB32,640, 640);

    qDebug()<<"decoder inited !";
}

void Decoder::decodeOneFrame(uint8_t *p_data_in, int n_size, QImage& image)
{
    m_pPacket.size = n_size;
    m_pPacket.data = p_data_in;

    int got_picture = 0;
    int ret = avcodec_decode_video2(m_pCodeCtx, m_pFrame, &got_picture, &m_pPacket);
    if (ret < 0) {
        qDebug() << "Decode Error !";
        return;
    }
    qDebug()<<"Decode !";

    if (got_picture) {
        sws_scale(img_convert_ctx,
                    (uint8_t const * const *) m_pFrame->data,
                    m_pFrame->linesize, 0, 640, m_pFrameRGB->data,
                    m_pFrameRGB->linesize);

        image = QImage((uchar *)rgbBuffer,640,640,QImage::Format_RGB32);
    }
    qDebug()<<"decode one frame !";
}
