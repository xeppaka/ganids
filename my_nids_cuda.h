#ifndef MY_NIDS_CUDA_H
#define MY_NIDS_CUDA_H

//extern texture<int, 1, cudaReadModeElementType> dfa_texture;

extern "C"
void analyze_payload_cuda(int *dfa,
                     int pitch,
                     unsigned char *packet_buffer,
                     int *task_buffer,
                     int *out_buffer,
                     int tasks);

extern "C"
void bind_texture(int *device_memory, int size) throw(int);

extern "C"
void unbind_texture();

//extern "C"
//void analyze_payload_cuda(int pitch,
//                     unsigned char *packet_buffer,
//                     int *task_buffer,
//                     int *out_buffer, int tasks);

#endif // MY_NIDS_CUDA_H
