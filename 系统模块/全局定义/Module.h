#ifndef MODULE_HEAD_FILE
#define MODULE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//ģ��ӿ�

#define VER_IUnknownEx INTERFACE_VERSION(1,1)
static const GUID IID_IUnknownEx={0x5feec21e,0xdbf3,0x46f0,0x9f,0x57,0xd1,0xcd,0x71,0x1c,0x46,0xde};

//�����ӿ�
interface IUnknownEx
{
	//�ͷŶ���
	virtual VOID Release()=NULL;
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//�汾�Ƚ�

//��Ʒ�汾
#define BULID_VER					0									//��Ȩ�汾
#define PRODUCT_VER					6									//��Ʒ�汾

//�ӿڰ汾
#define INTERFACE_VERSION(cbMainVer,cbSubVer)							\
		(DWORD)(														\
		(((BYTE)(PRODUCT_VER))<<24)+									\
		(((BYTE)(cbMainVer))<<16)+										\
		((BYTE)(cbSubVer)<<8))+											\
		((BYTE)(BULID_VER))

//ģ��汾
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)					\
		(DWORD)(														\
		(((BYTE)(PRODUCT_VER))<<24)+									\
		(((BYTE)(cbMainVer))<<16)+										\
		((BYTE)(cbSubVer)<<8)+											\
		(BYTE)(cbBuildVer))

//��Ʒ�汾
inline BYTE GetProductVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[3];
}

//��Ҫ�汾
inline BYTE GetMainVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[2];
}

//��Ҫ�汾
inline BYTE GetSubVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[1];
}

//����汾
inline BYTE GetBuildVer(DWORD dwVersion)
{
	return ((BYTE *)&dwVersion)[0];
}

//�汾�Ƚ�
inline bool InterfaceVersionCompare(DWORD dwQueryVer, DWORD dwInterfaceVer)
{
	if (GetSubVer(dwQueryVer)>GetSubVer(dwInterfaceVer)) return false;
	if (GetMainVer(dwQueryVer)!=GetMainVer(dwInterfaceVer)) return false;
	if (GetBuildVer(dwQueryVer)!=GetBuildVer(dwInterfaceVer)) return false;
	if (GetProductVer(dwQueryVer)!=GetProductVer(dwInterfaceVer)) return false;
	return true;
};

//////////////////////////////////////////////////////////////////////////////////
//�ڲ��ӿڲ�ѯ

