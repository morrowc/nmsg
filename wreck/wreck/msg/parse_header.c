#include "private.h"

wreck_msg_status
wreck_parse_header(const uint8_t *p, size_t len, uint16_t *id, uint16_t *flags,
		   uint16_t *qdcount, uint16_t *ancount, uint16_t *nscount, uint16_t *arcount)
{
	if (len < WRECK_DNS_LEN_HEADER)
		WRECK_ERROR(wreck_msg_err_len);

	*id = htons(*((uint16_t *) p));
	*flags = htons(*((uint16_t *) (p + 2)));
	*qdcount = htons(*((uint16_t *) (p + 4)));
	*ancount = htons(*((uint16_t *) (p + 6)));
	*nscount = htons(*((uint16_t *) (p + 8)));
	*arcount = htons(*((uint16_t *) (p + 10)));

	return (wreck_msg_success);
}
