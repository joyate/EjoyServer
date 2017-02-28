#include "DataProxyPlugin.h"


const char* sz_plugin_dbn_get		= "GetDataProxyName";
const char* sz_plugin_db_create		= "CreateDataProxy";
const char* sz_plugin_db_release	= "ReleaseDataProxy";

CDataProxyPlugin::CDataProxyPlugin(void):
	m_dbn_get_func(NULL),
	m_db_create_func(NULL),
	m_db_release_func(NULL),
#ifdef WIN32
	m_module(NULL)
#elif _LINUX
	m_soptr(NULL)
#endif
{
}


CDataProxyPlugin::~CDataProxyPlugin(void)
{
#ifdef WIN32
	if (m_module)
	{
		FreeLibrary(m_module);
	}
#elif _LINUX
	if (m_soptr)
	{
		dlclose(m_soptr);
	}
#endif
}

bool CDataProxyPlugin::LoadPlugin( const char* sz_plugin_name )
{

#ifdef WIN32
	m_module = ::LoadLibraryA(sz_plugin_name);
	if (!m_module)
	{
		return false;
	}

	m_dbn_get_func = (DataBaseNameGetFunc)GetProcAddress(m_module,sz_plugin_dbn_get);
	m_db_create_func = (DataBaseCreateFunc)GetProcAddress(m_module,sz_plugin_db_create);
	m_db_release_func = (DataBaseRelease)GetProcAddress(m_module,sz_plugin_db_release);

#else

	m_soptr = dlopen(sz_plugin_name);
	if (!m_soptr)
	{
		return false;
	}

	m_dbn_get_func = (DataBaseNameGetFunc)dlsym(m_soptr,sz_plugin_dbn_get);
	m_db_create_func = (DataBaseCreateFunc)dlsym(m_soptr,sz_plugin_db_create);
	m_db_release_func = (DataBaseRelease)dlsym(m_soptr,sz_plugin_db_release);

#endif

	return true;
}

IDataBase* CDataProxyPlugin::Create()
{
	if (m_db_create_func)
	{
		return m_db_create_func();
	}

	return NULL;
}

const char* CDataProxyPlugin::GetPluginName()
{
	if (m_dbn_get_func)
	{
		return m_dbn_get_func();
	}

	return NULL;
}

void CDataProxyPlugin::ReleasePlugin( IDataBase* _db_plugin )
{
	if (m_db_release_func)
	{
		m_db_release_func(_db_plugin);
	}
}
