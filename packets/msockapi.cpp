#include "msockapi.h"
#include "msockbuff.h"

void MSockAPI::send(MSockBuffer *buf)
{
	write(buf->id);
	write(buf->size);
	if (buf->buf)
		write(buf->buf, buf->size);
}