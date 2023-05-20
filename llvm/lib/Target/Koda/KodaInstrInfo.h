#ifndef LLVM_LIB_TARGET_Koda_KodaINSTRINFO_H
#define LLVM_LIB_TARGET_Koda_KodaINSTRINFO_H

#include "MCTargetDesc/KodaInfo.h"
#include "KodaRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "KodaGenInstrInfo.inc"

namespace llvm {

class KodaSubtarget;

class KodaInstrInfo : public KodaGenInstrInfo {
  const KodaSubtarget &STI;
  virtual void anchor();

  const MCInstrDesc &getBrCond(KodaCC::CondCode CC) const;

public:
  KodaInstrInfo(const KodaSubtarget &);

  unsigned isLoadFromStackSlot(const MachineInstr &MI,
                               int &FrameIndex) const override;
  unsigned isStoreToStackSlot(const MachineInstr &MI,
                              int &FrameIndex) const override;

  unsigned getInstSizeInBytes(const MachineInstr &MI) const override;

  bool analyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
                     MachineBasicBlock *&FBB,
                     SmallVectorImpl<MachineOperand> &Cond,
                     bool AllowModify) const override;

  unsigned insertBranch(MachineBasicBlock &MBB, MachineBasicBlock *TBB,
                        MachineBasicBlock *FBB, ArrayRef<MachineOperand> Cond,
                        const DebugLoc &,
                        int *BytesAdded = nullptr) const override;

  unsigned removeBranch(MachineBasicBlock &MBB,
                        int *BytesRemoved = nullptr) const override;

  MachineBasicBlock *getBranchDestBlock(const MachineInstr &MI) const override;

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MI, Register SrcReg,
                           bool IsKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI) const override;

  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MI, Register DestReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI) const override;

  bool
  reverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const override;

  virtual bool getBaseAndOffsetPosition(const MachineInstr &MI,
                                        unsigned &BasePos,
                                        unsigned &OffsetPos) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_Koda_KodaINSTRINFO_H