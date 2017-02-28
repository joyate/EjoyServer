#include "Dll_Common.h"
#include "IDataBase.h"
#include "DataBaseImpl.h"




#ifdef __cplusplus
extern "C" {
#endif


DLL_EXPORT const char* GetDataProxyName()
{
	return "mysql";
}


DLL_EXPORT IDataBase* CreateDataProxy()
{
	return new DataBaseImpl();
}


DLL_EXPORT void ReleaseDataProxy(IDataBase* _proxy)
{
	delete _proxy;
}




#ifdef __cplusplus
}
#endif
