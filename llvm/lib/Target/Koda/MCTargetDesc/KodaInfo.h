#ifndef LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaINFO_H
#define LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaINFO_H

#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/MCRegister.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCInstrDesc.h"
#include "llvm/MC/SubtargetFeature.h"

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
  OPERAND_SIMM12,
  OPERAND_PCREL
};
} // namespace KodaOp

namespace KodaABI {
enum ABI { ABI_ILP32, ABI_Unknown };

// Returns the target ABI, or else a StringError if the requested ABIName is
// not supported for the given TT and FeatureBits combination.
ABI computeTargetABI(const Triple &TT, FeatureBitset FeatureBits,
                     StringRef ABIName);

ABI getTargetABI(StringRef ABIName);

}


namespace KodaII {

enum {
  InstFormatPseudo = 0,
  InstFormatR = 1,
  InstFormatI = 3,
  InstFormatS = 4,
  InstFormatB = 5,
  InstFormatU = 6,
  InstFormatJ = 7,
  InstFormatMask = 31,
  InstFormatShift = 0,
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

// Helper functions to read TSFlags.
/// \returns the format of the instruction.
static inline unsigned getFormat(uint64_t TSFlags) {
  return (TSFlags & InstFormatMask) >> InstFormatShift;
}

} // namespace KodaII

namespace KodaFeatures {

inline void validate(const Triple &TT, const FeatureBitset &FeatureBits) {
}

inline void toFeatureVector(std::vector<std::string> &FeatureVector,
                     const FeatureBitset &FeatureBits) {
}

}

} // end namespace llvm

#endif