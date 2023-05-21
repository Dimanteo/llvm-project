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
#include "KodaMCAsmInfo.h"
#include "KodaTargetStreamer.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

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

KodaTargetStreamer::KodaTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}
KodaTargetStreamer::~KodaTargetStreamer() = default;

static MCTargetStreamer *createTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new KodaTargetStreamer(S);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeKodaTargetMC() {
  // Register the MC asm info.
  Target &TheKodaTarget = getTheKodaTarget();
  RegisterMCAsmInfoFn X(TheKodaTarget, createKodaMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheKodaTarget, createKodaMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheKodaTarget, createKodaMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheKodaTarget,
                                          createKodaMCSubtargetInfo);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(TheKodaTarget, createKodaMCInstPrinter);

  TargetRegistry::RegisterAsmTargetStreamer(TheKodaTarget,
                                            createTargetAsmStreamer);
}