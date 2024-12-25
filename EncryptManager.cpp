#include "EncryptManager.h"

EncryptManager::EncryptManager()
{
}
static EncryptManager* p = nullptr;
EncryptManager::~EncryptManager()
{
	p = nullptr;
}

EncryptManager* EncryptManager::getInstance()
{
	
	if (p==nullptr)
	{
		p = new EncryptManager();
		if (!p->init())
		{
			delete p;
			p = nullptr;
			CCLOG("ERROR: Could not init EncryptManager");
		}
	}
	return p;
}

void EncryptManager::destroyInstance()
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

void EncryptManager::loadLookupFile(const std::string & filename)
{
	if (isFileExist(filename))
	{
		_lookupDoc.Parse(getStringFromFile(filename).c_str());
	}
}

bool EncryptManager::init()
{
	if (!ENCRYPTMGRBASE::init()) return false;
	if (s_sharedFileUtils)
	{
		_searchPathArray = s_sharedFileUtils->getSearchPaths();
		_searchResolutionsOrderArray = s_sharedFileUtils->getSearchResolutionsOrder();
	}
	this->setDelegate(this);
	return true;
}

FileUtils::Status EncryptManager::getContents(const std::string & filename, ResizableBuffer * buffer)
{
	auto ret = ENCRYPTMGRBASE::getContents(filename, buffer);
	if (ret==Status::OK&&buffer->size()>0)
	{
		unsigned long ret_len = 0;
		unsigned char *cstr = deCodeData((unsigned char*)buffer->buffer(), buffer->size(), &ret_len);
		if (cstr)
		{
			buffer->resize(ret_len);
			memcpy(buffer->buffer(), cstr, ret_len);
			free(cstr);
			//log("decrypt %s success",filename.c_str());
		}//else log("decrypt %s fail",filename.c_str());
	}
	return ret;
}

std::string EncryptManager::getFullPathForDirectoryAndFilename(const std::string & directory, const std::string & filename) const
{
	std::string ret = ENCRYPTMGRBASE::getFullPathForDirectoryAndFilename(directory, filename);
	if (ret.empty()&&directory.find(_defaultResRootPath)==0)
	{
		std::string path = directory.substr(_defaultResRootPath.size()) + filename;
		if (_lookupDoc.IsObject()&&_lookupDoc.HasMember(path.c_str()))
		{
			path = _defaultResRootPath + _lookupDoc[path.c_str()].GetString();
			std::string dir = path.substr(0, path.find_last_of('/') + 1);
			std::string name = path.substr(path.find_last_of('/') + 1);
			ret = ENCRYPTMGRBASE::getFullPathForDirectoryAndFilename(dir, name);
		}
	}
	return ret;
}

/*std::string EncryptManager::getStringFromFile(const std::string & filename)
{
	std::string str = ENCRYPTMGRBASE::getStringFromFile(filename);
	if (!str.empty())
	{
		unsigned long ret_len = 0;
		unsigned char *cstr = deCodeData((unsigned char*)str.c_str(), str.size(), &ret_len);
		if (cstr)
		{
			str.clear();
			str.append((const char*)cstr, ret_len);
			free(cstr);
		}
	}
	return str;
}

Data EncryptManager::getDataFromFile(const std::string & filename)
{
	Data data = ENCRYPTMGRBASE::getDataFromFile(filename);
	if (!data.isNull())
	{
		unsigned long ret_len = 0;
		unsigned char *cstr = deCodeData(data.getBytes(), data.getSize(), &ret_len);
		if (cstr)
		{
			data.clear();
			data.fastSet(cstr, ret_len);
		}
		
	}
	return data;
}

CC_DEPRECATED_ATTRIBUTE unsigned char * EncryptManager::getFileData(const std::string & filename, const char * mode, ssize_t * size)
{
	unsigned char *cstr = ENCRYPTMGRBASE::getFileData(filename, mode, size);
	if (cstr)
	{
		unsigned long ret_len = 0;
		auto dstr = deCodeData(cstr, *size, &ret_len);
		if (dstr)
		{
			*size = ret_len;
			free(cstr);
			cstr = dstr;
		}
	}
	return cstr;
}*/
