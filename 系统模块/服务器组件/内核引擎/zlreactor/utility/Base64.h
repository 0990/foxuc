// ***********************************************************************
// Filename         : Base64.h
// Author           : LIZHENG
// Created          : 2014-09-16
// Description      : Base64 ������
//
// Copyright (c) lizhenghn@gmail.com. All rights reserved.
// ***********************************************************************
#ifndef ZL_BASE64_H
#define ZL_BASE64_H
#include <string>
namespace zl{
	namespace util{
		// len == strlen(src), and strlen(dst) >= len * 4 / 3
		// ���ر��������ַ�������
		size_t         base64Encode(const char *src, size_t len, char *dst);
		size_t         base64Encode(const char *src, size_t len, std::string& dst);
		size_t         base64Encode(const std::string& src, std::string& dst);
		std::string    base64Encode(const char *src, size_t len);
		std::string    base64Encode(const std::string& src);

		// len == strlen(src), and strlen(dst) >= len * 3 / 4
		// ���ؽ��������ַ�������
		size_t         base64Decode(const char *src, size_t len, char *dst);
		size_t         base64Decode(const char *src, size_t len, std::string& dst);
		size_t         base64Decode(const std::string& src, std::string& dst);
		std::string    base64Decode(const char *src, size_t len);
		std::string    base64Decode(const std::string& src);
	}
}

#endif  /* ZL_BASE64_H */
