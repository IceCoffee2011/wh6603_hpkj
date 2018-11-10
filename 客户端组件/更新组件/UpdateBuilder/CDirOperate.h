//////////////////////////////////////////////////////////
// CDirOperate.h�ļ�

#ifndef __CDIROPERATE_H_
#define __CDIROPERATE_H_
#pragma once

#include <afxwin.h>

class CDirOperate
{
public:
	CDirOperate();
	virtual ~CDirOperate();

	//�������ַ�����lpszDir��ָ����Ŀ¼
	static void CreateAllDirectory(LPCTSTR lpszDir);
	//ɾ����lpszDir������Ŀ¼
	static void DeleteAllDirectory(LPCTSTR lpszDir);
	//�ж�ָ����Ŀ¼�Ƿ����
    static BOOL CheckFolderExist(LPCTSTR lpszDir);
    //�ж�һ���ļ��Ƿ���ڣ���ע���ж϶�����Ŀ¼
    static BOOL IsFileExist(LPCTSTR lpszFilePath);

protected:
	//�ж�һ��Ŀ¼�Ƿ���ڣ���ע�����������׼ȷ����
	static BOOL IsDirExist(LPCTSTR lpszDir);
	//�ݹ�ѭ��ɾ��ָ��Ŀ¼�µ������ļ�������Ŀ¼
	static void RecursiveDelete(LPCTSTR lpszPath); 
};

#endif