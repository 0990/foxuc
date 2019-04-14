#ifndef _STRING_CONVERT_H_
#define _STRING_CONVERT_H_

namespace Util
{
	class StringConvert
	{
	public:
		StringConvert();
		~StringConvert();

		static void CreateRandPassword(wchar_t* szPassword);

		static void	Utf8ToUnicode(wchar_t* wstr,int strMaxLen,const char* strUtf8);

		static std::wstring  Utf8ToUnicode(const std::string& strUtf8, int strMaxLen = -1);
		static std::string	Utf8ToAscii(const std::string& strUtf8);
		static std::string	Utf8ToGBK(const std::string& strUtf8);

		///////////////////////////////////////////////////////////////////////

		static std::string	UnicodeToUtf8(const std::wstring& strUnicode);
		static std::string	UnicodeToAscii(const std::wstring& strUnicode);

		//////////////////////////////////////////////////////////////////////////

		static std::string	AsciiToUtf8(const std::string& strAscii);
		static std::wstring	AsciiToUnicode(const std::string& strAscii);

		//////////////////////////////////////////////////////////////////////////

		static std::string	GBKToUtf8(const std::string& strGBK);


		//扩展转换：utf8 -> md5 unicode
		static std::wstring Utf8ToMD5Unicode(const std::string& srcUtf8);

		//扩展转换: uf8 -> md5
		static std::string Uft8ToMD5(const std::string& destUf8);

		static int UniCharToUTF8(TCHAR UniChar, char *OutUTFString);
		static int UTF8StrToUnicode(const char* UTF8String, int UTF8StringLength, TCHAR* OutUnicodeString, int UnicodeStringBufferSize);
	};
}


#endif //_STRING_CONVERT_H_