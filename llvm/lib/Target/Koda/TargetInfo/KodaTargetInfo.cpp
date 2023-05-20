//===-- KodaTargetInfo.cpp - Koda Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Koda.h"
#include "llvm/IR/Module.h"
#include "TargetInfo/KodaTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheKodaTarget() {
  static Target TheKodaTarget;
  return TheKodaTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeKodaTargetInfo() {
  RegisterTarget<Triple::Koda, false> X(getTheKodaTarget(), "Koda", "Koda (32-bit RISC-V arch)", "Koda");
}