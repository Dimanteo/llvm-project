#ifndef LLVM_LIB_TARGET_Koda_MCTARGETDESC_MATINT_H
#define LLVM_LIB_TARGET_Koda_MCTARGETDESC_MATINT_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/SubtargetFeature.h"
#include <cstdint>

namespace llvm {
class APInt;

namespace KodaMatInt {
struct Inst {
  unsigned Opc;
  int64_t Imm;

  Inst(unsigned Opc, int64_t Imm) : Opc(Opc), Imm(Imm) {}
};
using InstSeq = SmallVector<Inst, 8>;

// Helper to generate an instruction sequence that will materialise the given
// immediate value into a register. A sequence of instructions represented by a
// simple struct is produced rather than directly emitting the instructions in
// order to allow this helper to be used from both the MC layer and during
// instruction selection.
InstSeq generateInstSeq(int64_t Val, const FeatureBitset &ActiveFeatures);

// Helper to estimate the number of instructions required to materialise the
// given immediate value into a register. This estimate does not account for
// `Val` possibly fitting into an immediate, and so may over-estimate.
//
// This will attempt to produce instructions to materialise `Val` as an
// `Size`-bit immediate.
//
// If CompressionCost is true it will use a different cost calculation if RVC is
// enabled. This should be used to compare two different sequences to determine
// which is more compressible.
int getIntMatCost(const APInt &Val, unsigned Size,
                  const FeatureBitset &ActiveFeatures,
                  bool CompressionCost = false);
} // namespace KodaMatInt
} // namespace llvm
#endif
