#include "getFeatureCode.h"

std::string& getFeatureCode::trim(std::string& str, const std::string& chars) {
	TRIM(str, chars);
	return str;
}

std::string& getFeatureCode::trim(char* str) {
	std::string ret = str;
	return getFeatureCode::trim(ret);
}

std::string getFeatureCode::execmd(const char* 命令, int 缓冲区大小, bool 去除第一行, bool 去除头尾空白符) {
	if (缓冲区大小 == 0) 缓冲区大小 = 1024;
	std::string 返回 = "";

	char* 缓冲区 = new char[缓冲区大小];
	FILE* pipe = _popen(命令, "r");
	if (!pipe) return 返回;
	while (!feof(pipe)) {
		if (fgets(缓冲区, 缓冲区大小, pipe)) {
			if (去除第一行) {
				去除第一行 = false;
				continue;
			}
			返回 += 缓冲区;
		}
	}
	_pclose(pipe);
	if (去除头尾空白符) 返回 = getFeatureCode::trim(返回);
	return 返回;
}

std::string getFeatureCode::get_CPU_Name() {
	return execmd("wmic cpu get Name");
}

std::string getFeatureCode::get_CPU_id() {
	return execmd("wmic cpu get processorid");
}

std::string getFeatureCode::get_DiskDrive_id() {
	return execmd("wmic diskdrive where index=0 get serialnumber");
}

std::string getFeatureCode::get_BaseBoard_id() {
	return execmd("wmic baseboard get serialnumber");
}

std::string getFeatureCode::get_BIOS_id() {
	return execmd("wmic bios get serialnumber");
}

std::string getFeatureCode::get_CPU_SystemName() {
	return execmd("wmic cpu get SystemName");
}

std::string getFeatureCode::get_NIC_WIFI_MACAddress() {
	return execmd("wmic nic where \"Name like 'Microsoft Wi-Fi Direct Virtual Adapter'\" get MACAddress");
}