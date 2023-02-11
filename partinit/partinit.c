#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include "atari_rootsect.h"

int main(int argc, char *argv[])
{
    int opt;
    _Bool print_table = false;
    _Bool install_bootstrap = false;
    _Bool make_bootable = true;
    _Bool interactive_modify = false;
    
    char *image_file;
    FILE *fp;
    char buff[512];
    
    while ((opt = getopt(argc, argv, "pibnm")) != -1)
    {
        switch (opt) 
        {
            case 'p':
                print_table = true;
                break;
            
            case 'i':
                install_bootstrap = true;
                break;
            
            case 'b':
                make_bootable = true;
                break;
                
            case 'n':
                make_bootable = false;
                break;
              
            case 'm':
                interactive_modify = true;
                
            default:
                fprintf(stderr, "unknown option %c\n", optopt);
                fprintf(stderr, "usage: %s [-p] image\n", argv[0]);
                exit(-1);
        }
    }
    
    image_file = argv[optind];
    
    fp = fopen(image_file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "failed to open image file %s\n", image_file);
        exit(1);
    }
    
    if (fread(buff, sizeof(buff), 1, fp) != 1)
    {
        fprintf(stderr, "failed to read root sector\n");
        exit(1);
    }
    
    fclose(fp);
    
    if (print_table)
    {
        struct atari_rootsect *rs = (struct atari_rootsect *) buff;
        
        printf("#\tflags\tid\tSTART\tSIZE\t\n");
        for (int i = 0; i < 4; i++)
        {
            printf("%03d\t%02x\t%c%c%c\t0x%04x\t0x%04x\n",
                   i, (uint8_t) rs->part[i].flags.lit, 
                   rs->part[i].id[0], rs->part[i].id[1], rs->part[i].id[2],
                    ntohl(rs->part[i].start), 
                    ntohl(rs->part[i].size));   
        }
    }
    return 0;
}
