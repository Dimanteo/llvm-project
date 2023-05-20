#include "KodaRegisterInfo.h"
#include "Koda.h"
#include "KodaInstrInfo.h"
//#include "KodaMachineFunctionInfo.h"
#include "KodaSubtarget.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "Koda-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "KodaGenRegisterInfo.inc"

KodaRegisterInfo::KodaRegisterInfo() : KodaGenRegisterInfo(Koda::X0) {}

#if 0
bool KodaRegisterInfo::needsFrameMoves(const MachineFunction &MF) {
  return MF.needsFrameMoves();
}
#endif

const MCPhysReg *
KodaRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_Koda_SaveList;
}

// TODO: check cconv
BitVector KodaRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(Koda::X18);
  Reserved.set(Koda::X19);
  Reserved.set(Koda::X20);
  Reserved.set(Koda::X21);
  Reserved.set(Koda::X22);
  Reserved.set(Koda::X23);
  Reserved.set(Koda::X24);
  Reserved.set(Koda::X25);
  Reserved.set(Koda::X26);
  Reserved.set(Koda::X27);
  return Reserved;
}

bool KodaRegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return false; // TODO: what for?
}

#if 0
bool KodaRegisterInfo::useFPForScavengingIndex(
    const MachineFunction &MF) const {
  llvm_unreachable("");
}
#endif

// TODO: rewrite!
void KodaRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                           int SPAdj, unsigned FIOperandNum,
                                           RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  int Offset = getFrameLowering(MF)
                   ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                   .getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (!isInt<16>(Offset)) {
    llvm_unreachable("");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false, false, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
}

Register KodaRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? Koda::FP : Koda::SP;
}

const uint32_t *
KodaRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const {
  return CSR_Koda_RegMask;
}