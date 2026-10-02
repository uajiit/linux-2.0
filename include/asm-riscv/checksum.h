#ifndef _ASM_RISCV_CHECKSUM_H
#define _ASM_RISCV_CHECKSUM_H

unsigned int csum_partial(const unsigned char *buff, int len, unsigned int sum);
unsigned int csum_partial_copy(const char *src, char *dst, int len, int sum);
unsigned int csum_partial_copy_fromuser(const char *src, char *dst, int len, int sum);

static inline unsigned int csum_fold(unsigned int sum)
{
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return ~sum & 0xffff;
}

static inline unsigned short ip_fast_csum(unsigned char *iph, unsigned int ihl)
{
	unsigned int sum = 0;
	unsigned int i;

	for (i = 0; i < ihl; i++) {
		sum += ((unsigned int *)iph)[i];
		if (sum & 0x80000000)
			sum = (sum & 0xffff) + (sum >> 16);
	}
	return csum_fold(sum);
}

static inline unsigned short csum_tcpudp_magic(unsigned long saddr,
		unsigned long daddr, unsigned short len, unsigned short proto,
		unsigned int sum)
{
	sum += (saddr & 0xffff) + (saddr >> 16);
	sum += (daddr & 0xffff) + (daddr >> 16);
	sum += (proto << 8) + len;
	return csum_fold(sum);
}

static inline unsigned short ip_compute_csum(unsigned char *buff, int len)
{
	return csum_fold(csum_partial(buff, len, 0));
}

#endif /* _ASM_RISCV_CHECKSUM_H */
