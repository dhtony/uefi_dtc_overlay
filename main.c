#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ufdt_overlay.h"
#include "libfdt_env.h"
#define fdt_get_header(fdt, field) \
    (fdt32_to_cpu(((const struct fdt_header *)(fdt))->field))
#define fdt_magic(fdt) 			(fdt_get_header(fdt, magic))
#define fdt_totalsize(fdt)		(fdt_get_header(fdt, totalsize))
#define fdt_off_dt_struct(fdt)		(fdt_get_header(fdt, off_dt_struct))
#define fdt_off_dt_strings(fdt)		(fdt_get_header(fdt, off_dt_strings))
#define fdt_off_mem_rsvmap(fdt)		(fdt_get_header(fdt, off_mem_rsvmap))
#define fdt_version(fdt)		(fdt_get_header(fdt, version))
#define fdt_last_comp_version(fdt) 	(fdt_get_header(fdt, last_comp_version))
#define fdt_boot_cpuid_phys(fdt) 	(fdt_get_header(fdt, boot_cpuid_phys))
#define fdt_size_dt_strings(fdt) 	(fdt_get_header(fdt, size_dt_strings))
#define fdt_size_dt_struct(fdt)		(fdt_get_header(fdt, size_dt_struct))
int main(int argc, char **argv){
//	struct fdt_header *SocDtb = NULL;
	void * BoardDtb = NULL;
	void * SocDtbHdr = NULL;
	void * FinalDtbHdr = NULL;

    FILE * BoardDtbFile;
    size_t BoardDtbsize;
    char * BoardDtbbuffer;
    size_t BoardDtbfinal;
    FILE * SocDtbFile;
    size_t SocDtbsize;
    char * SocDtbbuffer;
    size_t SocDtbfinal;

/*
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
*/
    if (argc>3){
        printf("Wrong input arguments\n");
        return 1;
    }
 printf ("%s will be taken as SOC dtb file\n", argv[1]);
 printf ("%s will be taken as overlay dtbo file\n", argv[2]);
    /*Open soc Dtb and BoardDtb*/

    BoardDtbFile= fopen ( argv[2] , "rb" );
        if (BoardDtbFile==NULL) {printf ("File error %s \n", argv[2]);
        exit (1);
    }

    // calculating file size
    fseek (BoardDtbFile, 0 , SEEK_END);
    BoardDtbsize = ftell (BoardDtbFile);
    rewind (BoardDtbFile);

    // allocating mem for file
    BoardDtbbuffer = (char*) malloc (sizeof(char)*BoardDtbsize);
    if (BoardDtbbuffer == NULL) {fputs ("Memory error\n",stderr); exit (2);}

    // copying file into buffer
    BoardDtbfinal = fread (BoardDtbbuffer,1,BoardDtbsize,BoardDtbFile);
    if (BoardDtbfinal != BoardDtbsize)
    {
        fputs ("Reading error",stderr);
        exit (3);
    }

     SocDtbFile= fopen ( argv[1] , "rb" );
        if (SocDtbFile==NULL) {printf ("File error %s \n", argv[1]);
        exit (1);
    }

    // calculating file size
    fseek (SocDtbFile, 0 , SEEK_END);
    SocDtbsize = ftell (SocDtbFile);
    rewind (SocDtbFile);

    // allocating mem for file
    SocDtbbuffer = (char*) malloc (sizeof(char)*SocDtbsize);
    if (SocDtbbuffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copying file into buffer
    SocDtbfinal = fread (SocDtbbuffer,1,SocDtbsize,SocDtbFile);
    if (SocDtbfinal != SocDtbsize)
    {
        fputs ("Reading error",stderr);
        exit (3);
    }

    //SocDtbHdr = ufdt_install_blob(SocDtb, fdt_totalsize(SocDtb));
    SocDtbHdr = SocDtbbuffer;
    BoardDtb = BoardDtbbuffer;
    FinalDtbHdr = ufdt_apply_overlay(SocDtbHdr, fdt_totalsize(SocDtbHdr), BoardDtb, fdt_totalsize(BoardDtb));
    if (!FinalDtbHdr) {
        printf(("ufdt apply overlay failed\n"));
        return 1;
    }else{
        printf("ufdt successfully apply overlay\n");
    }

    fclose (BoardDtbFile);
    free (BoardDtbbuffer);

    fclose (SocDtbFile);
    free (SocDtbbuffer);

    return 0;
}
