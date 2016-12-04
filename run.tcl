open_project stream_merge
set_top stream_merge
add_files stream_merge.cpp
add_files -tb stream_merge_test.cpp
add_files -tb test_1080p.bmp
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
set_clock_uncertainty 0.5
csim_design -clean
csynth_design
exit
