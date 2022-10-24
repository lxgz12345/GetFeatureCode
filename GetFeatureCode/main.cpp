// GetFeatureCode.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "getFeatureCode.h"

int main()
{
    std::cout << getFeatureCode::get_CPU_Name() << std::endl;
    std::cout << getFeatureCode::get_CPU_id() << std::endl;
    std::cout << getFeatureCode::get_DiskDrive_id() << std::endl;
    std::cout << getFeatureCode::get_BaseBoard_id() << std::endl;
    std::cout << getFeatureCode::get_BIOS_id() << std::endl;
    std::cout << getFeatureCode::get_CPU_SystemName() << std::endl;
    std::cout << getFeatureCode::get_NIC_WIFI_MACAddress() << std::endl;


    system("pause");
}
