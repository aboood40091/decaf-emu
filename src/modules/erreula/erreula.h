#pragma once
#include "kernel/kernel_hlemodule.h"

namespace nn
{

namespace erreula
{

class Module : public kernel::HleModuleImpl<Module>
{
public:
   virtual void initialise() override;

public:
   static void RegisterFunctions();

private:
   static void registerErrorViewerFunctions();
};

} // namespace erreula

} // namespace nn
