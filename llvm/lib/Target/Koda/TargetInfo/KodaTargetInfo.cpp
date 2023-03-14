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
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target llvm::TheKodaTarget;

extern "C" void LLVMInitializeKodaTargetInfo() {
  RegisterTarget<Triple::koda,
                 /*HasJIT=*/false>
      X(TheKodaTarget, "Koda", "Koda (32-bit RISC-V arch)", "Koda");
}