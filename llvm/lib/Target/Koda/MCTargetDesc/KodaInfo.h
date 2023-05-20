#ifndef LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaINFO_H
#define LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaINFO_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace KodaCC {
enum CondCode {
  EQ,
  NE,
  LT,
  GE,
  LTU,
  GEU,
  INVALID,
};

CondCode getOppositeBranchCondition(CondCode);

enum BRCondCode {
  BREQ = 0x0,
};
} // end namespace KodaCC

namespace KodaOp {
enum OperandType : unsigned {
  OPERAND_SIMMM16 = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_UIMM16,
};
} // namespace KodaOp

} // end namespace llvm

#endif