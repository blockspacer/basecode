#ifndef B2N_H
#define B2N_H

#define B2N_16(x)                               \
  x = ((((x) & 0xff00) >> 8) |                  \
       (((x) & 0x00ff) << 8))
#define B2N_32(x)                               \
  x = ((((x) & 0xff000000) >> 24) |             \
       (((x) & 0x00ff0000) >>  8) |             \
       (((x) & 0x0000ff00) <<  8) |             \
       (((x) & 0x000000ff) << 24))
#define B2N_64(x)                               \
  x = ((((x) & 0xff00000000000000LL) >> 56) |     \
       (((x) & 0x00ff000000000000LL) >> 40) |     \
       (((x) & 0x0000ff0000000000LL) >> 24) |     \
       (((x) & 0x000000ff00000000LL) >>  8) |     \
       (((x) & 0x00000000ff000000LL) <<  8) |     \
       (((x) & 0x0000000000ff0000LL) << 24) |     \
       (((x) & 0x000000000000ff00LL) << 40) |     \
       (((x) & 0x00000000000000ffLL) << 56))

#endif
