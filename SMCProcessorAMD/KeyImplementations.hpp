//
//  KeyImplementations.hpp
//  SMCProcessorAMD
//

#ifndef KeyImplementations_hpp
#define KeyImplementations_hpp

#include <VirtualSMCSDK/kern_vsmcapi.hpp>
#include "SMCProcessorAMD.hpp"

class SMCProcessorAMD;


class AMDSupportVsmcValue : public VirtualSMCValue {
protected:
    SMCProcessorAMD *provider;
    size_t package;
    size_t core;
public:
    AMDSupportVsmcValue(SMCProcessorAMD *provider, size_t package, size_t core=0) : provider(provider), package(package), core(core) {}
};

// 封装温度
class TempPackage  : public AMDSupportVsmcValue { using AMDSupportVsmcValue::AMDSupportVsmcValue; protected: SMC_RESULT readAccess() override; };
// 核心温度
class TempCore     : public AMDSupportVsmcValue { using AMDSupportVsmcValue::AMDSupportVsmcValue; protected: SMC_RESULT readAccess() override; };
// 功率
class EnergyPackage: public AMDSupportVsmcValue
{ using AMDSupportVsmcValue::AMDSupportVsmcValue; protected: SMC_RESULT readAccess() override; };
// 频率
class ClockCore    : public AMDSupportVsmcValue {
    using AMDSupportVsmcValue::AMDSupportVsmcValue;
protected: SMC_RESULT readAccess() override;
};
// 电源状态？
class BSIn         : public AMDSupportVsmcValue {
    using AMDSupportVsmcValue::AMDSupportVsmcValue;
protected: SMC_RESULT update(const SMC_DATA *src) override;
};

//电池健康
class BDVT         : public AMDSupportVsmcValue {
    using AMDSupportVsmcValue::AMDSupportVsmcValue;
protected: SMC_RESULT update(const SMC_DATA *src) override;
};

#endif /* KeyImplementations_hpp */
