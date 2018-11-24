#pragma once

#define DLLEXPORT __declspec(dllexport)

#define REGISTER_INTERFACE(__class__, __interface__) \
public: \
class X##__interface__ : public I##__interface__ \
{ \
private: \
	__class__##* m_pThis; \
public: \
	X##__interface__() \
	{ \
		m_pThis = (__class__*)((unsigned long long)this - (unsigned long long)(&((__class__*)0)->m_x##__interface__)); \
		GetCore()->RegisterInterface(this); \
	} \
	operator __class__##*() {return m_pThis;}
#define REGISTER_INTERFACE_END(__interface__) \
} m_x##__interface__; \
public: \
operator X##__interface__() {return m_x##__interface__;} \
operator X##__interface__##*() {return &m_x##__interface__;}

#define DECLARE_MANAGER(__dll_key__) \
public: \
friend __dll_key__##_EXPORT CPluginManager* GetInstance(); \
private: \
static CPluginManager* s_pGlobalInstance;

#define EXPORT_CLASS(__dll_key__) \
extern "C" __dll_key__##_EXPORT CPluginManager* GetInstance();

#define DEFINE_MANAGER(__dll_key__, __class__) \
CPluginManager* __class__##::s_pGlobalInstance = nullptr; \
extern "C" __dll_key__##_EXPORT CPluginManager* GetInstance() \
{ \
	if(__class__##::s_pGlobalInstance == nullptr) \
		return __class__##::s_pGlobalInstance = new __class__(); \
	return __class__##::s_pGlobalInstance;\
}