#ifndef PAWINIT_H
#define PAWINIT_H

#define MEMORY_SIZE 500000
typedef struct { float PAW[MEMORY_SIZE]; } PAWC_DEF;
#define PAWC COMMON_BLOCK(PAWC,pawc)
COMMON_BLOCK_DEF(PAWC_DEF,PAWC);

#endif
