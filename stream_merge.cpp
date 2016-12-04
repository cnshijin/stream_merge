#include "stream_merge.h"

void stream_merge(AXI_STREAM& st_input0, AXI_STREAM& st_input1, AXI_STREAM& st_output,
                  int rows, int cols)
{
#pragma HLS INTERFACE axis port=st_input0 bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=st_input1 bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=st_output bundle=OUTPUT_STREAM

#pragma HLS INTERFACE s_axilite port=rows bundle=CONTROL_BUS offset=0x14
#pragma HLS INTERFACE s_axilite port=cols bundle=CONTROL_BUS offset=0x1C
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS
#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

  IMAGE img_0(rows, cols);
  IMAGE img_1(rows, cols);
  IMAGE img_merge(rows*2, cols);

  SIMPLE_BUF st0_bufa, st0_bufb;
#pragma HLS RESOURCE variable=st0_bufa core=RAM_S2P_BRAM
#pragma HLS RESOURCE variable=st0_bufb core=RAM_S2P_BRAM
  SIMPLE_BUF st1_bufa, st1_bufb;
#pragma HLS RESOURCE variable=st1_bufa core=RAM_S2P_BRAM
#pragma HLS RESOURCE variable=st1_bufb core=RAM_S2P_BRAM

  PIXEL pix_i0, pix_i1;
  PIXEL pix_o;

  hls::AXIvideo2Mat(st_input0, img_0);
  hls::AXIvideo2Mat(st_input1, img_1);

  vert_loop: for(int row = 0; row < rows+1; row++) {
#pragma HLS LOOP_TRIPCOUNT min=2048 max=2048 avg=2048
    horz_loop0: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=2048 max=2048 avg=2048
      img_0 >> pix_i0;
      img_1 >> pix_i1;

      if(row%2 == 0) {
        st0_bufa[col] = pix_i0;
        st1_bufa[col] = pix_i1;
      }
      else {
        st0_bufb[col] = pix_i0;
        st1_bufb[col] = pix_i1;
      }
    }

  merge_loop: for(int i = 0; i < 2; i++) {
    horz_loop1: for(int m = 0; m < cols; m++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=2048 max=2048 avg=2048
        if(row >= 1) {
          if(row%2 == 1) {
            if(i == 0) pix_o = st0_bufa[m];
            else       pix_o = st1_bufa[m];
          }
          else {
            if(i == 0) pix_o = st0_bufb[m];
            else       pix_o = st1_bufb[m];
          }

          img_merge << pix_o;
        }
      }
    }
  }

  hls::Mat2AXIvideo(img_merge, st_output);
}
