
@echo off

set curRootPath=%cd%\..
echo %curRootPath%

echo ��ǰĿ¼
set curPath=%cd%

echo ��һ��Ŀ¼
set prevPath=%cd%\..\
echo %prevPath%

echo Դ��Դ·��
set srcClientPath=%curPath%\client
echo %srcClientPath%
set srcServerPath=%curPath%\server
echo %srcServerPath%

echo Ŀ��ͻ�����Դ·��
set dstClientDebug=%prevPath%����\ClientDebugUnicode
echo %dstClientDebug%
set dstClientRelease=%prevPath%����\ClientReleaseUnicode
echo %dstClientRelease%

mkdir %dstClientDebug%
mkdir %dstClientRelease%

echo Ŀ��������Դ·��
set dstServerDebug=%prevPath%����\ServDebugUnicode
set dstServerRelease=%prevPath%����\ServReleaseUnicode
mkdir %dstServerDebug%
mkdir %dstServerRelease%

echo �밴���������ʼ����������
pause

title ���������ļ�
color 30

xcopy %srcClientPath%\*.* %dstClientDebug%\    /s /Y /e /h
xcopy %srcClientPath%\*.* %dstClientRelease%\  /s /Y /e /h

xcopy %srcServerPath%\*.* %dstServerDebug%\    /s /Y /e /h
xcopy %srcServerPath%\*.* %dstServerRelease%\  /s /Y /e /h

pause