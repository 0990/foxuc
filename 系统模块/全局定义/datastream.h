#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
//#include "GameLog.h"


#define Stream_VALUE(Name)	\
	if(bSend)			\
{                           \
	kData.pushValue(Name);\
}\
else\
{\
	kData.popValue(Name);\
}\


#define Stream_VECTOR(Name)	\
	if(bSend)			\
{                           \
	kData.pushValueVector(Name);\
}		\
else\
{\
	kData.popValueVector(Name);\
}\


#define Stream_DATA(Name)	\
	if(bSend)			\
{                           \
	kData.pushValueData(Name);\
}		\
else\
{\
	kData.popValueData(Name);\
}\


#define Stream_VALUE_SYSTEMTIME(Name)	\
	if(bSend)			\
{                           \
	kData.pushValue(&Name,sizeof(SYSTEMTIME));\
}	\
	else\
{\
	kData.popValue(&Name,sizeof(SYSTEMTIME));\
}\

#define StructVecotrMember(TypeMem,Member)\
{\
	if (!bSend)\
{\
	int iCout = 0;\
	kData.popValue(iCout);\
	for (int i = 0;i<iCout;i++)\
{\
	TypeMem kTempValue;\
	kTempValue.StreamValue(kData,bSend);\
	this->Member.push_back(kTempValue);\
}\
}\
	else\
{\
	int iCout = this->Member.size();\
	kData.pushValue(iCout);\
	for (int i = 0;i<iCout;i++)\
{\
	this->Member[i].StreamValue(kData,bSend);\
}\
}\
}

class datastream:public std::vector<char>
{
public:
	datastream(){}
	datastream(void* pData,int dSize)
	{
		pushValue((char*)pData,dSize);
	}
	char* data()
	{
		return &((*this)[0]);
	}
	datastream& pushValue(unsigned short value)
	{
		push(value);
		return *this;
	}  
	datastream& popValue(unsigned short& value)
	{
		return pop(value);
	} 
	datastream& pushValueData(datastream& value)
	{
		push(int(value.size()));
		if (!value.size())
		{
			return *this;
		}
		memcpy(inc_size(value.size()), (void*)&value[0], value.size());
		return *this;
	}  
	datastream& popValueData(datastream& value)
	{
		int nSize = 0;
		pop(nSize);
		if (nSize == 0)
		{
			return *this;
		}
		if (nSize > (int)size())
		{
			return *this;
		}
		std::vector<char>::iterator first=begin(), last=first+nSize;
		value.assign(first, last);
		erase(first, last);
		return *this;
	} 
	datastream& pushValue(unsigned int value)
	{
		push(value);
		return *this;
	}  
	datastream& popValue(unsigned int& value)
	{
		return pop(value);
	}  

	datastream& pushValue(int value)
	{
		push(value);
		return *this;
	}  
	datastream& popValue(int& value)
	{
		return pop(value);
	} 

	datastream& pushValue(std::string data)
	{
		push(int(data.size()));
		if (!data.size())
		{
			return *this;
		}
		memcpy(inc_size(data.size()), (void*)&data[0], data.size());
		return *this;
	}    
	datastream& popValue(std::string& data)
	{
		int nSize = 0;
		pop(nSize);
		if (nSize == 0)
		{
			return *this;
		}
		if ((int)size() < nSize)
		{
			return *this;
		}
		std::vector<char>::iterator first=begin(), last=first+nSize;
		data.assign(first, last);
		erase(first, last);
		return *this;
	}
	datastream& pushValue(long long value)
	{
		push(value);
		return *this;
	}  
	datastream& popValue(long long& value)
	{
		return pop(value);
	} 

	datastream& pushValue(unsigned long long value)
	{
		push(value);
		return *this;
	}  
	datastream& popValue(unsigned long long& value)
	{
		return pop(value);
	} 
	datastream& pushValue(float value)
	{
		return push(value);
	}  
	datastream& popValue(float& value)
	{
		return pop(value);
	} 
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	datastream& pushValue(unsigned long value)
	{
		push((unsigned int)value);
		return *this;
	}  
	datastream& popValue(unsigned long& value)
	{
		unsigned int kTempValue = 0; 
		pop(kTempValue);
		value = kTempValue;
		return *this;
	} 
#endif
	datastream& pushValue(unsigned char value)
	{
		return push(value);
	}  
	datastream& popValue(unsigned char& value)
	{
		return pop(value);
	}
	datastream& pushValue(char value)
	{
		return push(value);
	}  
	datastream& popValue(char& value)
	{
		return pop(value);
	}
	datastream& pushValue(void* value,int iSize)
	{
		if (iSize == 0)
		{
			return *this;
		}
		memcpy(inc_size(iSize), value, iSize);
		return *this;
	}  
	datastream& popValue(void* value,unsigned int iSize)
	{
		if (size() < iSize)
		{
			return *this;
		}
		memcpy(value, &operator[](0), iSize);
		erase(begin(), begin()+iSize);
		return *this;
	}
	datastream& pushValue(bool value)
	{
		return push(value);
	}  
	datastream& popValue(bool& value)
	{
		return pop(value);
	}

	//std::vector´¦Àí
	template<typename C>
	datastream& pushValueVector(const std::vector<C>& data)
	{
		unsigned int nSize=data.size();
		pushValue(nSize);
		for (size_t i=0; i<nSize; ++i)
		{
			pushValue(data[i]);
		}
		return *this;
	}
	template<typename C>
	datastream& popValueVector(std::vector<C>& data)
	{
		unsigned int nSize = 0;
		popValue(nSize);
		for (size_t i=0; i<nSize; ++i)
		{
			C tmp;
			popValue(tmp);
			data.push_back(tmp);
		}            
		return *this;
	}
public:

	char* inc_size(size_t delta_size)
	{
		size_t last_size=size();
		resize(last_size+delta_size);
		return &operator[](last_size);
	}
	datastream& popSize(int iSize)
	{
		if (iSize<=0)
		{
			return *this;
		}
		if (iSize>(int)size())
		{
			return *this;
		}
		erase(begin(), begin()+iSize);
		return *this;
	}
	template<typename C>
	datastream& push(C data)
	{
		memcpy(inc_size(sizeof(data)), &data, sizeof(data));
		return *this;
	}

	template<typename C>
	datastream& pop(C& data)
	{
		if (size() < sizeof(data))
		{
			return *this;
		}
		memcpy(&data, &operator[](0), sizeof(data));
		erase(begin(), begin()+sizeof(data));
		return *this;
	}
};