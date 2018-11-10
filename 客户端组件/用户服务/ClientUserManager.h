#ifndef CLIENT_USER_MANAGER_HEAD_FILE
#define CLIENT_USER_MANAGER_HEAD_FILE

#pragma once

#include "UserServiceHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CClientUserItem;

//�û�����
typedef CWHArray<CClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//�û���Ϣ
class CClientUserItem : public IClientUserItem
{
	//��Ԫ����
	friend class CGameUserManager;
	friend class CPlazaUserManager;

	//���Ա���
protected:
	tagUserInfo						m_UserInfo;							//�û���Ϣ

	//��չ����
protected:
	BYTE							m_cbCompanion;						//�û���ϵ
	TCHAR							m_szUserNote[LEN_USERNOTE];			//�û���ע

	//��������
public:
	//���캯��
	CClientUserItem();
	//��������
	virtual ~CClientUserItem();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���Խӿ�
public:
	//��Ҫ�ȼ�
	virtual LONG GetImportOrder();
	//�û���Ϣ
	virtual tagUserInfo * GetUserInfo() { return &m_UserInfo; }

	//������Ϣ
public:
	//ͷ������
	virtual WORD GetFaceID() { return m_UserInfo.wFaceID; }
	//�û��Ա�
	virtual BYTE GetGender() { return m_UserInfo.cbGender; }
	//�û���ʶ
	virtual DWORD GetUserID() { return m_UserInfo.dwUserID; }
	//�û��ǳ�
	virtual LPCTSTR GetNickName() { return m_UserInfo.szNickName; }
	//��ַ����
	virtual LPCTSTR GetAddrDescribe() {return m_UserInfo.szAddrDescribe; };

	//�ȼ���Ϣ
public:
	//��Ա�ȼ�
	virtual BYTE GetMemberOrder() { return m_UserInfo.cbMemberOrder; }
	//����ȼ�
	virtual BYTE GetMasterOrder() { return m_UserInfo.cbMasterOrder; }

	//�û�״̬
public:
	//�û�����
	virtual WORD GetTableID() { return m_UserInfo.wTableID; }
	//�û�����
	virtual WORD GetChairID() { return m_UserInfo.wChairID; }
	//�û�״̬
	virtual BYTE GetUserStatus() { return m_UserInfo.cbUserStatus; }

	//������Ϣ
public:
	//������ֵ
	virtual SCORE GetUserScore() { return m_UserInfo.lScore; }
	//������ֵ
	virtual SCORE GetUserInsure() { return m_UserInfo.lInsure; }
	//������ֵ
	virtual SCORE GetUserMedal() { return m_UserInfo.lUserMedal; };
	//������ֵ
	virtual DWORD GetUserExperience() { return m_UserInfo.dwExperience; }

	//��Ϸ��Ϣ
public:
	//ʤ������
	virtual DWORD GetUserWinCount() { return m_UserInfo.dwWinCount; }
	//ʧ������
	virtual DWORD GetUserLostCount() { return m_UserInfo.dwLostCount; }
	//�;�����
	virtual DWORD GetUserDrawCount() { return m_UserInfo.dwDrawCount; }
	//��������
	virtual DWORD GetUserFleeCount() { return m_UserInfo.dwFleeCount; }
	//��Ϸ����
	virtual DWORD GetUserPlayCount() { return m_UserInfo.dwWinCount+m_UserInfo.dwLostCount+m_UserInfo.dwDrawCount+m_UserInfo.dwFleeCount; }

	//������Ϣ
public:
	//�û�ʤ��
	virtual float GetUserWinRate();
	//�û�����
	virtual float GetUserLostRate();
	//�û�����
	virtual float GetUserDrawRate();
	//�û�����
	virtual float GetUserFleeRate();

	//�û���ϵ
public:
	//��ȡ��ϵ
	virtual BYTE GetUserCompanion() { return m_cbCompanion; }
	//���ù�ϵ
	virtual VOID SetUserCompanion(BYTE cbCompanion) { m_cbCompanion=cbCompanion; }

	//�û���ע
public:
	//���ñ�ע
	virtual VOID SetUserNoteInfo(LPCTSTR pszUserNote);
	//��ȡ��ע
	virtual LPCTSTR GetUserNoteInfo() { return m_szUserNote; }
};

//////////////////////////////////////////////////////////////////////////////////

//�û�����
class CPlazaUserManager : public IPlazaUserManager
{
	//��������
protected:
	CClientUserItemArray			m_UserItemActive;					//�����
	static CClientUserItemArray		m_UserItemStorage;					//�洢����

	//����ӿ�
protected:
	IUserManagerSink *				m_pIUserManagerSink;				//֪ͨ�ӿ�
	IUserInformation *				m_pIUserInformation;				//�û���Ϣ

	//��������
public:
	//���캯��
	CPlazaUserManager();
	//��������
	virtual ~CPlazaUserManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetUserInformation(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx);

	//����ӿ�
public:
	//ɾ���û�
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem);
	//�����û�
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo);

	//���½ӿ�
public:
	//���»���
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore);
	//����״̬
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus);
	//��չ��Ϣ
	virtual bool UpdateUserItemSpreadInfo(IClientUserItem * pIClientUserItem, tagUserSpreadInfo * const pUserSpreadInfo);

	//���ҽӿ�
public:
	//ö���û�
	virtual IClientUserItem * EnumUserItem(WORD wEnumIndex);
	//�����û�
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID);
	//�����û�
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName);
};

//////////////////////////////////////////////////////////////////////////////////

//�û�����
class CGameUserManager : public IGameUserManager
{
	//�û�����
protected:
	CClientUserItemArray			m_UserItemLookon;					//�Թ�����
	CClientUserItemArray			m_UserItemStorage;					//�洢����

	//��������
protected:
	CClientUserItem *				m_pTableUserItem[MAX_CHAIR];		//��Ϸ�û�

	//����ӿ�
protected:
	IUserManagerSink *				m_pIUserManagerSink;				//֪ͨ�ӿ�

	//��������
public:
	//���캯��
	CGameUserManager();
	//��������
	virtual ~CGameUserManager();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx);
	//��ȡ�ӿ�
	virtual VOID * GetUserManagerSink(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//�����û�
	virtual bool ResetUserItem();
	//ɾ���û�
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem);
	//�����û�
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, BYTE cbCompanion, LPCTSTR pszUserNote);

	//���½ӿ�
public:
	//���»���
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore);
	//����״̬
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus);
	//����״̬
	virtual bool UpdateUserItemSpreadInfo(IClientUserItem * pIClientUserItem, tagUserSpreadInfo * const pUserSpreadInfo);

	//��ѯ�ӿ�
public:
	//��Ϸ�û�
	virtual IClientUserItem * GetTableUserItem(WORD wChariID);
	//�Թ��û�
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex);

	//���ҽӿ�
public:
	//�����û�
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID);
	//�����û�
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName);
};

//////////////////////////////////////////////////////////////////////////////////

#endif