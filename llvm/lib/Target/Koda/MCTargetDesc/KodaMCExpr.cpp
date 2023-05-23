#include "KodaMCExpr.h"
#include "MCTargetDesc/KodaAsmBackend.h"
#include "KodaFixupKinds.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbolELF.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "Kodamcexpr"

const KodaMCExpr *KodaMCExpr::create(const MCExpr *Expr, VariantKind Kind,
                                       MCContext &Ctx) {
  return new (Ctx) KodaMCExpr(Expr, Kind);
}

void KodaMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  VariantKind Kind = getKind();
  bool HasVariant = ((Kind != VK_Koda_None) && (Kind != VK_Koda_CALL) &&
                     (Kind != VK_Koda_CALL_PLT));

  if (HasVariant)
    OS << '%' << getVariantKindName(getKind()) << '(';
  Expr->print(OS, MAI);
  if (Kind == VK_Koda_CALL_PLT)
    OS << "@plt";
  if (HasVariant)
    OS << ')';
}

const MCFixup *KodaMCExpr::getPCRelHiFixup(const MCFragment **DFOut) const {
  MCValue AUIPCLoc;
  if (!getSubExpr()->evaluateAsRelocatable(AUIPCLoc, nullptr, nullptr))
    return nullptr;

  const MCSymbolRefExpr *AUIPCSRE = AUIPCLoc.getSymA();
  if (!AUIPCSRE)
    return nullptr;

  const MCSymbol *AUIPCSymbol = &AUIPCSRE->getSymbol();
  const auto *DF = dyn_cast_or_null<MCDataFragment>(AUIPCSymbol->getFragment());

  if (!DF)
    return nullptr;

  uint64_t Offset = AUIPCSymbol->getOffset();
  if (DF->getContents().size() == Offset) {
    DF = dyn_cast_or_null<MCDataFragment>(DF->getNextNode());
    if (!DF)
      return nullptr;
    Offset = 0;
  }

  for (const MCFixup &F : DF->getFixups()) {
    if (F.getOffset() != Offset)
      continue;

    switch ((unsigned)F.getKind()) {
    default:
      continue;
    case Koda::fixup_Koda_got_hi20:
    case Koda::fixup_Koda_tls_got_hi20:
    case Koda::fixup_Koda_tls_gd_hi20:
    case Koda::fixup_Koda_pcrel_hi20:
      if (DFOut)
        *DFOut = DF;
      return &F;
    }
  }

  return nullptr;
}

bool KodaMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                            const MCAsmLayout *Layout,
                                            const MCFixup *Fixup) const {
  // Explicitly drop the layout and assembler to prevent any symbolic folding in
  // the expression handling.  This is required to preserve symbolic difference
  // expressions to emit the paired relocations.
  if (!getSubExpr()->evaluateAsRelocatable(Res, nullptr, nullptr))
    return false;

  Res =
      MCValue::get(Res.getSymA(), Res.getSymB(), Res.getConstant(), getKind());
  // Custom fixup types are not valid with symbol difference expressions.
  return Res.getSymB() ? getKind() == VK_Koda_None : true;
}

void KodaMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

KodaMCExpr::VariantKind KodaMCExpr::getVariantKindForName(StringRef name) {
  return StringSwitch<KodaMCExpr::VariantKind>(name)
      .Case("lo", VK_Koda_LO)
      .Case("hi", VK_Koda_HI)
      .Case("pcrel_lo", VK_Koda_PCREL_LO)
      .Case("pcrel_hi", VK_Koda_PCREL_HI)
      .Case("got_pcrel_hi", VK_Koda_GOT_HI)
      .Case("tprel_lo", VK_Koda_TPREL_LO)
      .Case("tprel_hi", VK_Koda_TPREL_HI)
      .Case("tprel_add", VK_Koda_TPREL_ADD)
      .Case("tls_ie_pcrel_hi", VK_Koda_TLS_GOT_HI)
      .Case("tls_gd_pcrel_hi", VK_Koda_TLS_GD_HI)
      .Default(VK_Koda_Invalid);
}

