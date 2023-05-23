#ifndef __LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaFIXUPKINDS_H__
#define __LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaFIXUPKINDS_H__
#include "llvm/MC/MCFixup.h"

#undef Koda

namespace llvm {
namespace Koda {
enum Fixups {
  // 20-bit fixup corresponding to %hi(foo) for instructions like lui
  fixup_Koda_hi20 = FirstTargetFixupKind,
  // 12-bit fixup corresponding to %lo(foo) for instructions like addi
  fixup_Koda_lo12_i,
  // 12-bit fixup corresponding to %lo(foo) for the S-type store instructions
  fixup_Koda_lo12_s,
  // 20-bit fixup corresponding to %pcrel_hi(foo) for instructions like auipc
  fixup_Koda_pcrel_hi20,
  // 12-bit fixup corresponding to %pcrel_lo(foo) for instructions like addi
  fixup_Koda_pcrel_lo12_i,
  // 12-bit fixup corresponding to %pcrel_lo(foo) for the S-type store
  // instructions
  fixup_Koda_pcrel_lo12_s,
  // 20-bit fixup corresponding to %got_pcrel_hi(foo) for instructions like
  // auipc
  fixup_Koda_got_hi20,
  // 20-bit fixup corresponding to %tprel_hi(foo) for instructions like lui
  fixup_Koda_tprel_hi20,
  // 12-bit fixup corresponding to %tprel_lo(foo) for instructions like addi
  fixup_Koda_tprel_lo12_i,
  // 12-bit fixup corresponding to %tprel_lo(foo) for the S-type store
  // instructions
  fixup_Koda_tprel_lo12_s,
  // Fixup corresponding to %tprel_add(foo) for PseudoAddTPRel, used as a linker
  // hint
  fixup_Koda_tprel_add,
  // 20-bit fixup corresponding to %tls_ie_pcrel_hi(foo) for instructions like
  // auipc
  fixup_Koda_tls_got_hi20,
  // 20-bit fixup corresponding to %tls_gd_pcrel_hi(foo) for instructions like
  // auipc
  fixup_Koda_tls_gd_hi20,
  // 20-bit fixup for symbol references in the jal instruction
  fixup_Koda_jal,
  // 12-bit fixup for symbol references in the branch instructions
  fixup_Koda_branch,
  // 11-bit fixup for symbol references in the compressed jump instruction
  fixup_Koda_rvc_jump,
  // 8-bit fixup for symbol references in the compressed branch instruction
  fixup_Koda_rvc_branch,
  // Fixup representing a legacy no-pic function call attached to the auipc
  // instruction in a pair composed of adjacent auipc+jalr instructions.
  fixup_Koda_call,
  // Fixup representing a function call attached to the auipc instruction in a
  // pair composed of adjacent auipc+jalr instructions.
  fixup_Koda_call_plt,
  // Used to generate an R_Koda_RELAX relocation, which indicates the linker
  // may relax the instruction pair.
  fixup_Koda_relax,
  // Used to generate an R_Koda_ALIGN relocation, which indicates the linker
  // should fixup the alignment after linker relaxation.
  fixup_Koda_align,
  // 8-bit fixup corresponding to R_Koda_SET8 for local label assignment.
  fixup_Koda_set_8,
  // 8-bit fixup corresponding to R_Koda_ADD8 for 8-bit symbolic difference
  // paired relocations.
  fixup_Koda_add_8,
  // 8-bit fixup corresponding to R_Koda_SUB8 for 8-bit symbolic difference
  // paired relocations.
  fixup_Koda_sub_8,
  // 16-bit fixup corresponding to R_Koda_SET16 for local label assignment.
  fixup_Koda_set_16,
  // 16-bit fixup corresponding to R_Koda_ADD16 for 16-bit symbolic difference
  // paired reloctions.
  fixup_Koda_add_16,
  // 16-bit fixup corresponding to R_Koda_SUB16 for 16-bit symbolic difference
  // paired reloctions.
  fixup_Koda_sub_16,
  // 32-bit fixup corresponding to R_Koda_SET32 for local label assignment.
  fixup_Koda_set_32,
  // 32-bit fixup corresponding to R_Koda_ADD32 for 32-bit symbolic difference
  // paired relocations.
  fixup_Koda_add_32,
  // 32-bit fixup corresponding to R_Koda_SUB32 for 32-bit symbolic difference
  // paired relocations.
  fixup_Koda_sub_32,
  // 64-bit fixup corresponding to R_Koda_ADD64 for 64-bit symbolic difference
  // paired relocations.
  fixup_Koda_add_64,
  // 64-bit fixup corresponding to R_Koda_SUB64 for 64-bit symbolic difference
  // paired relocations.
  fixup_Koda_sub_64,
  // 6-bit fixup corresponding to R_Koda_SET6 for local label assignment in
  // DWARF CFA.
  fixup_Koda_set_6b,
  // 6-bit fixup corresponding to R_Koda_SUB6 for local label assignment in
  // DWARF CFA.
  fixup_Koda_sub_6b,

  // Used as a sentinel, must be the last
  fixup_Koda_invalid,
  NumTargetFixupKinds = fixup_Koda_invalid - FirstTargetFixupKind
};
} // namespace Koda
} // end namespace llvm

#endif // __LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaFIXUPKINDS_H__