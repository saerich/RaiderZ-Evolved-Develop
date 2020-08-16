#ifndef _MMONITORNETINFO
#define _MMONITORNETINFO


#include <string>

using std::string;

namespace mmonitor {

class MMonitorNetInfo
{
private :
	string	m_strIP;
    DWORD	m_dwIP;

	WORD	m_nPort;
	WORD	m_nPortHtoN;

public :
	MMonitorNetInfo() {}
	~MMonitorNetInfo() {}

	void SetIPStr( const string& strIP );					// DWORD�� ip�� �ڵ����� �������ش�.
	void SetIP( const DWORD dwIP )							{ m_dwIP = dwIP; }
	void SetIP( const DWORD dwIP, const string& strIP )		{ m_dwIP = dwIP; m_strIP = strIP; }

	void SetPort( const WORD nPort );						// NtoS�� Port�� �ڵ����� �������ش�. 
	void SetPortNtoH( const WORD nNetPort );
	
	const string&	GetIPStr() const						{ return m_strIP; }
	const DWORD		GetIP() const							{ return m_dwIP; }
	const WORD		GetPort() const							{ return m_nPort; }
	const WORD		GetPortHtoN() const						{ return m_nPortHtoN; }
	const string	GetPortStr() const;
};

}
#endif