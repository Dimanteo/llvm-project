add_llvm_component_group(Koda)

set(LLVM_TARGET_DEFINITIONS Koda.td)

# Generate KodaGenRegisterInfo.inc and KodaGenInstrInfo.inc which included by
#  your hand code C++ files.
# KodaGenRegisterInfo.inc came from KodaRegisterInfo.td, KodaGenInstrInfo.inc
#  came from KodaInstrInfo.td.
tablegen(LLVM KodaGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM KodaGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM KodaGenCallingConv.inc -gen-callingconv)
tablegen(LLVM KodaGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM KodaGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM KodaGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM KodaGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM KodaGenMCCodeEmitter.inc -gen-emitter)
#tablegen(LLVM KodaGenDisassemblerTables.inc -gen-disassembler)

add_public_tablegen_target(KodaCommonTableGen)

add_llvm_target(KodaCodeGen
  KodaTargetMachine.cpp
  KodaAsmPrinter.cpp
  KodaFrameLowering.cpp
  KodaISelDagToDag.cpp
  KodaISelLowering.cpp
  KodaInstrInfo.cpp
  KodaMCInstLower.cpp
  KodaMachineFunctionInfo.cpp
  KodaRegisterInfo.cpp
  KodaSubtarget.cpp

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
