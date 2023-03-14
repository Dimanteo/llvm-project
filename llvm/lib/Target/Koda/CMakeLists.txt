add_llvm_component_group(Koda)

set(LLVM_TARGET_DEFINITIONS Koda.td)

tablegen(LLVM KodaGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM KodaGenInstrInfo.inc -gen-instr-info)

add_public_tablegen_target(KodaCommonTableGen)

add_llvm_target(KodaCodeGen
  KodaTargetMachine.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  Core
  CodeGen
  MC
  KodaDesc
  KodaInfo
  SelectionDAG
  Support
  Target
  TransformUtils
  GlobalISel

  ADD_TO_COMPONENT
  Koda
  )

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
