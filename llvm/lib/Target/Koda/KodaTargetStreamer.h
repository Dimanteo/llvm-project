#ifndef LLVM_LIB_TARGET_Koda_KodaTARGETSTREAMER_H
#define LLVM_LIB_TARGET_Koda_KodaTARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"

namespace llvm {

class KodaTargetStreamer : public MCTargetStreamer {
public:
  KodaTargetStreamer(MCStreamer &S);
  ~KodaTargetStreamer() override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_Koda_KodaTARGETSTREAMER_H