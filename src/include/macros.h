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

#define GENERATE_FUNCTOR_0(__base, __function) \
struct __function##Functor : IFunctor<__base> { \
	void operator() (__base* obj) override { obj->##__function##(); } \
};

#define GENERATE_FUNCTOR_1(__base, __function, __arg_type) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg_type arg) : a(arg) {} \
	void operator() (__base* obj) override { obj->##__function##(a); } \
private: __arg_type a; \
};

#define GENERATE_FUNCTOR_1_1(__base, __function, __arg_type, __arg_default) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg_type arg = __arg_default) \
		: a(arg) {} \
	void operator() (__base* obj) override { obj->##__function##(a); } \
private: __arg_type a; \
};

#define GENERATE_FUNCTOR_2_1(__base, __function, __arg1_type, __arg2_type, __arg2_default) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg1_type arg1, __arg2_type arg2 = __arg2_default) \
		: a1(arg1), a2(arg2) {} \
	void operator() (__base* obj) override { obj->##__function##(a1, a2); } \
private: __arg1_type a1; __arg2_type a2; \
};

#define GENERATE_FUNCTOR_2_2(__base, __function, __arg1_type, __arg2_type, __arg1_default, __arg2_default) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg1_type arg1 = __arg1_default, __arg2_type arg2 = __arg2_default) \
		: a1(arg1), a2(arg2) {} \
	void operator() (__base* obj) override { obj->##__function##(a1, a2); } \
private: __arg1_type a1; __arg2_type a2; \
};

#define GENERATE_FUNCTOR_3_1(__base, __function, __arg1_type, __arg2_type, __arg3_type, __arg3_default) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg1_type arg1, __arg2_type arg2, __arg3_type arg3 = __arg3_default) \
		: a1(arg1), a2(arg2), a3(arg3) {} \
	void operator() (__base* obj) override { obj->##__function##(a1, a2, a3); } \
private: __arg1_type a1; __arg2_type a2; __arg3_type a3; \
};

#define GENERATE_FUNCTOR_3_2(__base, __function, __arg1_type, __arg2_type, __arg3_type, __arg2_default, __arg3_default) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg1_type arg1, __arg2_type arg2 = __arg2_default, __arg3_type arg3 = __arg3_default) \
		: a1(arg1), a2(arg2), a3(arg3) {} \
	void operator() (__base* obj) override { obj->##__function##(a1, a2, a3); } \
private: __arg1_type a1; __arg2_type a2; __arg3_type a3; \
};

#define GENERATE_FUNCTOR_3_3(__base, __function, __arg1_type, __arg2_type, __arg3_type, __arg1_default, __arg2_default, __arg3_default) \
struct __function##Functor : IFunctor<__base> { \
	__function##Functor(__arg1_type arg1 = __arg1_default,, __arg2_type arg2 = __arg2_default, __arg3_type arg3 = __arg3_default) \
		: a1(arg1), a2(arg2), a3(arg3) {} \
	void operator() (__base* obj) override { obj->##__function##(a1, a2, a3); } \
private: __arg1_type a1; __arg2_type a2; __arg3_type a3; \
};

#define RET_GENERATE_FUNCTOR_0(__return, __base, __function) \
struct __function##Functor : IFunctor<__base, __return> { \
	__return operator() (__base* obj) override { return obj->##__function##(); } \
};

#define RET_GENERATE_FUNCTOR_1(__return, __base, __function, __arg_type) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg_type arg) : a(arg) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a); } \
private: __arg_type a; \
};

#define RET_GENERATE_FUNCTOR_1_1(__return, __base, __function, __arg_type, __arg_default) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg_type arg = __arg_default) \
		: a(arg) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a); } \
private: __arg_type a; \
};

#define RET_GENERATE_FUNCTOR_2_1(__return, __base, __function, __arg1_type, __arg2_type, __arg2_default) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg1_type arg1, __arg2_type arg2 = __arg2_default) \
		: a1(arg1), a2(arg2) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a1, a2); } \
private: __arg1_type a1; __arg2_type a2; \
};

#define RET_GENERATE_FUNCTOR_2_2(__return, __base, __function, __arg1_type, __arg2_type, __arg1_default, __arg2_default) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg1_type arg1 = __arg1_default, __arg2_type arg2 = __arg2_default) \
		: a1(arg1), a2(arg2) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a1, a2); } \
private: __arg1_type a1; __arg2_type a2; \
};

#define RET_GENERATE_FUNCTOR_3_1(__return, __base, __function, __arg1_type, __arg2_type, __arg3_type, __arg3_default) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg1_type arg1, __arg2_type arg2, __arg3_type arg3 = __arg3_default) \
		: a1(arg1), a2(arg2), a3(arg3) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a1, a2, a3); } \
private: __arg1_type a1; __arg2_type a2; __arg3_type a3; \
};

#define RET_GENERATE_FUNCTOR_3_2(__return, __base, __function, __arg1_type, __arg2_type, __arg3_type, __arg2_default, __arg3_default) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg1_type arg1, __arg2_type arg2 = __arg2_default, __arg3_type arg3 = __arg3_default) \
		: a1(arg1), a2(arg2), a3(arg3) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a1, a2, a3); } \
private: __arg1_type a1; __arg2_type a2; __arg3_type a3; \
};

#define RET_GENERATE_FUNCTOR_3_3(__return, __base, __function, __arg1_type, __arg2_type, __arg3_type, __arg1_default, __arg2_default, __arg3_default) \
struct __function##Functor : IFunctor<__base, __return> { \
	__function##Functor(__arg1_type arg1 = __arg1_default,, __arg2_type arg2 = __arg2_default, __arg3_type arg3 = __arg3_default) \
		: a1(arg1), a2(arg2), a3(arg3) {} \
	__return operator() (__base* obj) override { return obj->##__function##(a1, a2, a3); } \
private: __arg1_type a1; __arg2_type a2; __arg3_type a3; \
};