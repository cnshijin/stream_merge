#include "stream_merge.h"

void stream_merge(AXI_STREAM& st_input0, AXI_STREAM& st_input1, AXI_STREAM& st_output,
                  int rows, int cols)
{
#pragma HLS DATAFLOW
#pragma HLS INTERFACE axis port=st_input0
#pragma HLS INTERFACE axis port=st_input1
#pragma HLS INTERFACE axis port=st_output
#pragma HLS INTERFACE s_axilite port=rows bundle=CONTROL_BUS offset=0x14
#pragma HLS INTERFACE s_axilite port=cols bundle=CONTROL_BUS offset=0x1C
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

  IMAGE img_0(rows, cols);
  IMAGE img_1(rows, cols);
  IMAGE img_merge(rows*2, cols);
#pragma HLS STREAM variable=img_0     depth=2080 dim=1
#pragma HLS STREAM variable=img_1     depth=2080 dim=1
#pragma HLS STREAM variable=img_merge depth=32   dim=1

  SCALAR sca_var;

  hls::AXIvideo2Mat(st_input0, img_0);
  hls::AXIvideo2Mat(st_input1, img_1);

 vert_loop: for(int row = 0; row < rows*2; row++)
#pragma HLS LOOP_TRIPCOUNT min=32 max=4096
  horz_loop: for(int col = 0; col < cols; col++) {
#pragma HLS LOOP_TRIPCOUNT min=32 max=4096
#pragma HLS PIPELINE II=1
      if(row%2 == 0)
        img_0.read(sca_var);
      else
        img_1.read(sca_var);

      img_merge.write(sca_var);
    }

  hls::Mat2AXIvideo(img_merge, st_output);
}
