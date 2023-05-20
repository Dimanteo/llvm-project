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
class KodaSubtarget;
class AsmPrinter;
class InstructionSelector;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class PassRegistry;

bool lowerKodaMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                    AsmPrinter &AP);
bool LowerKodaMachineOperandToMCOperand(const MachineOperand &MO,
                                         MCOperand &MCOp, const AsmPrinter &AP);

FunctionPass *createKodaISelDag(KodaTargetMachine &TM,
                                CodeGenOpt::Level OptLevel);


namespace Koda {
enum {
  GP = Koda::X3,
  RA = Koda::X1,
  SP = Koda::X2,
  FP = Koda::X8
};
} // namespace Koda

} // namespace llvm

#endif