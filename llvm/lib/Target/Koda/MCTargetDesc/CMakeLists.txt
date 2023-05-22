add_llvm_component_library(LLVMKodaDesc
  KodaInstPrinter.cpp
  KodaMCAsmInfo.cpp
  KodaMCTargetDesc.cpp
  KodaMatInt.cpp

  LINK_COMPONENTS
  MC
  KodaInfo
  Support

  ADD_TO_COMPONENT
  Koda
  )