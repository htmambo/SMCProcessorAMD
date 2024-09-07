//
//  SMCProcessorAMD
//

#include "KeyImplementations.hpp"

// 温度
SMC_RESULT TempPackage::readAccess() {
    uint16_t *ptr = reinterpret_cast<uint16_t *>(data);
    *ptr = VirtualSMCAPI::encodeSp(type, (double)provider->PACKAGE_TEMPERATURE_perPackage[0]);

    return SmcSuccess;
}

// 温度
SMC_RESULT TempCore::readAccess() {
    uint16_t *ptr = reinterpret_cast<uint16_t *>(data);
    *ptr = VirtualSMCAPI::encodeSp(type, (double)provider->PACKAGE_TEMPERATURE_perPackage[0]);

    return SmcSuccess;
}

// 频率
SMC_RESULT ClockCore::readAccess() {
    uint16_t *ptr = reinterpret_cast<uint16_t *>(data);
    *ptr = VirtualSMCAPI::encodeSp(type, (double)provider->MSR_HARDWARE_PSTATE_STATUS_perCore[core]);

    return SmcSuccess;
}

// 功率
SMC_RESULT EnergyPackage::readAccess(){
    if (type == SmcKeyTypeFloat)
        *reinterpret_cast<uint32_t *>(data) = VirtualSMCAPI::encodeFlt(provider->uniPackageEnergy);
    else
        *reinterpret_cast<uint16_t *>(data) = VirtualSMCAPI::encodeSp(type, provider->uniPackageEnergy);
    
    return SmcSuccess;
}

// 电源状态（获取不到状态变化......）
SMC_RESULT BSIn::update(const SMC_DATA *src)  {
    SYSLOG("SMCProcessorAMD", "BSIn update 0x%x -> 0x%x\n", src[0], src[1]);
    uint state = 0;
    lilu_os_memcpy(&state, src, size);
//    let batteryInfoData = try readData(batteryInfoKey);
//    let isCharging = batteryInfoData.0 & 1 == 1 ? true : false;
//    let isACPresent = (batteryInfoData.0 >> 1) & 1 == 1 ? true : false;
//    let isBatteryOk = (batteryInfoData.0 >> 6) & 1 == 1 ? true : false;
    if(data[0] == src[0]) {
        IOLog("BSIn does not need to be updated(0x%x, 0x%x)\n", data[0], src[1]);
        return SmcSuccess;
    }
    SYSLOG("SMCProcessorAMD", "BSIn update 0x%x -> 0x%x\n", src[0], src[1]);
    IOLog("BSIn update 0x%x -> 0x%x\n", src[0], src[1]);

    lilu_os_memcpy(data, src, size);
    auto isACPresent = ((src[0] >> 1) & 1) == 1;
    provider->setCPBState(isACPresent);
    return SmcSuccess;
}

// 电池健康->管理电池寿命
SMC_RESULT BDVT::update(const SMC_DATA *src)  {
    bool state = false;
    lilu_os_memcpy(&state, src, size);
    SYSLOG("BDVT", "update 0x%x -> 0x%x\n", data[0], src[0]);
    if(data[0] == src[0]) {
        SYSLOG("BDVT", "state is not changed");
        return SmcSuccess;
    }
    auto isACPresent = src[0] == 1;
    provider->setCPBState(isACPresent);

    lilu_os_memcpy(data, src, size);
    return SmcSuccess;
}
