#pragma once


//������
class CSendCard
{
public:
	CSendCard(void);
	~CSendCard(void);

public:
	//����·��
	void SetFilePath(CString strFilePath);
	//��������������
	int GetCardData(int iChairID, BYTE* pCard, int iNums);
	//�������
	BYTE GetKingCard();
	//���ɫ����
	bool GetSZNums(int& iNum1, int& iNum2);
	//����´�ץ����
	BYTE GetNextCard();
	//�����ǽ����
	int GetCardWallData(BYTE* pCard, int iNums);

private:
	BYTE GetNum(char c);

private:
	CString			m_strFilePath;
};
