#ifndef TRACE_LOG_HEAD_FILE
#define TRACE_LOG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////////

//�����־
class CTraceLog
{
	//�������
public:
	bool							m_bLoging;							//������־
	std::ofstream					m_LogFile;							//�����־

	//��������
public:
	//���캯��
	CTraceLog();
	//��������
	virtual ~CTraceLog();

	//��Ϣ�ӿ�
public:
	//׷����Ϣ
	virtual bool TraceString(LPCTSTR pszString, enTraceLevel TraceLevel);

	//���ܺ���
public:
	//������־
	bool OpenLog(LPCTSTR lpszFile);
	//�ر���־
	bool CloseLog();
	//��ѯ״̬
	bool IsLoging() { return m_bLoging; }
};

//////////////////////////////////////////////////////////////////////////

#endif