#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0501 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0501	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "..\��Ϣ����\CMD_Baccarat.h"


//////////////////////////////////////////////////////////////////////////
//���Ӵ���

#pragma comment( lib,"strsafe.lib" )


//////////////////////////////////////////////////////////////////////////////////

//���뻷��
#include "..\..\SDKCondition.h"


//////////////////////////////////////////////////////////////////////////////////

#ifdef SDK_CONDITION

//////////////////////////////////////////////////////////////////////////////////
//��������

#include "..\..\..\������\Include\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/WHImage.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/ServiceCore.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/ShareControl.lib")
#else
#pragma comment (lib,"../../../������/Lib/Unicode/WHImage.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/ServiceCore.lib")
#pragma comment (lib,"../../../������/Lib/Unicode/ShareControl.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../������/Lib/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/ServiceCoreD.lib")
#pragma comment (lib,"../../../������/Lib/Ansi/ShareControlD.lib")
#else
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../../ϵͳģ��/���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////////////
//ϵͳ����

#include "..\..\..\�ͻ������\��Ϸ���\GameFrameHead.h"

#ifndef _DEBUG
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/WHImage.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrame.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngine.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControl.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCore.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/WHImage.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrame.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngine.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControl.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCore.lib")
#endif
#else
#ifndef _UNICODE
#pragma comment (lib,"../../../���ӿ�/Ansi/WHImageD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameFrameD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/GameEngineD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/SkinControlD.lib")
#pragma comment (lib,"../../../���ӿ�/Ansi/ServiceCoreD.lib")
#else
#pragma comment (lib,"../../../���ӿ�/Unicode/WHImageD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameFrameD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/GameEngineD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/SkinControlD.lib")
#pragma comment (lib,"../../../���ӿ�/Unicode/ServiceCoreD.lib")
#endif
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif


//////////////////////////////////////////////////////////////////////////
