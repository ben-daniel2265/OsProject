#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct record{
    uint64_t blocks_grp[8]; 
} record;

typedef struct metadata{
    record section[64]; 
} metadata;

typedef struct block {
    char data[4096]; // 4KB = page size
} block;

typedef struct section {
    block block[512]; // 512 x 4KB = 2MB 
} section;

typedef struct dictionary {
    section section[64]; // 64 x 2MB = 128MB
} dictionary;


void main(){
    struct dictionary* dict[10];
    for(int i=0;i<10;i++){
        dict[i] = malloc(sizeof(struct dictionary));
    }
    //struct dictionary* dict = malloc(sizeof(struct dictionary)*10);

    int* dummy_guard = malloc(2*1024*1024); // to force dict and meta be in different 2MB regions

    struct metadata* meta = malloc(sizeof(struct metadata));

    int shuffled_section_idx[64] = {
        20,57,58,59,31,35,36,3,9,
        10,12,13,11,21,27,28,29,
        0,2,62,63,43,44,45,22,26,
        30,32,37,38,39,46,47,48,49,
        50,52,53,54,55,56,14,51,61,
        40,42,34,16,17,33,4,5,15,1,
        60,6,7,8,23,24,25,41,18,19
    };

    // initialize all blocks
    printf("here");

    for (int blk_idx=0; blk_idx<512; blk_idx++) {
        for (int sec_idx=0; sec_idx<64; sec_idx++) {
            for(int i=0; i < 10;i++) {
                dict[i]->section[sec_idx].block[blk_idx].data[0] = sec_idx;
            }
        }
    }

    printf("or here");

    // access all blocks
    for (int repeat=0; repeat<10000; repeat++) {
        for (int blk_idx=0; blk_idx<512; blk_idx++) {
            int blk_grp_idx = blk_idx >> 6; // blk_idx / 64 = [0..8)
            for (int sec_i=0; sec_i<64; sec_i++) {
                int sec_idx = shuffled_section_idx[sec_i];
                int d_idx = (sec_i >> 6) << 6; // access different cache lines
                for(int i=0; i < 10;i++) { 
                    int data = dict[i]->section[sec_idx].block[blk_idx].data[d_idx];
                    meta->section[sec_idx].blocks_grp[blk_grp_idx] += data;
                }
            }
        }
    }
}