//��ѯ�ӿ�
#define QUERYINTERFACE(Interface,Guid,dwQueryVer)															\
	if ((Guid==IID_##Interface)&&(InterfaceVersionCompare(dwQueryVer,VER_##Interface)))						\
		return static_cast<Interface *>(this);											

//��ѯ�ӿ�
#define QUERYINTERFACE_IUNKNOWNEX(BaseInterface,Guid,dwQueryVer)											\
	if ((Guid==IID_IUnknownEx)&&(InterfaceVersionCompare(dwQueryVer,VER_IUnknownEx)))						\
		return static_cast<IUnknownEx *>(static_cast<BaseInterface *>(this));			

//////////////////////////////////////////////////////////////////////////////////
//�ⲿ�ӿڲ�ѯ

//��ѯ�ӿ�
#define QUERY_ME_INTERFACE(Interface)																		\
	((Interface *)QueryInterface(IID_##Interface,VER_##Interface))

//��ѯ�ӿ�
#define QUERY_OBJECT_INTERFACE(Object,Interface)															\
	((Interface *)Object.QueryInterface(IID_##Interface,VER_##Interface))

//��ѯ�ӿ�
#define QUERY_OBJECT_PTR_INTERFACE(pObject,Interface)														\
	((pObject==NULL)?NULL:((Interface *)pObject->QueryInterface(IID_##Interface,VER_##Interface)))

//////////////////////////////////////////////////////////////////////////////////
//���ģ�帨��ģ��

//�����������
typedef VOID * (ModuleCreateProc)(REFGUID Gudi, DWORD dwInterfaceVer);

//���������ģ��
template <typename IModeluInterface> class CTempldateHelper
{
	//�ӿ�����
public:
	REFGUID							m_Guid;								//�ӿڱ�ʶ
	const DWORD						m_dwVersion;						//�ӿڰ汾

	//�������
public:
	CHAR							m_szCreateProc[32];					//��������
	TCHAR							m_szModuleDllName[MAX_PATH];		//�������

	//�ں˱���
public:
	HINSTANCE						m_hDllInstance;						//DLL ���
	IModeluInterface *				m_pIModeluInterface;				//ģ��ӿ�

	//��������
public:
	TCHAR							m_szDescribe[128];					//��������

	//��������
public:
	//���캯��
	CTempldateHelper(REFGUID Guid, DWORD dwVersion);
	//���캯��
	CTempldateHelper(REFGUID Guid, DWORD dwVersion, LPCTSTR pszModuleDll, LPCSTR pszCreateProc);
	//��������
	virtual ~CTempldateHelper();

	//������
public:
	//�ͷ����
	bool CloseInstance();
	//��������
	bool CreateInstance();

	//���ú���
public:
	//������Ϣ
	VOID SetModuleCreateInfo(LPCTSTR pszModuleDllName, LPCSTR pszCreateProc);

	//��������
public:
	//��ȡ����
	inline LPCTSTR GetErrorDescribe() const;
	//ָ������
	inline IModeluInterface * operator->() const;
	//��ȡ�ӿ�
	inline IModeluInterface * GetInterface() const;
};

//////////////////////////////////////////////////////////////////////////////////
// CTempldateHelper<IModeluInterface> ��������

//���캯��
template <typename IModeluInterface>
CTempldateHelper<IModeluInterface>::CTempldateHelper(REFGUID Guid, DWORD dwVersion) : m_dwVersion(dwVersion), m_Guid(Guid)
{
	//��������
	m_szDescribe[0]=0;

	//�ں���Ϣ
	m_hDllInstance=NULL;
	m_pIModeluInterface=NULL;

	//�������
	ZeroMemory(m_szCreateProc,sizeof(m_szCreateProc));
	ZeroMemory(m_szModuleDllName,sizeof(m_szModuleDllName));

	return;
}

//���캯��
template <typename IModeluInterface>
CTempldateHelper<IModeluInterface>::CTempldateHelper(REFGUID Guid, DWORD dwVersion, LPCTSTR pszModuleDll, LPCSTR pszCreateProc) : m_dwVersion(dwVersion), m_Guid(Guid)
{
	//��������
	m_szDescribe[0]=0;

	//�ں���Ϣ
	m_hDllInstance=NULL;
	m_pIModeluInterface=NULL;

	//�������
	lstrcpynA(m_szCreateProc,pszCreateProc,CountArray(m_szCreateProc));
	lstrcpyn(m_szModuleDllName,pszModuleDll,CountArray(m_szModuleDllName));

	return;
}

//��������
template <typename IModeluInterface>
CTempldateHelper<IModeluInterface>::~CTempldateHelper()
{
	CloseInstance();
}

//�������
template <typename IModeluInterface>
bool CTempldateHelper<IModeluInterface>::CreateInstance()
{
	//�ͷ����
	CloseInstance();

	//�������
	try
	{
		//Ч�����
		ASSERT(m_szCreateProc[0]!=0);
		ASSERT(m_szModuleDllName[0]!=0);

		//����ģ��
		m_hDllInstance=AfxLoadLibrary(m_szModuleDllName);
		if (m_hDllInstance==NULL) 
		{
			_sntprintf(m_szDescribe,CountArray(m_szDescribe),TEXT("��%s��ģ�����ʧ��"),m_szModuleDllName);
			return false;
		}

		//Ѱ�Һ���
		ModuleCreateProc * CreateProc=(ModuleCreateProc *)GetProcAddress(m_hDllInstance,m_szCreateProc);
		if (CreateProc==NULL) 
		{
			_sntprintf(m_szDescribe,CountArray(m_szDescribe),TEXT("�Ҳ����������������%s��"),m_szCreateProc);
			return false;
		}

		//�������
		m_pIModeluInterface=(IModeluInterface *)CreateProc(m_Guid,m_dwVersion);
		if (m_pIModeluInterface==NULL) 
		{
			_sntprintf(m_szDescribe,CountArray(m_szDescribe),TEXT("���ú�����%s�����ɶ���ʧ��"),m_szCreateProc);
			return false;
		}
	}
	catch (LPCTSTR pszError)
	{
		_sntprintf(m_szDescribe,CountArray(m_szDescribe),TEXT("���ڡ�%s�����������ʧ��"),pszError);
		return false;
	}
	catch (...)	
	{ 
		_sntprintf(m_szDescribe,CountArray(m_szDescribe),TEXT("�������������%s������δ֪�쳣�����������ʧ��"),m_szCreateProc);
		return false;
	}

	return true;
}

//�ͷ����
template <typename IModeluInterface>
bool CTempldateHelper<IModeluInterface>::CloseInstance()
{
	//���ñ���
	m_szDescribe[0]=0;

	//���ٶ���
	if (m_pIModeluInterface!=NULL)
	{
		m_pIModeluInterface->Release();
		m_pIModeluInterface=NULL;
	}

	//�ͷ� DLL
	if (m_hDllInstance!=NULL)
	{
		AfxFreeLibrary(m_hDllInstance);
		m_hDllInstance=NULL;
	}

	return true;
}

//������Ϣ
template <typename IModeluInterface>
VOID CTempldateHelper<IModeluInterface>::SetModuleCreateInfo(LPCTSTR pszModuleDllName, LPCSTR pszCreateProc)
{
	//������Ϣ
	lstrcpynA(m_szCreateProc,pszCreateProc,CountArray(m_szCreateProc));
	lstrcpyn(m_szModuleDllName,pszModuleDllName,CountArray(m_szModuleDllName));

	return;
}

//////////////////////////////////////////////////////////////////////////////////
// CTempldateHelper<IModeluInterface> ��������

//��ȡ����
template <typename IModeluInterface>
inline LPCTSTR CTempldateHelper<IModeluInterface>::GetErrorDescribe() const
{ 
	return m_szDescribe; 
}

//ָ������
template <typename IModeluInterface>
inline IModeluInterface * CTempldateHelper<IModeluInterface>::operator->() const
{ 
	return GetInterface(); 
}

//��ȡ�ӿ�
template <typename IModeluInterface>
inline IModeluInterface * CTempldateHelper<IModeluInterface>::GetInterface() const
{ 
	return m_pIModeluInterface; 
}

//////////////////////////////////////////////////////////////////////////////////
//���������

//�����������
#define DECLARE_CREATE_MODULE(OBJECT_NAME)																	\
extern "C" __declspec(dllexport) VOID * Create##OBJECT_NAME(REFGUID Guid, DWORD dwInterfaceVer)		\
{																											\
	C##OBJECT_NAME * p##OBJECT_NAME=NULL;																	\
	try																										\
	{																										\
		p##OBJECT_NAME=new C##OBJECT_NAME();																\
		if (p##OBJECT_NAME==NULL) throw TEXT("����ʧ��");													\
		VOID * pObject=p##OBJECT_NAME->QueryInterface(Guid,dwInterfaceVer);									\
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");														\
		return pObject;																						\
	}																										\
	catch (...) {}																							\
	SafeDelete(p##OBJECT_NAME);																				\
	return NULL;																							\
}

//����������
#define DECLARE_MODULE_DYNAMIC(OBJECT_NAME)																	\
class C##OBJECT_NAME##Helper : public CTempldateHelper<I##OBJECT_NAME>										\
{																											\
public:																										\
	C##OBJECT_NAME##Helper() : CTempldateHelper<I##OBJECT_NAME>(IID_I##OBJECT_NAME,VER_I##OBJECT_NAME) { }	\
};

//����������
#define DECLARE_MODULE_HELPER(OBJECT_NAME,MODULE_DLL_NAME,CREATE_FUNCTION_NAME)								\
class C##OBJECT_NAME##Helper : public CTempldateHelper<I##OBJECT_NAME>										\
{																											\
public:																										\
	C##OBJECT_NAME##Helper() : CTempldateHelper<I##OBJECT_NAME>(IID_I##OBJECT_NAME,							\
		VER_I##OBJECT_NAME,MODULE_DLL_NAME,CREATE_FUNCTION_NAME) { }										\
};

//////////////////////////////////////////////////////////////////////////////////

#endif