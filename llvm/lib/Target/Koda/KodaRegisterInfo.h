#ifndef LLVM_LIB_TARGET_Koda_KodaREGISTERINFO_H
#define LLVM_LIB_TARGET_Koda_KodaREGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "KodaGenRegisterInfo.inc"

namespace llvm {

class TargetInstrInfo;
class KodaSubtarget;

struct KodaRegisterInfo : public KodaGenRegisterInfo {
public:
  KodaRegisterInfo();

  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Debug information queries.
  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_Koda_KodaREGISTERINFO_H