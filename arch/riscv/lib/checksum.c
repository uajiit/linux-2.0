#include <linux/types.h>
#include <linux/string.h>
#include <asm/checksum.h>

unsigned int csum_partial(const unsigned char *buff, int len, unsigned int sum)
{
	while (len > 1) {
		sum += buff[0] | (buff[1] << 8);
		buff += 2;
		len -= 2;
	}
	if (len)
		sum += *buff;
	return sum;
}

unsigned int csum_partial_copy(const char *src, char *dst, int len, int sum)
{
	memcpy(dst, src, len);
	return csum_partial((const unsigned char *)dst, len, sum);
}

unsigned int csum_partial_copy_fromuser(const char *src, char *dst, int len, int sum)
{
	return csum_partial_copy(src, dst, len, sum);
}
