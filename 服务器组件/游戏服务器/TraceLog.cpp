#include "StdAfx.h"
#include "TraceLog.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CTraceLog::CTraceLog()
{
	//���ñ���
	m_bLoging = false;

	return;
}

//��������
CTraceLog::~CTraceLog()
{
	CloseLog();
}

//׷����Ϣ
bool CTraceLog::TraceString(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	//�����־
	if( m_bLoging && m_LogFile.is_open() )
	{
		if( TraceLevel == TraceLevel_Exception )
		{
			//��ȡʱ��
			SYSTEMTIME SystemTime;
			CHAR szTimeBuffer[64];
			GetLocalTime(&SystemTime);
			_snprintf(szTimeBuffer,CountArray(szTimeBuffer),"�� %04d-%02d-%02d %02d:%02d:%02d ��",
				SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

			m_LogFile << szTimeBuffer << (LPCSTR)CT2A(pszString) << "\n";

			m_LogFile.flush();
		}
	}

	return true;
}

//������־
bool CTraceLog::OpenLog(LPCTSTR lpszFile)
{
	//Ч��
	ASSERT(!m_bLoging);
	if( m_bLoging ) return false;

	ASSERT( !m_LogFile.is_open() );
	if( m_LogFile.is_open() )
		m_LogFile.close();

	m_bLoging = true;

	m_LogFile.open( (LPCSTR)CT2A(lpszFile),std::ios::out|std::ios::app );

	return m_bLoging;
}

//�ر���־
bool CTraceLog::CloseLog()
{
	if( m_LogFile.is_open() )
		m_LogFile.close();

	m_bLoging = false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
