#ifndef __LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCEXPR_H__
#define __LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCEXPR_H__

#include "llvm/MC/MCExpr.h"

namespace llvm {

class StringRef;

class KodaMCExpr : public MCTargetExpr {
public:
  enum VariantKind {
    VK_Koda_None,
    VK_Koda_LO,
    VK_Koda_HI,
    VK_Koda_PCREL_LO,
    VK_Koda_PCREL_HI,
    VK_Koda_GOT_HI,
    VK_Koda_TPREL_LO,
    VK_Koda_TPREL_HI,
    VK_Koda_TPREL_ADD,
    VK_Koda_TLS_GOT_HI,
    VK_Koda_TLS_GD_HI,
    VK_Koda_CALL,
    VK_Koda_CALL_PLT,
    VK_Koda_32_PCREL,
    VK_Koda_Invalid // Must be the last item
  };

private:
  const MCExpr *Expr;
  const VariantKind Kind;

  int64_t evaluateAsInt64(int64_t Value) const;

  explicit KodaMCExpr(const MCExpr *Expr, VariantKind Kind)
      : Expr(Expr), Kind(Kind) {}

public:
  static const KodaMCExpr *create(const MCExpr *Expr, VariantKind Kind,
                                 MCContext &Ctx);

  VariantKind getKind() const { return Kind; }

  const MCExpr *getSubExpr() const { return Expr; }

  /// Get the corresponding PC-relative HI fixup that a VK_Koda_PCREL_LO
  /// points to, and optionally the fragment containing it.
  ///
  /// \returns nullptr if this isn't a VK_Koda_PCREL_LO pointing to a
  /// known PC-relative HI fixup.
  const MCFixup *getPCRelHiFixup(const MCFragment **DFOut) const;

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override {
    return getSubExpr()->findAssociatedFragment();
  }

  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override;

  bool evaluateAsConstant(int64_t &Res) const;

  static bool classof(const MCExpr *E) {
    return E->getKind() == MCExpr::Target;
  }

  static bool classof(const KodaMCExpr *) { return true; }

  static VariantKind getVariantKindForName(StringRef name);
  static StringRef getVariantKindName(VariantKind Kind);
};

} // end namespace llvm.

#endif // __LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCEXPR_H__