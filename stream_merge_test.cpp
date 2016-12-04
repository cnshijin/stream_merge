#include "stream_merge.h"
#include "hls_opencv.h"

using namespace cv;

int main(int argc, char** argv) {
  IplImage*  src;
  IplImage*  dst;
  CvSize     src_size, dst_size;
  AXI_STREAM src0_axi, src1_axi, dst_axi;

  src = cvLoadImage(INPUT_IMAGE);
  src_size = cvGetSize(src);
  dst_size.height = src_size.height * 2;
  dst_size.width  = src_size.width;
  dst = cvCreateImage(dst_size, src->depth, src->nChannels);
  IplImage2AXIvideo(src, src0_axi);
  IplImage2AXIvideo(src, src1_axi);
  // DUT
  stream_merge(src0_axi, src1_axi, dst_axi, src_size.height, src_size.width);
  // Convert the AXI4 Stream data to OpenCV format
  AXIvideo2IplImage(dst_axi, dst);
  cvSaveImage(OUTPUT_IMAGE, dst);
  cvReleaseImage(&src);
  cvReleaseImage(&dst);

  return 0;
}
