#ifndef LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCASMINFO_H
#define LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class KodaMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit KodaMCAsmInfo(const Triple &TT);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCASMINFO_H