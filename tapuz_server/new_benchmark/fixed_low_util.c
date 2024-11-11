#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct record{
    uint64_t blocks_grp[8]; 
} record;

typedef struct metadata{
    record section[128]; 
} metadata;

typedef struct block {
    char data[4096]; // 4KB = page size
} block;

typedef struct section {
    block block[512]; // 512 x 4KB = 2MB 
} section;

typedef struct dictionary {
    section section[128]; // 64 x 2MB = 128MB
} dictionary;


int main(int argc, char **argv){
    struct dictionary* dict[10];
    for(int i=0;i<atoi(argv[1]);i++){
        dict[i] = malloc(sizeof(struct dictionary));
    }
    //struct dictionary* dict = malloc(sizeof(struct dictionary)*10);

    int num_dict = atoi(argv[1]);

    int* dummy_guard = malloc(2*1024*1024); // to force dict and meta be in different 2MB regions

    struct metadata* meta = malloc(sizeof(struct metadata));

    int shuffled_section_idx[128] = {
        45, 90, 86, 107, 31, 105, 40, 35,
        21, 110, 114, 98, 57, 49, 4, 93,
        5, 106, 80, 126, 73, 17, 15, 74,
        16, 48, 99, 0, 104, 108, 109, 2,
        89, 10, 39, 121, 94, 119, 60, 91,
        55, 112, 123, 53, 65, 8, 41, 92,
        111, 25, 33, 102, 125, 113, 14,
        34, 59, 70, 29, 37, 64, 71, 30,
        84, 38, 46, 78, 36, 44, 120, 127,
        88, 101, 52, 56, 23, 97, 95, 81,
        79, 27, 11, 26, 103, 72, 13, 32,
        3, 28, 12, 87, 63, 19, 1, 67, 85,
        42, 77, 96, 76, 100, 18, 43, 75,
        62, 9, 117, 115, 116, 66, 118,
        20, 47, 69, 50, 122, 61, 24,
        83, 51, 124, 58, 6, 82, 68, 7, 22, 54
    };


    // initialize all blocks

    for (int blk_idx=0; blk_idx<128; blk_idx++) {
        for (int sec_idx=0; sec_idx<128; sec_idx++) {
            for(int i=0; i <num_dict;i++) {
                dict[i]->section[sec_idx].block[blk_idx].data[0] = sec_idx;
            }
        }
    }

    // access all blocks
    for (int repeat=0; repeat<10000; repeat++) {
        for (int blk_idx=0; blk_idx<128; blk_idx++) {
            int blk_grp_idx = blk_idx >> 6; // blk_idx / 64 = [0..8)
            for (int sec_i=0; sec_i<128; sec_i++) {
                int sec_idx = shuffled_section_idx[sec_i];
                int d_idx = (sec_i >> 6) << 6; // access different cache lines
                for(int i=0; i < num_dict;i++) { 
                    int data = dict[i]->section[sec_idx].block[blk_idx].data[d_idx];
                    meta->section[sec_idx].blocks_grp[blk_grp_idx] += data;
                }
            }
        }
    }

    printf("done\n");
    return 0;
}



