#ifndef STREAM_MERGE_H
#define STREAM_MERGE_H

#include "hls_video.h"
#include "ap_int.h"

#define MAX_WIDTH    4096
#define MAX_HEIGHT   4096
#define INPUT_IMAGE  "test_1080p.bmp"
#define OUTPUT_IMAGE "result_1080p.bmp"

typedef hls::stream<ap_axiu<24, 1, 1, 1> > AXI_STREAM;

typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> IMAGE;
typedef hls::Scalar<3, unsigned char> PIXEL;
typedef hls::Scalar<3, unsigned char> SIMPLE_BUF[MAX_WIDTH];

void stream_merge(AXI_STREAM& st_input0,
                  AXI_STREAM& st_input1,
                  AXI_STREAM& st_output,
                  int rows, int cols);

#endif
