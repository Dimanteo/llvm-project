//===-- KodaMCTargetDesc.cpp - Koda Target Descriptions -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Koda specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "KodaMCTargetDesc.h"
#include "TargetInfo/KodaTargetInfo.h"
#include "KodaInfo.h"
#include "KodaInstPrinter.h"
#include "KodaELFStreamer.h"
#include "KodaMCObjectFIleInfo.h"
#include "KodaMCAsmInfo.h"
#include "KodaTargetStreamer.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "KodaGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "KodaGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "KodaGenSubtargetInfo.inc"

static MCInstrInfo *createKodaMCInstrInfo() {
  auto *X = new MCInstrInfo();
  InitKodaMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createKodaMCRegisterInfo(const Triple &TT) {
  auto *X = new MCRegisterInfo();
  InitKodaMCRegisterInfo(X, Koda::X1);
  return X;
}

static MCSubtargetInfo *createKodaMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  return createKodaMCSubtargetInfoImpl(TT, CPU, /*TuneCPU=*/CPU, FS);
}

static MCAsmInfo *createKodaMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new KodaMCAsmInfo(TT);
  MCRegister SP = MRI.getDwarfRegNum(Koda::X2, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createKodaMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new KodaInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *createKodaTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new KodaTargetStreamer(S);
}

static MCObjectFileInfo *
createKodaMCObjectFileInfo(MCContext &Ctx, bool PIC,
                            bool LargeCodeModel = false) {
  MCObjectFileInfo *MOFI = new KodaMCObjectFileInfo();
  MOFI->initMCObjectFileInfo(Ctx, PIC, LargeCodeModel);
  return MOFI;
}

static MCTargetStreamer *
createKodaObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI) {
  const Triple &TT = STI.getTargetTriple();
  if (TT.isOSBinFormatELF())
    return new KodaTargetELFStreamer(S, STI);
  return nullptr;
}

class KodaMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit KodaMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    if (isConditionalBranch(Inst)) {
      int64_t Imm;
      if (Size == 2)
        Imm = Inst.getOperand(1).getImm();
      else
        Imm = Inst.getOperand(2).getImm();
      Target = Addr + Imm;
      return true;
    }

    if (Inst.getOpcode() == Koda::JAL) {
      Target = Addr + Inst.getOperand(1).getImm();
      return true;
    }

    return false;
  }
};

static MCInstrAnalysis *createKodaInstrAnalysis(const MCInstrInfo *Info) {
  return new KodaMCInstrAnalysis(Info);
}

static MCTargetStreamer *createKodaNullTargetStreamer(MCStreamer &S) {
  return new KodaTargetStreamer(S);
}

namespace {
MCStreamer *createKodaELFStreamer(const Triple &T, MCContext &Context,
                                   std::unique_ptr<MCAsmBackend> &&MAB,
                                   std::unique_ptr<MCObjectWriter> &&MOW,
                                   std::unique_ptr<MCCodeEmitter> &&MCE,
                                   bool RelaxAll) {
  return createKodaELFStreamer(Context, std::move(MAB), std::move(MOW),
                                std::move(MCE), RelaxAll);
}
} // end anonymous

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeKodaTargetMC() {
  // Register the MC asm info.
  Target &TheKodaTarget = getTheKodaTarget();
  RegisterMCAsmInfoFn X(TheKodaTarget, createKodaMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCObjectFileInfo(TheKodaTarget, createKodaMCObjectFileInfo);
  TargetRegistry::RegisterMCInstrInfo(TheKodaTarget, createKodaMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheKodaTarget, createKodaMCRegisterInfo);
  TargetRegistry::RegisterMCAsmBackend(TheKodaTarget, createKodaAsmBackend);
  TargetRegistry::RegisterMCCodeEmitter(TheKodaTarget, createKodaMCCodeEmitter);
  TargetRegistry::RegisterMCInstPrinter(TheKodaTarget, createKodaMCInstPrinter);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheKodaTarget,
                                          createKodaMCSubtargetInfo);

  TargetRegistry::RegisterELFStreamer(TheKodaTarget, createKodaELFStreamer);
  TargetRegistry::RegisterObjectTargetStreamer(
        TheKodaTarget, createKodaObjectTargetStreamer);
  TargetRegistry::RegisterMCInstrAnalysis(TheKodaTarget, createKodaInstrAnalysis);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(TheKodaTarget, createKodaMCInstPrinter);

  TargetRegistry::RegisterAsmTargetStreamer(TheKodaTarget,
                                            createKodaTargetAsmStreamer);

  TargetRegistry::RegisterNullTargetStreamer(TheKodaTarget,
                                               createKodaNullTargetStreamer);
}