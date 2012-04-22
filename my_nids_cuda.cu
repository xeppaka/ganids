#include <cuda.h>
#include "my_nids_cuda.h"

__global__ void analyze_payload_cuda_(int *dfa,
                                      int pitch,
                                      unsigned char *packet_buffer,
                                      int *task_buffer,
                                      int *out_buffer,
                                      int tasks)
{
    int task_num = blockIdx.x * blockDim.x + threadIdx.x;
    int pitch_int = pitch / 4;
    int task_offset = task_num * 4;

    while (task_num < tasks) {
        out_buffer[task_num] = -1;

        unsigned char* payload = packet_buffer + task_buffer[task_offset];
        int payload_length = task_buffer[task_offset + 1];
        int *dfa_offset = dfa + pitch_int * task_buffer[task_offset + 2];
        int result_rule = task_buffer[task_offset + 3];
        int cur_state = 0;
        int cur_idx = 0;

        while (true) {
            if (cur_state == -1)
                break;

            if (cur_state == -2) {
                out_buffer[task_num] = result_rule;
                break;
            }

            if ((dfa_offset + cur_state * pitch_int)[256] > 0) {
                if (dfa_offset == dfa)
                    out_buffer[task_num] = cur_state;
                else
                    out_buffer[task_num] = result_rule;

                break;
            }

            cur_state = (dfa_offset + cur_state * pitch_int)[payload[cur_idx]];
            ++cur_idx;

            if (cur_idx < payload_length)
                break;
        }

        __syncthreads();
        task_num += gridDim.x * blockDim.x;
    }
}

void analyze_payload_cuda(int *dfa,
                          int pitch,
                          unsigned char *packet_buffer,
                          int *task_buffer,
                          int *out_buffer,
                          int tasks)
{
    analyze_payload_cuda_<<< 4, 64 >>>(dfa, pitch, packet_buffer, task_buffer, out_buffer, tasks);
}
