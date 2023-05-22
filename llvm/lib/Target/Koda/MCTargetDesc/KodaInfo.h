#ifndef LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaINFO_H
#define LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaINFO_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace KodaCC {
enum CondCode {
  COND_EQ,
  COND_NE,
  COND_LT,
  COND_GE,
  COND_LTU,
  COND_GEU,
  COND_INVALID,
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
  OPERAND_UIMM20,
  OPERAND_UIMMLOG2XLEN,
  OPERAND_SIMM12
};
} // namespace KodaOp

namespace KodaABI {
enum ABI { ABI_ILP32, ABI_Unknown };
}


namespace KodaII {

enum {
  InstFormatPseudo = 0,
  InstFormatR = 1,
  InstFormatR4 = 2,
  InstFormatI = 3,
  InstFormatS = 4,
  InstFormatB = 5,
  InstFormatU = 6,
  InstFormatJ = 7,
};

enum {
  MO_None = 0,
  MO_CALL = 1,
  MO_PLT = 2,
  MO_LO = 3,
  MO_HI = 4,
  MO_PCREL_LO = 5,
  MO_PCREL_HI = 6,
  MO_GOT_HI = 7,
  MO_TPREL_LO = 8,
  MO_TPREL_HI = 9,
  MO_TPREL_ADD = 10,
  MO_TLS_GOT_HI = 11,
  MO_TLS_GD_HI = 12,

  // Used to differentiate between target-specific "direct" flags and "bitmask"
  // flags. A machine operand can only have one "direct" flag, but can have
  // multiple "bitmask" flags.
  MO_DIRECT_FLAG_MASK = 15
};

} // namespace KodaII

} // end namespace llvm

#endif