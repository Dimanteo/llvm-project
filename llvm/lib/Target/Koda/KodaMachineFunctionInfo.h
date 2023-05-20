//===----------------------------------------------------------------------===//
//
// This file declares Koda-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Koda_KodaMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_Koda_KodaMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"
#include <vector>

namespace llvm {

/// KodaFunctionInfo - This class is derived from MachineFunction private
/// Koda target-specific information for each MachineFunction.
class KodaFunctionInfo : public MachineFunctionInfo {
  virtual void anchor();

  bool ReturnStackOffsetSet = false;
  unsigned ReturnStackOffset = -1U;

  /// FrameIndex for start of varargs area
  int VarArgsFrameIndex = 0;
  /// Size of the save area used for varargs
  int VarArgsSaveSize = 0;
  /// Size of stack frame to save callee saved registers
  unsigned CalleeSavedStackSize = 0;

public:
  KodaFunctionInfo() {}
  explicit KodaFunctionInfo(MachineFunction &MF) {}
  ~KodaFunctionInfo() {}

  void setVarArgsFrameIndex(int Off) { VarArgsFrameIndex = Off; }
  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }

  void setVarArgsSaveSize(int Size) { VarArgsSaveSize = Size; }
  int getVarArgsSaveSize() const { return VarArgsSaveSize; }

  unsigned getCalleeSavedStackSize() const { return CalleeSavedStackSize; }
  void setCalleeSavedStackSize(unsigned Size) { CalleeSavedStackSize = Size; }

  void setReturnStackOffset(unsigned Off) {
    assert(!ReturnStackOffsetSet && "Return stack offset set twice");
    ReturnStackOffset = Off;
    ReturnStackOffsetSet = true;
  }

  unsigned getReturnStackOffset() const {
    assert(ReturnStackOffsetSet && "Return stack offset not set");
    return ReturnStackOffset;
  }

  // unsigned MaxCallStackReq = 0;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_Koda_KodaMACHINEFUNCTIONINFO_H