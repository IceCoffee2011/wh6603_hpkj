#include "StdAfx.h"
#include "ServerListManager.h"

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameTypeItem::CGameTypeItem()
{
	//���ñ���
	ZeroMemory(&m_GameType,sizeof(m_GameType));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameKindItem::CGameKindItem()
{
	//���ñ���
	ZeroMemory(&m_GameKind,sizeof(m_GameKind));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameServerItem::CGameServerItem()
{
	//���ñ���
	m_dwVirtualCount=0;

	//���ñ���
	ZeroMemory(&m_GameServer,sizeof(m_GameServer));

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CServerListManager::CServerListManager()
{
	//��������
	m_TypeItemMap.InitHashTable(PRIME_TYPE);
	m_KindItemMap.InitHashTable(PRIME_KIND);
	m_ServerItemMap.InitHashTable(PRIME_SERVER);

	return;
}

//��������
CServerListManager::~CServerListManager()
{
	//��������
	WORD wKey=0;

	//ɾ������
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_TypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		SafeDelete(pGameTypeItem);
	}
	for (INT_PTR i=0;i<m_TypeItemBuffer.GetCount();i++)
	{
		pGameTypeItem=m_TypeItemBuffer[i];
		SafeDelete(pGameTypeItem);
	}
	m_TypeItemMap.RemoveAll();
	m_TypeItemBuffer.RemoveAll();

	//ɾ������
	CGameKindItem * pGameKindItem=NULL;
	Position=m_KindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_KindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		SafeDelete(pGameKindItem);
	}
	for (INT_PTR i=0;i<m_KindItemBuffer.GetCount();i++)
	{
		pGameKindItem=m_KindItemBuffer[i];
		SafeDelete(pGameKindItem);
	}
	m_KindItemMap.RemoveAll();
	m_KindItemBuffer.RemoveAll();

	//ɾ������
	CGameServerItem * pGameServerItem=NULL;
	Position=m_ServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_ServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		SafeDelete(pGameServerItem);
	}
	for (INT_PTR i=0;i<m_ServerItemBuffer.GetCount();i++)
	{
		pGameServerItem=m_ServerItemBuffer[i];
		SafeDelete(pGameServerItem);
	}
	m_ServerItemMap.RemoveAll();
	m_ServerItemBuffer.RemoveAll();

	return;
}

//�����б�
VOID CServerListManager::ResetServerList()
{
	//�����б�
	DisuseKernelItem();
	DisuseServerItem();

	//�����б�
	CleanKernelItem();
	CleanServerItem();

	return;
}

//�����ں�
VOID CServerListManager::CleanKernelItem()
{
	//��������
	POSITION PositionType=m_TypeItemMap.GetStartPosition();
	POSITION PositionKind=m_KindItemMap.GetStartPosition();

	//��������
	while (PositionType!=NULL)
	{
		//��������
		WORD wTypeID=0;
		CGameTypeItem * pGameTypeItem=NULL;
		m_TypeItemMap.GetNextAssoc(PositionType,wTypeID,pGameTypeItem);

		//ɾ���ж�
		if (pGameTypeItem->m_bDisuse==true)
		{
			m_TypeItemMap.RemoveKey(wTypeID);
			m_TypeItemBuffer.Add(pGameTypeItem);
		}
	}

	//��������
	while (PositionKind!=NULL)
	{
		//��������
		WORD wKindID=0;
		CGameKindItem * pGameKindItem=NULL;
		m_KindItemMap.GetNextAssoc(PositionKind,wKindID,pGameKindItem);

		//ɾ���ж�
		if (pGameKindItem->m_bDisuse==true)
		{
			m_KindItemMap.RemoveKey(wKindID);
			m_KindItemBuffer.Add(pGameKindItem);
		}
	}

	return;
}