StringRef KodaMCExpr::getVariantKindName(VariantKind Kind) {
  switch (Kind) {
  case VK_Koda_Invalid:
  case VK_Koda_None:
    llvm_unreachable("Invalid ELF symbol kind");
  case VK_Koda_LO:
    return "lo";
  case VK_Koda_HI:
    return "hi";
  case VK_Koda_PCREL_LO:
    return "pcrel_lo";
  case VK_Koda_PCREL_HI:
    return "pcrel_hi";
  case VK_Koda_GOT_HI:
    return "got_pcrel_hi";
  case VK_Koda_TPREL_LO:
    return "tprel_lo";
  case VK_Koda_TPREL_HI:
    return "tprel_hi";
  case VK_Koda_TPREL_ADD:
    return "tprel_add";
  case VK_Koda_TLS_GOT_HI:
    return "tls_ie_pcrel_hi";
  case VK_Koda_TLS_GD_HI:
    return "tls_gd_pcrel_hi";
  case VK_Koda_CALL:
    return "call";
  case VK_Koda_CALL_PLT:
    return "call_plt";
  case VK_Koda_32_PCREL:
    return "32_pcrel";
  }
  llvm_unreachable("Invalid ELF symbol kind");
}

static void fixELFSymbolsInTLSFixupKodapl(const MCExpr *Expr, MCAssembler &Asm) {
  switch (Expr->getKind()) {
  case MCExpr::Target:
    llvm_unreachable("Can't handle nested target expression");
    break;
  case MCExpr::Constant:
    break;

  case MCExpr::Binary: {
    const MCBinaryExpr *BE = cast<MCBinaryExpr>(Expr);
    fixELFSymbolsInTLSFixupKodapl(BE->getLHS(), Asm);
    fixELFSymbolsInTLSFixupKodapl(BE->getRHS(), Asm);
    break;
  }

  case MCExpr::SymbolRef: {
    // We're known to be under a TLS fixup, so any symbol should be
    // modified. There should be only one.
    const MCSymbolRefExpr &SymRef = *cast<MCSymbolRefExpr>(Expr);
    cast<MCSymbolELF>(SymRef.getSymbol()).setType(ELF::STT_TLS);
    break;
  }

  case MCExpr::Unary:
    fixELFSymbolsInTLSFixupKodapl(cast<MCUnaryExpr>(Expr)->getSubExpr(), Asm);
    break;
  }
}

void KodaMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
  switch (getKind()) {
  default:
    return;
  case VK_Koda_TPREL_HI:
  case VK_Koda_TLS_GOT_HI:
  case VK_Koda_TLS_GD_HI:
    break;
  }

  fixELFSymbolsInTLSFixupKodapl(getSubExpr(), Asm);
}

bool KodaMCExpr::evaluateAsConstant(int64_t &Res) const {
  MCValue Value;

  if (Kind == VK_Koda_PCREL_HI || Kind == VK_Koda_PCREL_LO ||
      Kind == VK_Koda_GOT_HI || Kind == VK_Koda_TPREL_HI ||
      Kind == VK_Koda_TPREL_LO || Kind == VK_Koda_TPREL_ADD ||
      Kind == VK_Koda_TLS_GOT_HI || Kind == VK_Koda_TLS_GD_HI ||
      Kind == VK_Koda_CALL || Kind == VK_Koda_CALL_PLT)
    return false;

  if (!getSubExpr()->evaluateAsRelocatable(Value, nullptr, nullptr))
    return false;

  if (!Value.isAbsolute())
    return false;

  Res = evaluateAsInt64(Value.getConstant());
  return true;
}

int64_t KodaMCExpr::evaluateAsInt64(int64_t Value) const {
  switch (Kind) {
  default:
    llvm_unreachable("Invalid kind");
  case VK_Koda_LO:
    return SignExtend64<12>(Value);
  case VK_Koda_HI:
    // Add 1 if bit 11 is 1, to compensate for low 12 bits being negative.
    return ((Value + 0x800) >> 12) & 0xfffff;
  }
}