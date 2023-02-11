#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>

struct pdinfo {
    uint32_t driveid;                   /* identifies the device type */
    uint32_t sanity;                    /* verifies device sanity */
    uint32_t version;                   /* version number */
    uint8_t serial[12];                 /* serial number of the device */
    uint32_t cyls;                      /* number of cylinders per drive */
    uint32_t tracks;                    /* number of tracks per cylinder */
    uint32_t sectors;                   /* number of sectors per track */
    uint32_t bytes;                     /* number of bytes per sector */
    uint32_t logicalst;                 /* sector address of logical sector 0 */
    uint32_t errlogst;                  /* sector address of error log area */
    uint32_t errlogsz;                  /* size in bytes of error log area */
    uint32_t mfgst;                     /* sector address of mfg. defect info */
    uint32_t mfgsz;                     /* size in bytes of mfg. defect info */
    uint32_t defectst;                  /* sector address of the defect map */
    uint32_t defectsz;                  /* size in bytes of defect map */
    uint32_t relno;                     /* number of relocation areas */
    uint32_t relst;                     /* sector address of relocation area */
    uint32_t relsz;                     /* size in sectors of relocation area */
    uint32_t relnext;                   /* address of next avail reloc sector */
    uint32_t allcstrt;                  /* start of the allocatable disk */
    uint32_t allcend;                   /* end of allocatable disk */
} __attribute__((packed)) __attribute__((aligned(2)));

typedef uint32_t asv_daddr_t;

struct partition {
    uint16_t p_tag;                     /*ID tag of partition*/
    uint16_t p_flag;                    /*permision flags*/
    asv_daddr_t p_start;                    /*start sector no of partition*/
    int32_t p_size;                     /*# of blocks in partition*/
} __attribute__((packed)) __attribute__((aligned(2)));


#define V_NUMPAR        16

struct vtoc {
    uint32_t v_bootinfo[3];             /* info needed by mboot */
    uint32_t v_sanity;                  /* to verify vtoc sanity */
    uint32_t v_version;                 /* layout version */
    uint8_t v_volume[8];                /* volume name */
    uint16_t v_sectorsz;                /* sector size in bytes */
    uint16_t v_nparts;                  /* number of partitions */
    uint32_t v_reserved[10];            /* free space */
    struct partition v_part[V_NUMPAR];  /* partition headers */
    time_t timestamp[V_NUMPAR];         /* partition timestamp */
} __attribute__((packed)) __attribute__((aligned(2)));


static char *tags[] = {
    "UNASSIGNED",
    "BOOT",
    "ROOT",
    "SWAP",
    "USR",
    "VAR",
    "STAND",
    "FULLDISK/BACKUP",
    "HOME",
    "OVERLAP"
};

int main(int argc, char *argv[])
{
    FILE *fp;
    size_t res;
    struct vtoc vtoc;

    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <image>\n", argv[0]);
        exit(1);
    }
    
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "could not open \"%s\"\n", argv[1]);
        exit(1);
    }
    
    res = fread(&vtoc, sizeof(vtoc), 1, fp);
    if (res != 1)
    {
        fprintf(stderr, "could not read vtoc from disk\n");
        exit(1);
    }
    
    fclose(fp);

    printf("nr\ttag\tflag\tstart\tsize\ttype\n");
    for (int i = 0; i < V_NUMPAR; i++)
    {
        struct partition *p = &vtoc.v_part[i];
        int tag_index = ntohs(p->p_tag);
        
        printf("%d\t%d\t%d\t%u\t%u\t%s\n", i, ntohs(p->p_tag), ntohs(p->p_flag),
                ntohl(p->p_start), ntohl(p->p_size), tag_index >= 0 && tag_index < sizeof(tags) / sizeof(char *) 
                        ? tags[tag_index] : "UNKNOWN");
    }
}

