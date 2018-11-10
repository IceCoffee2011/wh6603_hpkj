///////////////////////////////////////////////////////
// CDirOperate.cpp�ļ�
#include "stdAfx.h"
#include "CDirOperate.h"

CDirOperate::CDirOperate()
{
}

CDirOperate::~CDirOperate()
{
}

void CDirOperate::CreateAllDirectory(LPCTSTR lpszDir)
{
	int nPos = 0;
	CString strTemp = lpszDir;
	if(strTemp.Right(1)!="\\")
		strTemp += "\\";
	
	//��㴴��Ŀ¼
	while((nPos = strTemp.Find('\\',nPos+1)) != -1)
	{
		if(nPos != 2) //�ų���һ��'\\',(�磺C:\\)
		{
			if(!IsDirExist(strTemp.Left(nPos+1))) //�ж��Ƿ����
				::CreateDirectory(strTemp.Left(nPos+1),NULL);
		}
	}
}

//�ж�ָ����Ŀ¼�Ƿ����
BOOL CDirOperate::IsDirExist(LPCTSTR lpszDir)
{
	BOOL bExist;
	int n;
	CString szTemp = lpszDir;
	CString szDir;
	n = szTemp.ReverseFind('\\');
	if(n<=0)
		return FALSE;
	szDir = szTemp.Left(n);

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szDir,&wfd);
	if(hFind == INVALID_HANDLE_VALUE)
		bExist = FALSE;
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			bExist = TRUE;//Ŀ¼����
		else
			bExist = FALSE;//Ŀ¼������
		FindClose(hFind);
	}
	return bExist;
}

//�ж�ָ����Ŀ¼�Ƿ����
BOOL CDirOperate::CheckFolderExist(LPCTSTR lpszDir)
{
    CString strTemp = lpszDir;
    strTemp=strTemp.TrimRight(_T("\\"));
	strTemp=strTemp.TrimRight(_T("/"));
	
	WIN32_FIND_DATA  wfd;
    BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strTemp.GetString(), &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        rValue = TRUE;   
    }
    FindClose(hFind);
    return rValue;
}


//ɾ������Ŀ¼�µ������ļ�������Ŀ¼
void CDirOperate::RecursiveDelete(LPCTSTR lpszPath)
{
	CString szTemp = lpszPath;
	CString szPath; 
	if(szTemp.Right(1) != "\\")
		szTemp += "\\";
	szPath = szTemp; //����ԭʼĿ¼������ʹ��
	szTemp += "*.*";

	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	hFind = FindFirstFile(szTemp,&wfd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//����Ϊ"."��Ŀ¼����Ŀ¼��Ϊ".."��Ŀ¼������һ��Ŀ¼
			if(wfd.cFileName[0] == '.')
				continue;

			if(!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString szFilePathName;//�ļ���ȫ·����
				//wfd.cFileNameֻ���ļ�����û��·����szFilePathName�������ļ���ȫ·������
				szFilePathName = szPath + wfd.cFileName;
				::SetFileAttributes(szFilePathName,FILE_ATTRIBUTE_NORMAL);
				::DeleteFile(szFilePathName);
			}
			else
			{
				CString szFilePathName;
				szFilePathName = szPath + wfd.cFileName;
				RecursiveDelete(szFilePathName);
				::SetFileAttributes(szFilePathName,FILE_ATTRIBUTE_NORMAL);
				//ɾ����Ŀ¼(RemoveDirectoryֻ��ɾ����Ŀ¼)
				::RemoveDirectory(szFilePathName);
			}
		}while(::FindNextFile(hFind,&wfd));
		::FindClose(hFind);
	}
}

//ɾ������Ŀ¼
void CDirOperate::DeleteAllDirectory(LPCTSTR lpszDir)
{ 
	RecursiveDelete(lpszDir); //����ɾ����Ŀ¼�µ������ļ�������Ŀ¼
	::RemoveDirectory(lpszDir); //ɾ����Ŀ¼
}

//-------------------------------------------------------------------------
// Function Name    :IsFileExist
// Parameter(s)     :LPCTSTR lpszFilePath	File path
// Return           :
// Create			:2007-4-23 15:13	Jerry.Wang
// Memo             :verify the file exist
//-------------------------------------------------------------------------
BOOL CDirOperate::IsFileExist(LPCTSTR lpszFilePath)
{
	BOOL bExist = FALSE;
	HANDLE hFile = NULL;

	hFile = CreateFile( lpszFilePath
		, GENERIC_READ
		, FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL
		, OPEN_EXISTING
		, 0
		, 0
		);

	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hFile );
		bExist = TRUE;
	}

	return bExist;
}