#include "stream_merge.h"

void stream_merge(AXI_STREAM& st_input0, AXI_STREAM& st_input1, AXI_STREAM& st_output)
{
#pragma HLS DATAFLOW
#pragma HLS INTERFACE axis port=st_input0 bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=st_input1 bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=st_output bundle=OUTPUT_STREAM
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

  const int cols = 1920;
  const int rows = 1080;

  IMAGE img_0(rows, cols);
  IMAGE img_1(rows, cols);
  IMAGE img_merge(rows*2, cols);

  hls::AXIvideo2Mat(st_input0, img_0);
  hls::AXIvideo2Mat(st_input1, img_1);

  hls::stream<PIXEL> ch0, ch1;
#pragma HLS STREAM variable=ch0 depth=2080 dim=1
#pragma HLS STREAM variable=ch1 depth=2080 dim=1

  stream2channel<IMAGE, SCALAR, PIXEL>(img_0, img_1, ch0, ch1, rows, cols);
  channel_merge<IMAGE, SCALAR, PIXEL>(ch0, ch1, img_merge, rows, cols);

  hls::Mat2AXIvideo(img_merge, st_output);
}

template<typename IMG_T, typename SCALAR_T, typename PIXEL_T>
void stream2channel(
                    IMG_T& img_in0,
                    IMG_T& img_in1,
                    hls::stream<PIXEL_T> &ch_out0,
                    hls::stream<PIXEL_T> &ch_out1,
                    int rows, int cols)
{
  SCALAR_T sca_i0, sca_i1;
  PIXEL_T  pix_i0, pix_i1;

 vert_loop0: for(int row = 0; row < rows; row++)
  horz_loop0: for(int col = 0; col < cols; col++) {
#pragma HLS PIPELINE II=1
      img_in0 >> sca_i0; // 只能适配 hls::Scalar 类型
      pix_i0( 7, 0)  = sca_i0.val[0];
      pix_i0(15, 8)  = sca_i0.val[1];
      pix_i0(23, 16) = sca_i0.val[2];
      ch_out0.write(pix_i0);
    }

 vert_loop1: for(int row = 0; row < rows; row++)
  horz_loop1: for(int col = 0; col < cols; col++) {
#pragma HLS PIPELINE II=1
      img_in1 >> sca_i1;
      pix_i1( 7, 0)  = sca_i1.val[0];
      pix_i1(15, 8)  = sca_i1.val[1];
      pix_i1(23, 16) = sca_i1.val[2];
      ch_out1.write(pix_i1);
    }
}

template<typename IMG_T, typename SCALAR_T, typename PIXEL_T>
void channel_merge(
                   hls::stream<PIXEL_T> &ch_in0,
                   hls::stream<PIXEL_T> &ch_in1,
                   IMG_T& img_out,
                   int rows, int cols)
{
  SCALAR_T sca_val;
  PIXEL_T  pix_i0, pix_i1;

 vert_loop: for(int row = 0; row < 2*rows; row++)
  horz_loop: for(int col = 0; col < cols; col++) {
#pragma HLS PIPELINE II=1
      if(row%2 == 0) {
        ch_in0.read(pix_i0);
        sca_val.val[0] = pix_i0( 7, 0);
        sca_val.val[1] = pix_i0(15, 8);
        sca_val.val[2] = pix_i0(23, 16);
      }
      else {
        ch_in1.read(pix_i1);
        sca_val.val[0] = pix_i1( 7, 0);
        sca_val.val[1] = pix_i1(15, 8);
        sca_val.val[2] = pix_i1(23, 16);
      }

      img_out << sca_val;
    }
}
