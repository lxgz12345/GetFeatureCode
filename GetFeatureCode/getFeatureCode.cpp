#include "getFeatureCode.h"

std::string& getFeatureCode::trim(std::string& str, const std::string& chars) {
	TRIM(str, chars);
	return str;
}

std::string& getFeatureCode::trim(char* str) {
	std::string ret = str;
	return getFeatureCode::trim(ret);
}

std::string& getFeatureCode::处理文本(std::string &str,bool 去除第一行, bool 去除头尾空白符) {
	if (去除第一行) str = str.substr(str.find_first_of('\n') + 1);
	if (去除头尾空白符) str = getFeatureCode::trim(str);
	return str;
}

//会有黑窗口出现
std::string getFeatureCode::execmd(const char* 命令, int 缓冲区大小, bool 去除第一行, bool 去除头尾空白符) {
	if (缓冲区大小 == 0) 缓冲区大小 = 1024;
	std::string 返回 = "";

	char* 缓冲区 = new char[缓冲区大小];
	FILE* pipe = _popen(命令, "r");
	if (!pipe) return 返回;
	while (!feof(pipe)) {
		if (fgets(缓冲区, 缓冲区大小, pipe)) {
			返回 += 缓冲区;
		}
	}
	_pclose(pipe);
	return getFeatureCode::处理文本(返回, 去除第一行, 去除头尾空白符);
}

LPWSTR getFeatureCode::ConvertCharToLPWSTR(const char* szString) {
	int dwLen = (int)strlen(szString) + 1;
	int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);//算出合适的长度
	LPWSTR lpszPath = new WCHAR[dwLen];
	MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);
	return lpszPath;
}

std::string getFeatureCode::runCmd(std::string& strCmd, bool 去除第一行, bool 去除头尾空白符) {
	SECURITY_ATTRIBUTES sa{};
	HANDLE hRead = nullptr, hWrite = nullptr;
	std::string strResult;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	strCmd = "cmd /c " + strCmd;//加上"cmd /c"是为了能执行类似dir的命令,执行字符串指定的命令然后终断

	//创建匿名管道，导向DOS输入输出通道
	if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
		return NULL;
	}
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;//数据输出用的文件句柄
	si.hStdOutput = hWrite;//新创建进程的标准输出连在写管道一端
	si.wShowWindow = SW_HIDE;//隐藏窗口
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	LPWSTR cmd = getFeatureCode::ConvertCharToLPWSTR(strCmd.c_str());
	//执行cmd命令,并在命名中管道中写入cmd命令返回的串
	if (!CreateProcess(NULL, cmd, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
		CloseHandle(pi.hThread);
		return NULL;
	}
	
	//这里决定是否要等整个命令执行完,如ping等耗时较多的命令.
	if (pi.hProcess != NULL) {
		if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
			return FALSE;
		}
	}

	CloseHandle(hWrite);//关闭管道句柄

	char buffer[4096] = { 0 };
	DWORD bytesRead;

	while (ReadFile(hRead, buffer, 4096, &bytesRead, NULL))//从匿名管道中读取数据
	{
		strResult += buffer;
		memset(&buffer, 0, 4096);
		Sleep(200);//防止乱码
	}
	CloseHandle(hRead);
	return getFeatureCode::处理文本(strResult, 去除第一行, 去除头尾空白符);
}

std::string getFeatureCode::runCmd(const char* strCmd, bool 去除第一行, bool 去除头尾空白符) {
	std::string str = strCmd;
	return getFeatureCode::runCmd(str, 去除第一行, 去除头尾空白符);
}

std::string getFeatureCode::get_CPU_Name() {
	return getFeatureCode::runCmd("wmic cpu get Name");
}

std::string getFeatureCode::get_CPU_id() {
	return getFeatureCode::runCmd("wmic cpu get processorid");
}

std::string getFeatureCode::get_DiskDrive_id() {
	return getFeatureCode::runCmd("wmic diskdrive where index=0 get serialnumber");
}

std::string getFeatureCode::get_BaseBoard_id() {
	return getFeatureCode::runCmd("wmic baseboard get serialnumber");
}

std::string getFeatureCode::get_BIOS_id() {
	return getFeatureCode::runCmd("wmic bios get serialnumber");
}

std::string getFeatureCode::get_CPU_SystemName() {
	return getFeatureCode::runCmd("wmic cpu get SystemName");
}

std::string getFeatureCode::get_NIC_WIFI_MACAddress() {
	return getFeatureCode::runCmd("wmic nic where \"Name like 'Microsoft Wi-Fi Direct Virtual Adapter'\" get MACAddress");
}