#ifndef MY_NIDS_CUDA_H
#define MY_NIDS_CUDA_H

extern "C"
void analyze_payload_cuda(int *dfa,
                     int pitch,
                     unsigned char *packet_buffer,
                     int *task_buffer,
                     int *out_buffer,
                     int tasks);


#endif // MY_NIDS_CUDA_H
