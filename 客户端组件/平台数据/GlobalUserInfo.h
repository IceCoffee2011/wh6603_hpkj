#ifndef GLOBAL_USER_INFO_HEAD_FILE
#define GLOBAL_USER_INFO_HEAD_FILE

#pragma once

#include "PlatformDataHead.h"

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//�û���Ϣ
struct tagGlobalUserData
{
	//��������
	DWORD							dwUserID;							//�û� I D
	DWORD							dwExperience;						//�û�����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��¼����

	//�û�����
	WORD							wFaceID;							//ͷ������
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������

	//�û��ɼ�
	SCORE							lScore;								//�û�����
	SCORE							lInsure;							//�û�����
	SCORE                           lUserMedal;                         //�û�����

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��

	//��չ��Ϣ
	TCHAR                           szAddrDescribe[LEN_ADDR_DESCRIBE];  //��ַ����   
};

//��չ����
struct tagIndividualUserData
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUserNote[LEN_USER_NOTE];			//�û�˵��
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�绰����
	TCHAR							szSeatPhone[LEN_SEAT_PHONE];		//�̶��绰
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ƶ��绰

	//��ϵ����
	TCHAR							szQQ[LEN_QQ];						//Q Q ����
	TCHAR							szEMail[LEN_EMAIL];					//�����ʼ�
	TCHAR							szDwellingPlace[LEN_DWELLING_PLACE];//��ϵ��ַ
};

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class PLATFORM_DATA_CLASS CGlobalUserInfo
{
	//�û���Ϣ
protected:
	tagGlobalUserData				m_GlobalUserData;						//�û�����
	tagIndividualUserData			m_IndividualUserData;					//��չ����

	//��̬����
protected:
	static CGlobalUserInfo *		m_pGlobalUserInfo;						//�û���Ϣ

	//��������
public:
	//���캯��
	CGlobalUserInfo();
	//��������
	virtual ~CGlobalUserInfo();

	//���ܺ���
public:
	//��������
	VOID ResetUserInfoData();
	//�û�����
	tagGlobalUserData * GetGlobalUserData() { return &m_GlobalUserData; }
	//��չ����
	tagIndividualUserData * GetIndividualUserData() { return &m_IndividualUserData; }

	//��̬����
public:
	//��ȡ����
	static CGlobalUserInfo * GetInstance() { return m_pGlobalUserInfo; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif