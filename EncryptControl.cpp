#include "EncryptControl.h"

EncryptControl::EncryptControl()
{
}
static EncryptControl* p = nullptr;
EncryptControl::~EncryptControl()
{
	p = nullptr;
}

EncryptControl* EncryptControl::getInstance()
{
	
	if (p==nullptr)
	{
		p = new EncryptControl();
	}
	return p;
}

void EncryptControl::destroyInstance()
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

void EncryptControl::addEncryptInfo(std::string key, std::string sign, ENCRYPTFUNC func)
{
	EncryptInfo info;
	info.key = key;
	info.sign = sign;
	info.encryptFunc = func;
	arr_encryptInfo.push_back(info);
}


unsigned char * EncryptControl::deCodeData(unsigned char *edata, unsigned long data_len, unsigned long *ret_len)
{
	unsigned char *data = nullptr;
	unsigned char offset = 0;
	if (edata)
	{
		for (auto &it : arr_encryptInfo)
		{
			if (!it.sign.empty())
			{
				std::string sign = std::string((char*)edata, it.sign.size());
				if (sign != it.sign)continue;
				offset = it.sign.size();
			}
			data = it.encryptFunc(edata+offset, data_len-offset, it.key, ret_len);
			if (data)
			{
				break;
			}
		}
	}
	
	return data;
}
