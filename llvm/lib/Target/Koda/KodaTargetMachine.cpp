//===-- KodaTargetMachine.cpp - Define TargetMachine for Koda -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Koda target spec.
//
//===----------------------------------------------------------------------===//

#include "KodaTargetMachine.h"
#include "Koda.h"
//#include "KodaTargetTransformInfo.h"
#include "TargetInfo/KodaTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetOptions.h"

#define DEBUG_TYPE "Koda"

using namespace llvm;

static Reloc::Model getRelocModel(Optional<Reloc::Model> RM) {
  return RM.getValueOr(Reloc::Static);
}

/// KodaTargetMachine ctor - Create an ILP32 Architecture model
KodaTargetMachine::KodaTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T,
                        "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-"
                        "f32:32:32-i64:32-f64:32-a:0:32-n32",
                        TT, CPU, FS, Options, getRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

KodaTargetMachine::~KodaTargetMachine() = default;

namespace {

/// Koda Code Generator Pass Configuration Options.
class KodaPassConfig : public TargetPassConfig {
public:
  KodaPassConfig(KodaTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  KodaTargetMachine &getKodaTargetMachine() const {
    return getTM<KodaTargetMachine>();
  }

  bool addInstSelector() override;
  // void addPreEmitPass() override;
  // void addPreRegAlloc() override;
};

} // end anonymous namespace

TargetPassConfig *KodaTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new KodaPassConfig(*this, PM);
}

bool KodaPassConfig::addInstSelector() {
  addPass(createKodaISelDag(getKodaTargetMachine(), getOptLevel()));
  return false;
}

// void KodaPassConfig::addPreEmitPass() { llvm_unreachable(""); }

// void KodaPassConfig::addPreRegAlloc() { llvm_unreachable(""); }

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeKodaTarget() {
  RegisterTargetMachine<KodaTargetMachine> X(getTheKodaTarget());
}

#if 0
TargetTransformInfo
KodaTargetMachine::getTargetTransformInfo(const Function &F) {
  return TargetTransformInfo(KodaTTIImpl(this, F));
}
#endif