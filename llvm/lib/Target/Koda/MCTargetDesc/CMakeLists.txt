add_llvm_component_library(LLVMKodaDesc
  KodaInstPrinter.cpp
  KodaMCAsmInfo.cpp
  KodaMCTargetDesc.cpp
  KodaMatInt.cpp
  KodaInfo.cpp
  KodaAsmBackend.cpp
  KodaMCCodeEmitter.cpp
  KodaMCExpr.cpp
  KodaELFObjectWriter.cpp
  KodaTargetStreamer.cpp
  KodaELFStreamer.cpp
  KodaMCObjectFIleInfo.cpp

  LINK_COMPONENTS
  MC
  KodaInfo
  Support

  ADD_TO_COMPONENT
  Koda
  )