//������
VOID CServerListManager::CleanServerItem()
{
	//��������
	POSITION PositionServer=m_ServerItemMap.GetStartPosition();

	//ɾ������
	while (PositionServer!=NULL)
	{
		//��������
		WORD wServerID=0;
		CGameServerItem * pGameServerItem=NULL;
		m_ServerItemMap.GetNextAssoc(PositionServer,wServerID,pGameServerItem);

		//ɾ���ж�
		if (pGameServerItem->m_bDisuse==true)
		{
			//ɾ������
			m_ServerItemMap.RemoveKey(wServerID);
			m_ServerItemBuffer.Add(pGameServerItem);

			//��������
			CGameKindItem * pGameKindItem=NULL;
			if (m_KindItemMap.Lookup(pGameServerItem->m_GameServer.wKindID,pGameKindItem)==TRUE)
			{
				tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
				pGameKindItem->m_GameKind.dwOnLineCount-=pGameServer->dwOnLineCount;
			}
		}
	}

	return;
}

//�����ں�
VOID CServerListManager::DisuseKernelItem()
{
	//��������
	WORD wKey=0;
	DWORD dwKey=0;

	//��������
	CGameTypeItem * pGameTypeItem=NULL;
	POSITION Position=m_TypeItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);
		pGameTypeItem->m_bDisuse=true;
	}

	//��������
	CGameKindItem * pGameKindItem=NULL;
	Position=m_KindItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_KindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);
		pGameKindItem->m_bDisuse=true;
	}

	return;
}

//��������
VOID CServerListManager::DisuseServerItem()
{
	//��������
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;

	//��������
	POSITION Position=m_ServerItemMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_ServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);
		pGameServerItem->m_bDisuse=true;
	}

	return;
}

//ͳ������
DWORD CServerListManager::CollectOnlineInfo()
{
	//��������
	DWORD dwOnLineCount=0L;
	POSITION PositionKind=m_KindItemMap.GetStartPosition();
	
	//ͳ������
	while (PositionKind!=NULL)
	{
		//��������
		WORD wKindID=0;
		CGameKindItem * pGameKindItem=NULL;
		m_KindItemMap.GetNextAssoc(PositionKind,wKindID,pGameKindItem);

		//ͳ������
		if (pGameKindItem!=NULL) dwOnLineCount+=pGameKindItem->m_GameKind.dwOnLineCount;
	}

	return dwOnLineCount;
}

//��������
DWORD CServerListManager::CollectOnlineInfo(WORD wKindID)
{
	//��������
	DWORD dwOnLineCount=0L;
	POSITION PositionServer=m_ServerItemMap.GetStartPosition();

	//ö�ٷ���
	while (PositionServer!=NULL)
	{
		//��������
		WORD wServerID=0;
		CGameServerItem * pGameServerItem=NULL;
		m_ServerItemMap.GetNextAssoc(PositionServer,wServerID,pGameServerItem);

		//ͳ������
		if (pGameServerItem->m_GameServer.wKindID==wKindID)
		{
			dwOnLineCount+=pGameServerItem->m_GameServer.dwOnLineCount;
		}
	}

	return dwOnLineCount;
}

//��������
bool CServerListManager::InsertGameType(tagGameType * pGameType)
{
	//Ч�����
	ASSERT(pGameType!=NULL);
	if (pGameType==NULL) return false;

	//�����ִ�
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(pGameType->wTypeID,pGameTypeItem)==FALSE)
	{
		//��������
		try
		{
			INT_PTR nStroeCount=m_TypeItemBuffer.GetCount();
			if (nStroeCount>0)
			{
				pGameTypeItem=m_TypeItemBuffer[nStroeCount-1];
				m_TypeItemBuffer.RemoveAt(nStroeCount-1);
			}
			else
			{
				pGameTypeItem=new CGameTypeItem;
				if (pGameTypeItem==NULL) return false;
			}
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pGameTypeItem,sizeof(CGameTypeItem));
	}

	//��������
	pGameTypeItem->m_bDisuse=false;
	CopyMemory(&pGameTypeItem->m_GameType,pGameType,sizeof(tagGameType));

	//��������
	m_TypeItemMap[pGameType->wTypeID]=pGameTypeItem;

	return true;
}

