#ifndef STREAM_MERGE_H
#define STREAM_MERGE_H

#include "ap_int.h"
#include "hls_video.h"
#include "hls_stream.h"

#define MAX_WIDTH    4096
#define MAX_HEIGHT   4096
#define INPUT_IMAGE  "test_1080p.bmp"
#define OUTPUT_IMAGE "result_1080p.bmp"

typedef hls::stream<ap_axiu<24, 1, 1, 1> > AXI_STREAM;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> IMAGE;
typedef hls::Scalar<3, unsigned char> SCALAR;
typedef ap_uint<24> PIXEL;

void stream_merge(AXI_STREAM& st_input0,
                  AXI_STREAM& st_input1,
                  AXI_STREAM& st_output);

template<typename IMG_T, typename SCALAR_T, typename PIXEL_T>
  void stream2channel(
                      IMG_T& img_in0,
                      IMG_T& img_in1,
                      hls::stream<PIXEL_T> &ch_out0,
                      hls::stream<PIXEL_T> &ch_out1,
                      int rows, int cols);

template<typename IMG_T, typename SCALAR_T, typename PIXEL_T>
  void channel_merge(
                     hls::stream<PIXEL_T> &ch_in0,
                     hls::stream<PIXEL_T> &ch_in1,
                     IMG_T& img_out,
                     int rows, int cols);

#endif
