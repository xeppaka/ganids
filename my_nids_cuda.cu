#include <cuda.h>
#include "my_nids_cuda.h"

__device__ int check_payload_part(int payload, int cur_state, int result_rule, int task_num, int *dfa, int *dfa_offset, int pitch_int, int *out_buffer) {
    unsigned c1, c2, c3, c4;
    c1 = payload & 0xFF;
    c2 = (payload >> 8) & 0xFF;
    c3 = (payload >> 16) & 0xFF;
    c4 = (payload >> 24) & 0xFF;

    cur_state = (dfa_offset + cur_state * pitch_int)[c1];

    if (cur_state == -1)
        return -1;

    if (cur_state == -2) {
        out_buffer[task_num] = result_rule;
        return -1;
    }

    if ((dfa_offset + cur_state * pitch_int)[256] > 0) {
        if (dfa_offset == dfa)
            out_buffer[task_num] = cur_state;
        else
            out_buffer[task_num] = result_rule;

        return -1;
    }

    cur_state = (dfa_offset + cur_state * pitch_int)[c2];

    if (cur_state == -1)
        return -1;

    if (cur_state == -2) {
        out_buffer[task_num] = result_rule;
        return -1;
    }

    if ((dfa_offset + cur_state * pitch_int)[256] > 0) {
        if (dfa_offset == dfa)
            out_buffer[task_num] = cur_state;
        else
            out_buffer[task_num] = result_rule;

        return -1;
    }

    cur_state = (dfa_offset + cur_state * pitch_int)[c3];

    if (cur_state == -1)
        return -1;

    if (cur_state == -2) {
        out_buffer[task_num] = result_rule;
        return -1;
    }

    if ((dfa_offset + cur_state * pitch_int)[256] > 0) {
        if (dfa_offset == dfa)
            out_buffer[task_num] = cur_state;
        else
            out_buffer[task_num] = result_rule;

        return -1;
    }

    cur_state = (dfa_offset + cur_state * pitch_int)[c4];

    if (cur_state == -1)
        return -1;

    if (cur_state == -2) {
        out_buffer[task_num] = result_rule;
        return -1;
    }

    if ((dfa_offset + cur_state * pitch_int)[256] > 0) {
        if (dfa_offset == dfa)
            out_buffer[task_num] = cur_state;
        else
            out_buffer[task_num] = result_rule;

        return -1;
    }

    return cur_state;
}

__global__ void analyze_payload_cuda_(int *dfa,
                                      int pitch,
                                      unsigned char *packet_buffer,
                                      int *task_buffer,
                                      int *out_buffer,
                                      int tasks)
{
    int task_num = blockIdx.x * blockDim.x + threadIdx.x;
    int pitch_int = pitch / 4;

    while (task_num < tasks) {
        int task_offset = task_num * 4;
        out_buffer[task_num] = -1;

        unsigned char* payload = packet_buffer + task_buffer[task_offset];
        unsigned payload_length = task_buffer[task_offset + 1];
        int *dfa_offset = dfa + pitch_int * task_buffer[task_offset + 2];
        unsigned result_rule = task_buffer[task_offset + 3];
        unsigned cur_state = 0;
        unsigned cur_idx = 0;
        int ret;

        while (cur_idx < payload_length) {
            int4 payload_part = *((int4*)(payload + cur_idx));
            if ((ret = check_payload_part(payload_part.x, cur_state, result_rule, task_num, dfa, dfa_offset, pitch_int, out_buffer)) < 0)
                break;
            cur_state = ret;

            if ((ret = check_payload_part(payload_part.y, cur_state, result_rule, task_num, dfa, dfa_offset, pitch_int, out_buffer)) < 0)
                break;
            cur_state = ret;

            if ((ret = check_payload_part(payload_part.z, cur_state, result_rule, task_num, dfa, dfa_offset, pitch_int, out_buffer)) < 0)
                break;
            cur_state = ret;

            if ((ret = check_payload_part(payload_part.w, cur_state, result_rule, task_num, dfa, dfa_offset, pitch_int, out_buffer)) < 0)
                break;
            cur_state = ret;

            cur_idx += 16;
        }

        task_num += gridDim.x * blockDim.x;
    }
}

//void analyze_payload_cuda(int *dfa,
//                          int pitch,
//                          unsigned char *packet_buffer,
//                          int *task_buffer,
//                          int *out_buffer,
//                          int tasks)
//{
//    analyze_payload_cuda_<<< 8, 256 >>>(dfa, pitch, packet_buffer, task_buffer, out_buffer, tasks);
//}

//__global__ void analyze_payload_cuda_(int pitch,
//                                      unsigned char *packet_buffer,
//                                      int *task_buffer,
//                                      int *out_buffer,
//                                      int tasks)
//{
//    int task_num = blockIdx.x * blockDim.x + threadIdx.x;
//    int pitch_int = pitch / sizeof(int);
//    __shared__ unsigned int payload_cache[16];

//    while (task_num < /*tasks*/1000) {
//        int task_offset = task_num * 4;
//        out_buffer[task_num] = -1;

//        unsigned char* payload = packet_buffer + task_buffer[task_offset];
//        int payload_length = task_buffer[task_offset + 1];
//        int dfa_offset = task_buffer[task_offset + 2];
//        int result_rule = task_buffer[task_offset + 3];
//        int cur_state = 0;
//        int cur_idx = 0;
//        int cache_index = 1024;

//        while (cur_idx < /*payload_length*/100) {
//            if (cache_index >= 64) {
//                for (int i = 0; i < 16 && (cur_idx + i*4 < payload_length); ++i) {
//                    payload_cache[i] = ((unsigned int*)(payload + cur_idx))[i];
//                }
//                cache_index = 0;
//            }

//            if (cur_state == -1)
//                break;

//            if (cur_state == -2) {
//                out_buffer[task_num] = result_rule;
//                break;
//            }

//            tex1Dfetch(dfa_texture, 0);
            //int t = task_buffer[cur_idx];

//            if (tex1Dfetch(dfa_texture, dfa_offset * pitch + 256) > 0) {
//                if (dfa_offset == 0)
//                    out_buffer[task_num] = cur_state;
//                else
//                    out_buffer[task_num] = result_rule;

//                break;
//            }

            //cur_state = tex1Dfetch(dfa_texture, dfa_offset * pitch + ((unsigned char *)payload_cache)[cache_index]);

//            cur_state = dfa_state[((unsigned char *)payload_cache)[cache_index]];
//            cur_state = dfa_state[payload[cur_idx]];

//            ++cur_idx;
//            ++cache_index;
//        }

//        out_buffer[task_num] = task_num;

//        task_num += gridDim.x * blockDim.x;
//    }
//}

void analyze_payload_cuda(int *dfa,
                          int pitch,
                          unsigned char *packet_buffer,
                          int *task_buffer,
                          int *out_buffer,
                          int tasks)
{
    analyze_payload_cuda_<<< 8, 32 >>>(dfa, pitch, packet_buffer, task_buffer, out_buffer, tasks);
}

//void bind_texture(int *device_memory, int size) throw(int) {
//    if (cudaBindTexture(NULL, dfa_texture, device_memory, size) != cudaSuccess)
//        throw 1;
//    dfa_texture.normalized = false;
////    dfa_texture.filterMode = cudaFilterModePoint;
////    dfa_texture.addressMode = cudaAddressModeClamp;
//}

//void unbind_texture() {
//    cudaUnbindTexture(dfa_texture);
//}
