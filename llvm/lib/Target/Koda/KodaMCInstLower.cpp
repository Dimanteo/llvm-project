#include "Koda.h"
#include "KodaSubtarget.h"
#include "MCTargetDesc/KodaMCExpr.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// TODO: verify
static MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym,
                                    const AsmPrinter &AP) {
  MCContext &Ctx = AP.OutContext;

  KodaMCExpr::VariantKind Kind;

  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Unknown target flag on GV operand");
  case KodaII::MO_None:
    Kind = KodaMCExpr::VK_Koda_None;
    break;
  case KodaII::MO_CALL:
    Kind = KodaMCExpr::VK_Koda_CALL;
    break;
  case KodaII::MO_PLT:
    Kind = KodaMCExpr::VK_Koda_CALL_PLT;
    break;
  case KodaII::MO_LO:
    Kind = KodaMCExpr::VK_Koda_LO;
    break;
  case KodaII::MO_HI:
    Kind = KodaMCExpr::VK_Koda_HI;
    break;
  case KodaII::MO_PCREL_LO:
    Kind = KodaMCExpr::VK_Koda_PCREL_LO;
    break;
  case KodaII::MO_PCREL_HI:
    Kind = KodaMCExpr::VK_Koda_PCREL_HI;
    break;
  case KodaII::MO_GOT_HI:
    Kind = KodaMCExpr::VK_Koda_GOT_HI;
    break;
  case KodaII::MO_TPREL_LO:
    Kind = KodaMCExpr::VK_Koda_TPREL_LO;
    break;
  case KodaII::MO_TPREL_HI:
    Kind = KodaMCExpr::VK_Koda_TPREL_HI;
    break;
  case KodaII::MO_TPREL_ADD:
    Kind = KodaMCExpr::VK_Koda_TPREL_ADD;
    break;
  case KodaII::MO_TLS_GOT_HI:
    Kind = KodaMCExpr::VK_Koda_TLS_GOT_HI;
    break;
  case KodaII::MO_TLS_GD_HI:
    Kind = KodaMCExpr::VK_Koda_TLS_GD_HI;
    break;
  }

  const MCExpr *ME =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);

  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset())
    ME = MCBinaryExpr::createAdd(
        ME, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);

  if (Kind != KodaMCExpr::VK_Koda_None)
    ME = KodaMCExpr::create(ME, Kind, Ctx);

  return MCOperand::createExpr(ME);
}

bool llvm::LowerKodaMachineOperandToMCOperand(const MachineOperand &MO,
                                              MCOperand &MCOp,
                                              const AsmPrinter &AP) {
  switch (MO.getType()) {
  default:
    report_fatal_error("LowerKodaMachineInstrToMCInst: unknown operand type");
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit())
      return false;
    MCOp = MCOperand::createReg(MO.getReg());
    break;
  case MachineOperand::MO_RegisterMask:
    // Regmasks are like implicit defs.
    return false;
  case MachineOperand::MO_Immediate:
    MCOp = MCOperand::createImm(MO.getImm());
    break;
  case MachineOperand::MO_MachineBasicBlock:
    MCOp = lowerSymbolOperand(MO, MO.getMBB()->getSymbol(), AP);
    break;
  case MachineOperand::MO_GlobalAddress:
    MCOp = lowerSymbolOperand(MO, AP.getSymbolPreferLocal(*MO.getGlobal()), AP);
    break;
  case MachineOperand::MO_BlockAddress:
    MCOp = lowerSymbolOperand(
        MO, AP.GetBlockAddressSymbol(MO.getBlockAddress()), AP);
    break;
  case MachineOperand::MO_ExternalSymbol:
    MCOp = lowerSymbolOperand(
        MO, AP.GetExternalSymbolSymbol(MO.getSymbolName()), AP);
    break;
  case MachineOperand::MO_ConstantPoolIndex:
    MCOp = lowerSymbolOperand(MO, AP.GetCPISymbol(MO.getIndex()), AP);
    break;
  case MachineOperand::MO_JumpTableIndex:
    MCOp = lowerSymbolOperand(MO, AP.GetJTISymbol(MO.getIndex()), AP);
    break;
  }
  return true;
}

bool llvm::lowerKodaMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                         AsmPrinter &AP) {
  OutMI.setOpcode(MI->getOpcode());

  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp;
    if (LowerKodaMachineOperandToMCOperand(MO, MCOp, AP))
      OutMI.addOperand(MCOp);
  }
  return false;
}