#ifndef ATARI_ROOTSECT_H
#define ATARI_ROOTSECT_H

#include <stdint.h>

struct __attribute__((packed)) partflags {
    uint8_t bootable        : 1;    // disk is bootable
    uint8_t resvd           : 6;    // unused
    uint8_t exists          : 1;    // partition exists
};

union u_partflags {
    struct partflags pf;
    uint8_t lit;
};

struct __attribute__((packed)) partinfo {
    union u_partflags flags;
    uint8_t id[3];
    uint32_t start;
    uint32_t size;
};

struct __attribute__((packed)) atari_rootsect {
    uint8_t filler[0x1c2];      // empty space
    uint32_t hd_siz;            // size of disk in 512-bytes sectors
    struct partinfo part[4];
    uint32_t bsl_cnt;
    uint16_t checksum;
};


#endif // ATARI_ROOTSECT_H
