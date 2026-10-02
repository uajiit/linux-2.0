#ifndef _ASM_RISCV_BYTEORDER_H
#define _ASM_RISCV_BYTEORDER_H

#undef ntohl
#undef ntohs
#undef htonl
#undef htons

#define __LITTLE_ENDIAN
#define __LITTLE_ENDIAN_BITFIELD

static inline unsigned long __ntohl(unsigned long x)
{
	return ((x & 0x000000ffUL) << 24) |
	       ((x & 0x0000ff00UL) <<  8) |
	       ((x & 0x00ff0000UL) >>  8) |
	       ((x & 0xff000000UL) >> 24);
}

static inline unsigned short __ntohs(unsigned short x)
{
	return ((x & 0x00ff) << 8) | ((x & 0xff00) >> 8);
}

#define __constant_ntohl(x) \
	((unsigned long)((((unsigned long)(x) & 0x000000ffUL) << 24) | \
			 (((unsigned long)(x) & 0x0000ff00UL) <<  8) | \
			 (((unsigned long)(x) & 0x00ff0000UL) >>  8) | \
			 (((unsigned long)(x) & 0xff000000UL) >> 24)))

#define __constant_ntohs(x) \
	((unsigned short)((((unsigned short)(x) & 0x00ff) << 8) | \
			  (((unsigned short)(x) & 0xff00) >> 8)))

#define __htonl(x) __ntohl(x)
#define __htons(x) __ntohs(x)
#define __constant_htonl(x) __constant_ntohl(x)
#define __constant_htons(x) __constant_ntohs(x)

#ifdef __OPTIMIZE__
#define ntohl(x) \
	(__builtin_constant_p((long)(x)) ? __constant_ntohl((x)) : __ntohl((x)))
#define ntohs(x) \
	(__builtin_constant_p((short)(x)) ? __constant_ntohs((x)) : __ntohs((x)))
#define htonl(x) \
	(__builtin_constant_p((long)(x)) ? __constant_htonl((x)) : __htonl((x)))
#define htons(x) \
	(__builtin_constant_p((short)(x)) ? __constant_htons((x)) : __htons((x)))
#endif

#endif /* _ASM_RISCV_BYTEORDER_H */