//��������
bool CServerListManager::InsertGameKind(tagGameKind * pGameKind)
{
	//Ч�����
	ASSERT(pGameKind!=NULL);
	if (pGameKind==NULL) return false;

	//�����ִ�
	CGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(pGameKind->wKindID,pGameKindItem)==FALSE)
	{
		//��������
		try
		{
			INT_PTR nStroeCount=m_KindItemBuffer.GetCount();
			if (nStroeCount>0)
			{
				pGameKindItem=m_KindItemBuffer[nStroeCount-1];
				m_KindItemBuffer.RemoveAt(nStroeCount-1);
			}
			else
			{
				pGameKindItem=new CGameKindItem;
				if (pGameKindItem==NULL) return false;
			}
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pGameKindItem,sizeof(CGameKindItem));
	}

	//��������
	pGameKindItem->m_bDisuse=false;
	CopyMemory(&pGameKindItem->m_GameKind,pGameKind,sizeof(tagGameKind));

	//��������
	m_KindItemMap[pGameKind->wKindID]=pGameKindItem;

	return true;
}

//���뷿��
bool CServerListManager::InsertGameServer(tagGameServer * pGameServer)
{
	//Ч�����
	ASSERT(pGameServer!=NULL);
	if (pGameServer==NULL) return false;

	//���ҷ���
	WORD wKindID=0;
	DWORD dwOnLineCount=0L;
	CGameServerItem * pGameServerItem=NULL;

	//��ȡ����
	if (m_ServerItemMap.Lookup(pGameServer->wServerID,pGameServerItem)==FALSE)
	{
		//��������
		try
		{
			INT_PTR nStroeCount=m_ServerItemBuffer.GetCount();
			if (nStroeCount>0)
			{
				pGameServerItem=m_ServerItemBuffer[nStroeCount-1];
				m_ServerItemBuffer.RemoveAt(nStroeCount-1);
			}
			else
			{
				pGameServerItem=new CGameServerItem;
				if (pGameServerItem==NULL) return false;
			}
		}
		catch (...) { return false; }

		//���ñ���
		ZeroMemory(pGameServerItem,sizeof(CGameServerItem));
	}
	else
	{
		//�������
		wKindID=pGameServerItem->m_GameServer.wKindID;
		dwOnLineCount=pGameServerItem->m_GameServer.dwOnLineCount;
	}

	//��������
	pGameServerItem->m_bDisuse=false;
	CopyMemory(&pGameServerItem->m_GameServer,pGameServer,sizeof(tagGameServer));

	//��������
	if (wKindID!=pGameServerItem->m_GameServer.wKindID)
	{
		//��������
		CGameKindItem * pGameKindItemLast=NULL;
		CGameKindItem * pGameKindItemCurrent=NULL;

		//��ʷ����
		if ((wKindID!=0)&&(m_KindItemMap.Lookup(wKindID,pGameKindItemLast)==TRUE))
		{
			pGameKindItemLast->m_GameKind.dwOnLineCount-=dwOnLineCount;
		}

		//��ǰ����
		if (m_KindItemMap.Lookup(pGameServer->wKindID,pGameKindItemCurrent)==TRUE)
		{
			pGameKindItemCurrent->m_GameKind.dwOnLineCount+=pGameServer->dwOnLineCount;
		}
	}
	else
	{
		//��������
		CGameKindItem * pGameKindItem=NULL;
		m_KindItemMap.Lookup(wKindID,pGameKindItem);

		//��������
		if (pGameKindItem!=NULL)
		{
			pGameKindItem->m_GameKind.dwOnLineCount-=dwOnLineCount;
			pGameKindItem->m_GameKind.dwOnLineCount+=pGameServer->dwOnLineCount;
		}
	}

	//��������
	m_ServerItemMap[pGameServer->wServerID]=pGameServerItem;

	return true;
}

