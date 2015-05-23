#include "msockbuff.h"
#include "msockapi.h"

bool MSockBuffer::Initialize(MSockAPI *api)
{
	if (this->state != MSOCKBUFF_READALL
		&& this->state != MSOCKBUFF_READLENGTH
		&& this->state != MSOCKBUFF_READID)
	{
		if (api->read(&this->id))
		{
			state = MSOCKBUFF_READID;
		}
		else return false;
	}
	if (this->state != MSOCKBUFF_READLENGTH
		&& this->state != MSOCKBUFF_READALL)
	{
		if (api->read(&this->size))
		{
			state = MSOCKBUFF_READLENGTH;
		}
		else return false;
	}
	if (this->state != MSOCKBUFF_READALL)
	{
		if (!buf)
			buf = new char[this->size];
		if (api->read(buf, this->size))
		{
			this->state = MSOCKBUFF_READALL;
			return true;
		}
		else return false;
	}
	return true;
}