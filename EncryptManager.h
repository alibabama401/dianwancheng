#ifndef _ENCRYPTMANAGER_H_
#define _ENCRYPTMANAGER_H_
#include "cocos2d.h"
#include "external/json/rapidjson.h"
#include "external/json/document.h"
#include "EncryptControl.h"
USING_NS_CC;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "platform/win32/CCFileUtils-win32.h"
#define ENCRYPTMGRBASE FileUtilsWin32
#define ENCRYPT_DLL __declspec(dllexport)
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/CCFileUtils-android.h"
#define ENCRYPTMGRBASE FileUtilsAndroid
#define ENCRYPT_DLL
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "platform/apple/CCFileUtils-apple.h"
#define ENCRYPTMGRBASE FileUtilsApple
#define ENCRYPT_DLL
#endif
class ENCRYPT_DLL EncryptManager :public ENCRYPTMGRBASE,public EncryptControl
{
public:
	EncryptManager();
	~EncryptManager();
	static EncryptManager* getInstance();
	static void destroyInstance();
	void loadLookupFile(const std::string &filename);
protected:
	virtual bool init() override;
	//virtual std::string getStringFromFile(const std::string& filename);
	//virtual Data getDataFromFile(const std::string& filename);
	//CC_DEPRECATED_ATTRIBUTE virtual unsigned char* getFileData(const std::string& filename, const char* mode, ssize_t * size) override;
	virtual FileUtils::Status getContents(const std::string& filename, ResizableBuffer* buffer) override;
	virtual std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename) const;
private:
	rapidjson::Document _lookupDoc;
};
#endif

