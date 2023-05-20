#ifndef LLVM_LIB_TARGET_Koda_KodaTARGETMACHINE_H
#define LLVM_LIB_TARGET_Koda_KodaTARGETMACHINE_H

#include "KodaInstrInfo.h"
#include "KodaSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class KodaTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  KodaSubtarget Subtarget;
  // mutable StringMap<std::unique_ptr<KodaSubtarget>> SubtargetMap;

public:
  KodaTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    CodeGenOpt::Level OL, bool JIT);
  ~KodaTargetMachine() override;

  const KodaSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const KodaSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

#if 0
  bool
  addPassesToEmitFile(PassManagerBase &, raw_pwrite_stream &,
                      raw_pwrite_stream *, CodeGenFileType,
                      bool /*DisableVerify*/ = true,
                      MachineModuleInfoWrapperPass *MMIWP = nullptr) override {
    return false;
  }
#endif
  // TargetTransformInfo getTargetTransformInfo(const Function &F) override;
};
} // end namespace llvm

#endif