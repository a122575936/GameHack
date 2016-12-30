#include "stdafx.h"
#include "GamePlayer.h"
#include <assert.h>
#include "ProcessHelper.h"
#include "ConfigFile.h"

extern TCHAR g_configPath[MAX_PATH];
CGamePlayer::CGamePlayer(CProcessHelper* pProHlp, CConfigFile* pCfgHlp)
{
    m_pConfigFile = pCfgHlp;
    m_pProHelper = pProHlp;
}


CGamePlayer::~CGamePlayer(void)
{
    m_pConfigFile = NULL;
    m_pProHelper = NULL;
}

DWORD CGamePlayer::GetGameBase() const
{
    DWORD dwGameBase = m_pConfigFile->GetGameBase() + m_pConfigFile->GetGameModule();
    DWORD dwBuf = 0;
    m_pProHelper->ReadMemory((LPCVOID)dwGameBase, &dwBuf, sizeof(dwBuf));
    return dwBuf;
}

CString CGamePlayer::GetCurAxis() const
{
    DWORD dwAxisX = m_pConfigFile->GetAxisX() + GetGameBase();
    DWORD dwBufX = 0;
    m_pProHelper->ReadMemory((LPCVOID)dwAxisX, &dwBufX, sizeof(dwBufX));

    DWORD dwAxisY = m_pConfigFile->GetAxisY() + GetGameBase();
    DWORD dwBufY = 0;
    m_pProHelper->ReadMemory((LPCVOID)dwAxisY, &dwBufY, sizeof(dwBufY));

    CString sRet;
    sRet.Format(_T("��ǰ���꣺%u��%u"), dwBufX, dwBufY);
    return sRet;
}

CString CGamePlayer::GetPlayerName() const
{
    //DWORD dwAddr = m_pConfigFile->GetPlayerName() + m_pConfigFile->GetGameModule();
	DWORD dwAddr = m_pConfigFile->GetPlayerName() + GetGameBase();
	m_pProHelper->ReadMemory((LPCVOID)dwAddr, &dwAddr, sizeof(dwAddr));
    BYTE sName[20] = {0};
    m_pProHelper->ReadMemory((LPCVOID)dwAddr, sName, sizeof(sName));
    CString sPlayerName;
	sPlayerName.Format(_T("�������ƣ�%s"), CStringW(sName));
    return sPlayerName;
}
