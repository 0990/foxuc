#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�
#include "json/json.h"
#include "util/Util.h"
//�����ļ�
#include "Macro.h"
#include "Define.h"

//�ṹ�ļ�
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"
#include "datastream.h"

//////////////////////////////////////////////////////////////////////////////////

//����汾
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//��ܰ汾
#define VERSION_PLAZA				PROCESS_VERSION(9,0,3)				//�����汾
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(6,0,3)				//�ֻ��汾
#define VERSION_MOBILE_IOS			PROCESS_VERSION(6,0,3)				//�ֻ��汾

//�汾����
#define VERSION_EFFICACY			0									//Ч��汾
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

#ifndef _DEBUG

//ƽ̨����
const TCHAR szProduct[]=TEXT("0660��Ϸ");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("0660QPGamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("0660QPGame");					        //��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("");					//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("");			//ƽ̨��վ

#else

//////////////////////////////////////////////////////////////////////////////////
//�ڲ�汾

//ƽ̨����
const TCHAR szProduct[]=TEXT("��������ƽ̨");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("0660QPGamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("0660QPGame");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("");						//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("");				//ƽ̨��վ

#endif

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");						//ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");						//�û����ݿ�
const TCHAR szTreasureDB[]=TEXT("QPTreasureDB");						//�Ƹ����ݿ�
const TCHAR szExerciseDB[]=TEXT("QPExerciseDB");						//��ϰ���ݿ�

//////////////////////////////////////////////////////////////////////////////////

//��Ȩ��Ϣ
const TCHAR szCompilation[]=TEXT("4B536A49-1246-45EB-8363-FF4C7DF645CC");

//////////////////////////////////////////////////////////////////////////////////

#endif