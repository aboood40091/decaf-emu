#pragma once
#include "kernel/kernel_hlemodule.h"

namespace vpad
{

class Module : public kernel::HleModuleImpl<Module>
{
public:
   virtual void initialise() override;

public:
   static void RegisterFunctions();

private:
   static void registerCoreFunctions();
   static void registerStatusFunctions();
};

} // namespace vpad
