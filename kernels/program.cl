

__kernel
void add_vectors(__global float *a, __global float *b, __global float *c,  const uint size){
    const int id = get_global_id(0);
    if(id>=size) return;
    c[id] = a[id] + b[id];
}