//ɾ������
bool CServerListManager::DeleteGameType(WORD wTypeID)
{
	//��������
	CGameTypeItem * pGameTypeItem=NULL;
	if (m_TypeItemMap.Lookup(wTypeID,pGameTypeItem)==FALSE) return false;

	//ɾ������
	m_TypeItemMap.RemoveKey(wTypeID);
	m_TypeItemBuffer.Add(pGameTypeItem);

	return true;
}

//ɾ������
bool CServerListManager::DeleteGameKind(WORD wKindID)
{
	//��������
	CGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(wKindID,pGameKindItem)==FALSE) return false;

	//ɾ������
	m_KindItemMap.RemoveKey(wKindID);
	m_KindItemBuffer.Add(pGameKindItem);

	//���ñ���
	ZeroMemory(pGameKindItem,sizeof(CGameKindItem));

	return true;
}

//ɾ������
bool CServerListManager::DeleteGameServer(WORD wServerID)
{
	//���ҷ���
	CGameServerItem * pGameServerItem=NULL;
	if (m_ServerItemMap.Lookup(wServerID,pGameServerItem)==FALSE) return false;

	//ɾ������
	m_ServerItemMap.RemoveKey(wServerID);
	m_ServerItemBuffer.Add(pGameServerItem);

	//��������
	CGameKindItem * pGameKindItem=NULL;
	if (m_KindItemMap.Lookup(pGameServerItem->m_GameServer.wKindID,pGameKindItem)==TRUE)
	{
		tagGameServer * pGameServer=&pGameServerItem->m_GameServer;
		pGameKindItem->m_GameKind.dwOnLineCount = __max(pGameKindItem->m_GameKind.dwOnLineCount-pGameServer->dwOnLineCount,0);
	}

	return false;
}

//ö������
CGameTypeItem * CServerListManager::EmunGameTypeItem(POSITION & Position)
{
	//��������
	WORD wKey=0;
	CGameTypeItem * pGameTypeItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_TypeItemMap.GetStartPosition();
	if (Position!=NULL) m_TypeItemMap.GetNextAssoc(Position,wKey,pGameTypeItem);

	return pGameTypeItem;
}

//ö������
CGameKindItem * CServerListManager::EmunGameKindItem(POSITION & Position)
{
	//��������
	WORD wKey=0;
	CGameKindItem * pGameKindItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_KindItemMap.GetStartPosition();
	if (Position!=NULL) m_KindItemMap.GetNextAssoc(Position,wKey,pGameKindItem);

	return pGameKindItem;
}

//ö�ٷ���
CGameServerItem * CServerListManager::EmunGameServerItem(POSITION & Position)
{
	//��������
	WORD wKey=0;
	CGameServerItem * pGameServerItem=NULL;

	//��ȡ����
	if (Position==NULL) Position=m_ServerItemMap.GetStartPosition();
	if (Position!=NULL) m_ServerItemMap.GetNextAssoc(Position,wKey,pGameServerItem);

	return pGameServerItem;
}

//��������
CGameTypeItem * CServerListManager::SearchGameType(WORD wTypeID)
{
	CGameTypeItem * pGameTypeItem=NULL;
	m_TypeItemMap.Lookup(wTypeID,pGameTypeItem);
	return pGameTypeItem;
}

//��������
CGameKindItem * CServerListManager::SearchGameKind(WORD wKindID)
{
	CGameKindItem * pGameKindItem=NULL;
	m_KindItemMap.Lookup(wKindID,pGameKindItem);
	return pGameKindItem;
}

//���ҷ���
CGameServerItem * CServerListManager::SearchGameServer(WORD wServerID)
{
	CGameServerItem * pGameServerItem=NULL;
	m_ServerItemMap.Lookup(wServerID,pGameServerItem);
	return pGameServerItem;
}

//////////////////////////////////////////////////////////////////////////////////
