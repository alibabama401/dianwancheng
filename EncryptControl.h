#pragma once
#include <functional>
#include <string>
#include <vector>
class EncryptControl
{
public:
	typedef std::function<unsigned char*(unsigned char *data, unsigned long data_len, std::string key, unsigned long *ret_len)> ENCRYPTFUNC;
	struct EncryptInfo
	{
		std::string key;
		std::string sign;
		ENCRYPTFUNC encryptFunc;
	};
	typedef std::vector<EncryptInfo> ARRAY_ENCRYPTINFO;
protected:
	EncryptControl();
public:
	~EncryptControl();
	static EncryptControl* getInstance();
	static void destroyInstance();
	void addEncryptInfo(std::string key, std::string sign, ENCRYPTFUNC func);
	unsigned char* deCodeData(unsigned char *edata, unsigned long data_len, unsigned long *ret_len);
private:
	ARRAY_ENCRYPTINFO arr_encryptInfo;
};

