//===-- KodaELFObjectWriter.cpp - Koda ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/KodaFixupKinds.h"
#include "MCTargetDesc/KodaMCExpr.h"
#include "MCTargetDesc/KodaMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class KodaELFObjectWriter : public MCELFObjectTargetWriter {
public:
  KodaELFObjectWriter(uint8_t OSABI, bool Is64Bit);

  ~KodaELFObjectWriter() override;

  // Return true if the given relocation must be with a symbol rather than
  // section plus offset.
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override {
    // TODO: this is very conservative, update once RISC-V psABI requirements
    //       are clarified.
    return true;
  }

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
}

KodaELFObjectWriter::KodaELFObjectWriter(uint8_t OSABI, bool Is64Bit)
    : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_KODA,
                              /*HasRelocationAddend*/ true) {}

KodaELFObjectWriter::~KodaELFObjectWriter() {}

unsigned KodaELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  const MCExpr *Expr = Fixup.getValue();
  // Determine the type of the relocation
  unsigned Kind = Fixup.getTargetKind();
  if (Kind >= FirstLiteralRelocationKind)
    return Kind - FirstLiteralRelocationKind;
  if (IsPCRel) {
    switch (Kind) {
    default:
      Ctx.reportError(Fixup.getLoc(), "Unsupported relocation type");
      return ELF::R_Koda_NONE;
    case FK_Data_4:
    case FK_PCRel_4:
      return ELF::R_Koda_32_PCREL;
    case Koda::fixup_Koda_pcrel_hi20:
      return ELF::R_Koda_PCREL_HI20;
    case Koda::fixup_Koda_pcrel_lo12_i:
      return ELF::R_Koda_PCREL_LO12_I;
    case Koda::fixup_Koda_pcrel_lo12_s:
      return ELF::R_Koda_PCREL_LO12_S;
    case Koda::fixup_Koda_got_hi20:
      return ELF::R_Koda_GOT_HI20;
    case Koda::fixup_Koda_tls_got_hi20:
      return ELF::R_Koda_TLS_GOT_HI20;
    case Koda::fixup_Koda_tls_gd_hi20:
      return ELF::R_Koda_TLS_GD_HI20;
    case Koda::fixup_Koda_jal:
      return ELF::R_Koda_JAL;
    case Koda::fixup_Koda_branch:
      return ELF::R_Koda_BRANCH;
    case Koda::fixup_Koda_rvc_jump:
      return ELF::R_Koda_RVC_JUMP;
    case Koda::fixup_Koda_rvc_branch:
      return ELF::R_Koda_RVC_BRANCH;
    case Koda::fixup_Koda_call:
      return ELF::R_Koda_CALL;
    case Koda::fixup_Koda_call_plt:
      return ELF::R_Koda_CALL_PLT;
    case Koda::fixup_Koda_add_8:
      return ELF::R_Koda_ADD8;
    case Koda::fixup_Koda_sub_8:
      return ELF::R_Koda_SUB8;
    case Koda::fixup_Koda_add_16:
      return ELF::R_Koda_ADD16;
    case Koda::fixup_Koda_sub_16:
      return ELF::R_Koda_SUB16;
    case Koda::fixup_Koda_add_32:
      return ELF::R_Koda_ADD32;
    case Koda::fixup_Koda_sub_32:
      return ELF::R_Koda_SUB32;
    case Koda::fixup_Koda_add_64:
      return ELF::R_Koda_ADD64;
    case Koda::fixup_Koda_sub_64:
      return ELF::R_Koda_SUB64;
    }
  }

  switch (Kind) {
  default:
    Ctx.reportError(Fixup.getLoc(), "Unsupported relocation type");
    return ELF::R_Koda_NONE;
  case FK_Data_1:
    Ctx.reportError(Fixup.getLoc(), "1-byte data relocations not supported");
    return ELF::R_Koda_NONE;
  case FK_Data_2:
    Ctx.reportError(Fixup.getLoc(), "2-byte data relocations not supported");
    return ELF::R_Koda_NONE;
  case FK_Data_4:
    if (Expr->getKind() == MCExpr::Target &&
        cast<KodaMCExpr>(Expr)->getKind() == KodaMCExpr::VK_Koda_32_PCREL)
      return ELF::R_Koda_32_PCREL;
    return ELF::R_Koda_32;
  case FK_Data_8:
    return ELF::R_Koda_64;
  case Koda::fixup_Koda_hi20:
    return ELF::R_Koda_HI20;
  case Koda::fixup_Koda_lo12_i:
    return ELF::R_Koda_LO12_I;
  case Koda::fixup_Koda_lo12_s:
    return ELF::R_Koda_LO12_S;
  case Koda::fixup_Koda_tprel_hi20:
    return ELF::R_Koda_TPREL_HI20;
  case Koda::fixup_Koda_tprel_lo12_i:
    return ELF::R_Koda_TPREL_LO12_I;
  case Koda::fixup_Koda_tprel_lo12_s:
    return ELF::R_Koda_TPREL_LO12_S;
  case Koda::fixup_Koda_tprel_add:
    return ELF::R_Koda_TPREL_ADD;
  case Koda::fixup_Koda_relax:
    return ELF::R_Koda_RELAX;
  case Koda::fixup_Koda_align:
    return ELF::R_Koda_ALIGN;
  case Koda::fixup_Koda_set_6b:
    return ELF::R_Koda_SET6;
  case Koda::fixup_Koda_sub_6b:
    return ELF::R_Koda_SUB6;
  case Koda::fixup_Koda_add_8:
    return ELF::R_Koda_ADD8;
  case Koda::fixup_Koda_set_8:
    return ELF::R_Koda_SET8;
  case Koda::fixup_Koda_sub_8:
    return ELF::R_Koda_SUB8;
  case Koda::fixup_Koda_set_16:
    return ELF::R_Koda_SET16;
  case Koda::fixup_Koda_add_16:
    return ELF::R_Koda_ADD16;
  case Koda::fixup_Koda_sub_16:
    return ELF::R_Koda_SUB16;
  case Koda::fixup_Koda_set_32:
    return ELF::R_Koda_SET32;
  case Koda::fixup_Koda_add_32:
    return ELF::R_Koda_ADD32;
  case Koda::fixup_Koda_sub_32:
    return ELF::R_Koda_SUB32;
  case Koda::fixup_Koda_add_64:
    return ELF::R_Koda_ADD64;
  case Koda::fixup_Koda_sub_64:
    return ELF::R_Koda_SUB64;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createKodaELFObjectWriter(uint8_t OSABI, bool Is64Bit) {
  return std::make_unique<KodaELFObjectWriter>(OSABI, Is64Bit);
}