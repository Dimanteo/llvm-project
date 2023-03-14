//===-- Koda.h - Top-level interface for Koda representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM Koda back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Koda_Koda_H
#define LLVM_LIB_TARGET_Koda_Koda_H

#include "MCTargetDesc/KodaMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class KodaTargetMachine;
  class FunctionPass;

} // end namespace llvm;

#endif