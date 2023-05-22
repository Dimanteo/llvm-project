include "KodaInstrFormats.td"

//===----------------------------------------------------------------------===//
//
// This file describes the Koda instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// RISC-V specific DAG Nodes.
//===----------------------------------------------------------------------===//

// Target-independent type requirements, but with target-specific formats.
def SDT_CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>,
                                       SDTCisVT<1, i32>]>;
def SDT_CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>,
                                     SDTCisVT<1, i32>]>;

// Target-dependent type requirements.
def SDT_KodaCall     : SDTypeProfile<0, -1, [SDTCisVT<0, XLenVT>]>;
def SDT_KodaSelectCC : SDTypeProfile<1, 5, [SDTCisSameAs<1, 2>,
                                             SDTCisVT<3, OtherVT>,
                                             SDTCisSameAs<0, 4>,
                                             SDTCisSameAs<4, 5>]>;
def SDT_KodaBrCC : SDTypeProfile<0, 4, [SDTCisSameAs<0, 1>,
                                         SDTCisVT<2, OtherVT>,
                                         SDTCisVT<3, OtherVT>]>;
def SDT_KodaReadCSR  : SDTypeProfile<1, 1, [SDTCisInt<0>, SDTCisInt<1>]>;
def SDT_KodaWriteCSR : SDTypeProfile<0, 2, [SDTCisInt<0>, SDTCisInt<1>]>;
def SDT_KodaSwapCSR  : SDTypeProfile<1, 2, [SDTCisInt<0>, SDTCisInt<1>,
                                             SDTCisInt<2>]>;
def SDT_KodaReadCycleWide : SDTypeProfile<2, 0, [SDTCisVT<0, i32>,
                                                  SDTCisVT<1, i32>]>;
def SDT_KodaIntUnaryOpW : SDTypeProfile<1, 1, [
  SDTCisSameAs<0, 1>, SDTCisVT<0, i64>
]>;
def SDT_KodaIntBinOpW : SDTypeProfile<1, 2, [
  SDTCisSameAs<0, 1>, SDTCisSameAs<0, 2>, SDTCisVT<0, i64>
]>;
def SDT_KodaIntShiftDOpW : SDTypeProfile<1, 3, [
  SDTCisSameAs<0, 1>, SDTCisSameAs<0, 2>, SDTCisVT<0, i64>, SDTCisVT<3, i64>
]>;

// Target-independent nodes, but with target-specific formats.
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_CallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END", SDT_CallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

// Target-dependent nodes.
def Koda_call      : SDNode<"KodaISD::CALL", SDT_KodaCall,
                             [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def Koda_ret_flag  : SDNode<"KodaISD::RET_FLAG", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def Koda_uret_flag : SDNode<"KodaISD::URET_FLAG", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue]>;
def Koda_sret_flag : SDNode<"KodaISD::SRET_FLAG", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue]>;
def Koda_mret_flag : SDNode<"KodaISD::MRET_FLAG", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue]>;
def Koda_selectcc  : SDNode<"KodaISD::SELECT_CC", SDT_KodaSelectCC>;
def Koda_brcc      : SDNode<"KodaISD::BR_CC", SDT_KodaBrCC,
                             [SDNPHasChain]>;
def Koda_tail      : SDNode<"KodaISD::TAIL", SDT_KodaCall,
                             [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def Koda_sllw      : SDNode<"KodaISD::SLLW", SDT_KodaIntBinOpW>;
def Koda_sraw      : SDNode<"KodaISD::SRAW", SDT_KodaIntBinOpW>;
def Koda_srlw      : SDNode<"KodaISD::SRLW", SDT_KodaIntBinOpW>;
def Koda_read_csr  : SDNode<"KodaISD::READ_CSR", SDT_KodaReadCSR,
                             [SDNPHasChain]>;
def Koda_write_csr : SDNode<"KodaISD::WRITE_CSR", SDT_KodaWriteCSR,
                             [SDNPHasChain]>;
def Koda_swap_csr  : SDNode<"KodaISD::SWAP_CSR", SDT_KodaSwapCSR,
                             [SDNPHasChain]>;

def Koda_read_cycle_wide : SDNode<"KodaISD::READ_CYCLE_WIDE",
                                   SDT_KodaReadCycleWide,
                                   [SDNPHasChain, SDNPSideEffect]>;

def sim_add_lo : SDNode<"simISD::ADD_LO", SDTIntBinOp>;
def sim_hi : SDNode<"simISD::HI", SDTIntUnaryOp>;
def sim_lla : SDNode<"simISD::LLA", SDTIntUnaryOp>;
def sim_add_tprel : SDNode<"simISD::ADD_TPREL",
                             SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                                  SDTCisSameAs<0, 2>,
                                                  SDTCisSameAs<0, 3>,
                                                  SDTCisInt<0>]>>;

def sim_la : SDNode<"simISD::LA", SDTLoad,
                      [SDNPHasChain, SDNPMayLoad, SDNPMemOperand]>;
def sim_la_tls_ie : SDNode<"simISD::LA_TLS_IE", SDTLoad,
                             [SDNPHasChain, SDNPMayLoad, SDNPMemOperand]>;
def sim_la_tls_gd : SDNode<"simISD::LA_TLS_GD", SDTIntUnaryOp>;


//===----------------------------------------------------------------------===//
// Operand and SDNode transformation definitions.
//===----------------------------------------------------------------------===//

class ImmXLenAsmOperand<string prefix, string suffix = ""> : AsmOperandClass {
  let Name = prefix # "ImmXLen" # suffix;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

class ImmAsmOperand<string prefix, int width, string suffix> : AsmOperandClass {
  let Name = prefix # "Imm" # width # suffix;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

// A parse method for (${gpr}) or 0(${gpr}), where the 0 is be silently ignored.
def ZeroOffsetMemOpOperand : AsmOperandClass {
  let Name = "ZeroOffsetMemOpOperand";
  let RenderMethod = "addRegOperands";
  let PredicateMethod = "isGPR";
  let ParserMethod = "parseZeroOffsetMemOp";
}

// Check if (add r, imm) can be optimized to (ADDI (ADDI r, imm0), imm1),
// in which imm = imm0 + imm1 and both imm0 and imm1 are simm12.
def AddiPair : PatLeaf<(imm), [{
  if (!N->hasOneUse())
    return false;
  // The immediate operand must be in range [-4096,-2049] or [2048,4094].
  int64_t Imm = N->getSExtValue();
  return (-4096 <= Imm && Imm <= -2049) || (2048 <= Imm && Imm <= 4094);
}]>;

// Return imm/2.
def AddiPairImmA : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant(N->getSExtValue() / 2, SDLoc(N),
                                   N->getValueType(0));
}]>;

// Return imm - imm/2.
def AddiPairImmB : SDNodeXForm<imm, [{
  int64_t Imm = N->getSExtValue();
  return CurDAG->getTargetConstant(Imm - Imm / 2, SDLoc(N),
                                   N->getValueType(0));
}]>;

def HartRegsMemZeroOffset : RegisterOperand<HartRegs> {
  let ParserMatchClass = ZeroOffsetMemOpOperand;
  let PrintMethod = "printZeroOffsetMemOp";
}

class SImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"S", width, suffix> {
}

class UImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"U", width, suffix> {
}

def UImmLog2XLenAsmOperand : AsmOperandClass {
  let Name = "UImmLog2XLen";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = "InvalidUImmLog2XLen";
}

// A 12-bit signed immediate plus one where the imm range will be -2047~2048.
def simm12_plus1 : ImmLeaf<XLenVT,
  [{return (isInt<12>(Imm) && Imm != -2048) || Imm == 2048;}]>;

// Return the negation of an immediate value.
def NegImm : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant(-N->getSExtValue(), SDLoc(N),
                                   N->getValueType(0));
}]>;

def ixlenimm : Operand<XLenVT>;

def uimmlog2xlen : Operand<XLenVT>, ImmLeaf<XLenVT, [{
  return isUInt<5>(Imm);
}]> {
  let ParserMatchClass = UImmLog2XLenAsmOperand;
  // TODO: should ensure invalid shamt is rejected when decoding.
  let DecoderMethod = "decodeUImmOperand<6>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    if (STI.getTargetTriple().isArch64Bit())
      return isUInt<6>(Imm);
    return isUInt<5>(Imm);
  }];
  let OperandType = "OPERAND_UIMMLOG2XLEN";
  let OperandNamespace = "KodaOp";
}

def simm12 : Operand<XLenVT>, ImmLeaf<XLenVT, [{return isInt<12>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<12>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<12>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<12>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_SIMM12";
  let OperandNamespace = "KodaOp";
}


// A 13-bit signed immediate where the least significant bit is zero.
def simm13_lsb0 : Operand<OtherVT> {
  let ParserMatchClass = SImmAsmOperand<13, "Lsb0">;
  let PrintMethod = "printBranchOperand";
  let EncoderMethod = "getImmOpValueAsr1";
  let DecoderMethod = "decodeSImmOperandAndLsl1<13>";
  let OperandType = "OPERAND_PCREL";
}

class UImm20Operand : Operand<XLenVT> {
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeUImmOperand<20>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isUInt<20>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_UIMM20";
  let OperandNamespace = "KodaOp";
}

def uimm20_lui : UImm20Operand {
  let ParserMatchClass = UImmAsmOperand<20, "LUI">;
}
def uimm20_auipc : UImm20Operand {
  let ParserMatchClass = UImmAsmOperand<20, "AUIPC">;
}

def Simm21Lsb0JALAsmOperand : SImmAsmOperand<21, "Lsb0JAL"> {
  let ParserMethod = "parseJALOffset";
}

def ixlenimm_li : Operand<XLenVT> {
  let ParserMatchClass = ImmXLenAsmOperand<"", "LI">;
}

// A 21-bit signed immediate where the least significant bit is zero.
def simm21_lsb0_jal : Operand<OtherVT> {
  let ParserMatchClass = Simm21Lsb0JALAsmOperand;
  let PrintMethod = "printBranchOperand";
  let EncoderMethod = "getImmOpValueAsr1";
  let DecoderMethod = "decodeSImmOperandAndLsl1<21>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isShiftedInt<20, 1>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_PCREL";
}

def fencearg : Operand<i32> {}

def CallSymbol : AsmOperandClass {
  let Name = "CallSymbol";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = "InvalidCallSymbol";
  let ParserMethod = "parseCallSymbol";
}

// A bare symbol used in call/tail only.
def call_symbol : Operand<XLenVT> {
  let ParserMatchClass = CallSymbol;
}

//===----------------------------------------------------------------------===//
// Instructions
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0,
    mayStore =
        0 in class ALU_rr<bits<7> funct7, bits<3> funct3, string opcodestr>
    : KodaInstR<funct7, funct3, OPC_OP,
                (outs HartRegs
                 : $rd),
                (ins HartRegs
                 : $rs1, HartRegs
                 : $rs2), opcodestr, "$rd, $rs1, $rs2", []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in 
class ALU_ri<bits<3> funct3, string opcodestr>
    : KodaInstI<funct3, OPC_OP_IMM,
                (outs HartRegs
                 : $rd),
                (ins HartRegs
                 : $rs1, simm12 : $imm12),
                opcodestr, "$rd, $rs1, $imm12", []>;

let hasSideEffects = 0, mayLoad = 0,
    mayStore =
        0 in class Shift_ri<bits<5> imm11_7, bits<3> funct3, string opcodestr>
    : KodaInstIShift<imm11_7, funct3, OPC_OP_IMM,
                     (outs HartRegs
                      : $rd),
                     (ins HartRegs
                      : $rs1, uimmlog2xlen : $shamt),
                     opcodestr, "$rd, $rs1, $shamt">;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
def ADDIW : KodaInstI<0b000, OPC_OP_IMM_32, (outs HartRegs:$rd),
                    (ins HartRegs:$rs1, simm12:$imm12),
                    "addiw", "$rd, $rs1, $imm12", []>,
            Sched<[WriteIALU32, ReadIALU32]>;


def ADDI : ALU_ri<0b000, "addi">;
def SLTI : ALU_ri<0b010, "sltu">;
def SLTIU : ALU_ri<0b011, "sltiu">;
def ANDI : ALU_ri<0b111, "andi">;
def ORI : ALU_ri<0b110, "ori">;
def XORI : ALU_ri<0b100, "xori">;

def ADD : ALU_rr<0b0000000, 0b000, "add">;
def SUB : ALU_rr<0b0100000, 0b000, "sub">;
def SLL : ALU_rr<0b0000000, 0b001, "sll">;
def SLT : ALU_rr<0b0000000, 0b010, "slt">;
def SLTU : ALU_rr<0b0000000, 0b011, "sltu">;
def XOR : ALU_rr<0b0000000, 0b100, "xor">;
def SRL : ALU_rr<0b0000000, 0b101, "srl">;
def SRA : ALU_rr<0b0100000, 0b101, "sra">;
def OR : ALU_rr<0b0000000, 0b110, "or">;
def AND : ALU_rr<0b0000000, 0b111, "and">;

def SLLI : Shift_ri<0b00000, 0b001, "slli">;
def SRLI : Shift_ri<0b00000, 0b101, "srli">;
def SRAI : Shift_ri<0b01000, 0b101, "srai">;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in {
  def LUI : KodaInstU<OPC_LUI,
                      (outs HartRegs
                       : $rd),
                      (ins uimm20_lui : $imm20),
                      "lui", "$rd $imm20", []>;
  def AUIPC : KodaInstU<OPC_AUIPC,
                        (outs HartRegs
                         : $rd),
                        (ins uimm20_auipc : $imm20),
                        "lui", "$rd, $imm20", []>;

  let isCall = 1 in def JAL : KodaInstJ<OPC_JAL,
                                        (outs HartRegs
                                         : $rd),
                                        (ins simm21_lsb0_jal : $imm20),
                                        "jal", "$rd, $imm20", []>;

  let isCall = 1
  in def JALR : KodaInstI<0b000, OPC_JALR,
                                         (outs HartRegs
                                          : $rd),
                                         (ins HartRegs
                                          : $rs1, simm12 : $imm12),
                                         "jalr", "$rd, ${imm12}(${rs1})", []>;
}

// PseudoCALL is a pseudo instruction which will eventually expand to auipc
// and jalr while encoding. This is desirable, as an auipc+jalr pair with
// R_Koda_CALL and R_Koda_RELAX relocations can be be relaxed by the linker
// if the offset fits in a signed 21-bit immediate.
// Define AsmString to print "call" when compile with -S flag.
// Define isCodeGenOnly = 0 to support parsing assembly "call" instruction.
let isCall = 1, Defs = [X1], isCodeGenOnly = 0, Size = 8 in
def PseudoCALL : Pseudo<(outs), (ins call_symbol:$func), []> {
  let AsmString = "call\t$func";
}

def : Pat<(Koda_call tglobaladdr:$func), (PseudoCALL tglobaladdr:$func)>;
def : Pat<(Koda_call texternalsym:$func), (PseudoCALL texternalsym:$func)>;


let isCall = 1, Defs = [X1] in
def PseudoCALLIndirect : Pseudo<(outs), (ins JALRRegs:$rs1),
                                [(Koda_call JALRRegs:$rs1)]>,
                         PseudoInstExpansion<(JALR X1, HartRegs:$rs1, 0)>;

let isBarrier = 1, isBranch = 1, isIndirectBranch = 1, isTerminator = 1 in
def PseudoBRIND : Pseudo<(outs), (ins JALRRegs:$rs1, simm12:$imm12), []>,
                  PseudoInstExpansion<(JALR X0, HartRegs:$rs1, simm12:$imm12)>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class BranchCC_rri<bits<3> funct3, string opcodestr>
    : KodaInstB<funct3, OPC_BRANCH, (outs),
              (ins HartRegs:$rs1, HartRegs:$rs2, simm12 : $imm12),
              opcodestr, "$rs1, $rs2, $imm12", []>,
              Sched<[WriteJmp, ReadJmp, ReadJmp]>  {
  let isBranch = 1;
  let isTerminator = 1;
}

def BEQ  : BranchCC_rri<0b000, "beq">;

def BNE  : BranchCC_rri<0b001, "bne">;
def BLT  : BranchCC_rri<0b100, "blt">;
def BGE  : BranchCC_rri<0b101, "bge">;
def BLTU : BranchCC_rri<0b110, "bltu">;
def BGEU : BranchCC_rri<0b111, "bgeu">;

/// Setcc

class PatGprImm<SDPatternOperator OpNode, KodaInst Inst, ImmLeaf ImmType>
    : Pat<(XLenVT (OpNode (XLenVT HartRegs:$rs1), ImmType:$imm)),
          (Inst HartRegs:$rs1, ImmType:$imm)>;
class PatGprSimm12<SDPatternOperator OpNode, KodaInstI Inst>
    : PatGprImm<OpNode, Inst, simm12>;
class PatGprUimmLog2XLen<SDPatternOperator OpNode, KodaInstIShift Inst>
    : PatGprImm<OpNode, Inst, uimmlog2xlen>;

class PatGpr<SDPatternOperator OpNode, KodaInst Inst>
    : Pat<(OpNode HartRegs:$rs1), (Inst HartRegs:$rs1)>;
class PatGprGpr<SDPatternOperator OpNode, KodaInst Inst>
    : Pat<(OpNode HartRegs:$rs1, HartRegs:$rs2), 
          (Inst HartRegs:$rs1, HartRegs:$rs2)>;

def : PatGprGpr<setlt, SLT>;
def : PatGprSimm12<setlt, SLTI>;
def : PatGprGpr<setult, SLTU>;
def : PatGprSimm12<setult, SLTIU>;

// Define pattern expansions for setcc operations that aren't directly
// handled by a RISC-V instruction.
def : Pat<(seteq HartRegs:$rs1, 0), (SLTIU HartRegs:$rs1, 1)>;
def : Pat<(seteq HartRegs:$rs1, HartRegs:$rs2), (SLTIU (XOR HartRegs:$rs1, HartRegs:$rs2), 1)>;
def : Pat<(seteq HartRegs:$rs1, simm12_plus1:$imm12),
          (SLTIU (ADDI HartRegs:$rs1, (NegImm simm12_plus1:$imm12)), 1)>;
def : Pat<(setne HartRegs:$rs1, 0), (SLTU X0, HartRegs:$rs1)>;
def : Pat<(setne HartRegs:$rs1, HartRegs:$rs2), (SLTU X0, (XOR HartRegs:$rs1, HartRegs:$rs2))>;
def : Pat<(setne HartRegs:$rs1, simm12_plus1:$imm12),
          (SLTU X0, (ADDI HartRegs:$rs1, (NegImm simm12_plus1:$imm12)))>;
def : Pat<(setugt HartRegs:$rs1, HartRegs:$rs2), (SLTU HartRegs:$rs2, HartRegs:$rs1)>;
def : Pat<(setuge HartRegs:$rs1, HartRegs:$rs2), (XORI (SLTU HartRegs:$rs1, HartRegs:$rs2), 1)>;
def : Pat<(setule HartRegs:$rs1, HartRegs:$rs2), (XORI (SLTU HartRegs:$rs2, HartRegs:$rs1), 1)>;
def : Pat<(setgt HartRegs:$rs1, HartRegs:$rs2), (SLT HartRegs:$rs2, HartRegs:$rs1)>;
def : Pat<(setge HartRegs:$rs1, HartRegs:$rs2), (XORI (SLT HartRegs:$rs1, HartRegs:$rs2), 1)>;
def : Pat<(setle HartRegs:$rs1, HartRegs:$rs2), (XORI (SLT HartRegs:$rs2, HartRegs:$rs1), 1)>;

def IntCCtoKodaCC : SDNodeXForm<Koda_selectcc, [{
  ISD::CondCode CC = cast<CondCodeSDNode>(N->getOperand(2))->get();
  KodaCC::CondCode BrCC = getKodaCCForIntCC(CC);
  return CurDAG->getTargetConstant(BrCC, SDLoc(N), Subtarget->getXLenVT());
}]>;

def Koda_selectcc_frag : PatFrag<(ops node:$lhs, node:$rhs, node:$cc,
                                       node:$truev, node:$falsev),
                                  (Koda_selectcc node:$lhs, node:$rhs,
                                                  node:$cc, node:$truev,
                                                  node:$falsev), [{}],
                                  IntCCtoKodaCC>;

let usesCustomInserter = 1 in
class SelectCC_rrirr<RegisterClass valty, RegisterClass cmpty>
    : Pseudo<(outs valty:$dst),
             (ins cmpty:$lhs, cmpty:$rhs, ixlenimm:$imm,
              valty:$truev, valty:$falsev),
             [(set valty:$dst,
               (Koda_selectcc_frag:$imm cmpty:$lhs, cmpty:$rhs, cond,
                                         valty:$truev, valty:$falsev))]>;

def Select_GPR_Using_CC_GPR : SelectCC_rrirr<HartRegs, HartRegs>;

/// Branches and jumps

// Match `Koda_brcc` and lower to the appropriate RISC-V branch instruction.
class BccPat<CondCode Cond, KodaInstB Inst>
    : Pat<(Koda_brcc HartRegs:$rs1, HartRegs:$rs2, Cond, bb:$imm12),
          (Inst HartRegs:$rs1, HartRegs:$rs2, simm13_lsb0:$imm12)>;

def : BccPat<SETEQ, BEQ>;
def : BccPat<SETNE, BNE>;
def : BccPat<SETLT, BLT>;
def : BccPat<SETGE, BGE>;
def : BccPat<SETULT, BLTU>;
def : BccPat<SETUGE, BGEU>;

let isBarrier = 1, isBranch = 1, isTerminator = 1 in
def PseudoBR : Pseudo<(outs), (ins simm21_lsb0_jal:$imm20), [(br bb:$imm20)]>,
               PseudoInstExpansion<(JAL X0, simm21_lsb0_jal:$imm20)>;

let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
class Load_ri<bits<3> funct3, string opcodestr>
    : KodaInstI<funct3, OPC_LOAD, (outs HartRegs:$rd), (ins HartRegs:$rs1, simm12 : $imm12),
              opcodestr, "$rd, ${imm12}(${rs1})", []>;

def LB  : Load_ri<0b000, "lb">;
def LH  : Load_ri<0b001, "lh">;
def LW  : Load_ri<0b010, "lw">;
def LBU : Load_ri<0b100, "lbu">;
def LHU : Load_ri<0b101, "lhu">;

let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
class Store_rri<bits<3> funct3, string opcodestr>
    : KodaInstS<funct3, OPC_STORE, (outs),
              (ins HartRegs:$rs2, HartRegs:$rs1, simm12 : $imm12),
              opcodestr, "$rs2, ${imm12}(${rs1})", []>;

def SB : Store_rri<0b000, "sb">;
def SH : Store_rri<0b001, "sh">;
def SW : Store_rri<0b010, "sw">;

let hasSideEffects = 1, mayLoad = 0, mayStore = 0 in
def FENCE : KodaInstI<0b000, OPC_MISC_MEM, (outs),
                    (ins fencearg : $pred, fencearg : $succ),
                    "fence", "$pred, $succ", []> {
  bits<4> pred;
  bits<4> succ;

  let rs1 = 0;
  let rd = 0;
  let imm12 = {0b0000,pred,succ};
}

let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def PseudoRET : Pseudo<(outs), (ins), [(Koda_ret_flag)]>,
                PseudoInstExpansion<(JALR X0, X1, 0)>;

// Note that the size is 32 because up to 8 32-bit instructions are needed to
// generate an arbitrary 64-bit immediate. However, the size does not really
// matter since PseudoLI is currently only used in the AsmParser where it gets
// expanded to real instructions immediately.
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, Size = 32,
    isCodeGenOnly = 0, isAsmParserOnly = 1 in
def PseudoLI : Pseudo<(outs HartRegs:$rd), (ins ixlenimm_li:$imm), [],
                      "li", "$rd, $imm">;

def PseudoLB  : PseudoLoad<"lb">;
def PseudoLBU : PseudoLoad<"lbu">;
def PseudoLH  : PseudoLoad<"lh">;
def PseudoLHU : PseudoLoad<"lhu">;
def PseudoLW  : PseudoLoad<"lw">;

def PseudoSB  : PseudoStore<"sb">;
def PseudoSH  : PseudoStore<"sh">;
def PseudoSW  : PseudoStore<"sw">;

def : Pat<(brind JALRRegs:$rs1), (PseudoBRIND JALRRegs:$rs1, 0)>;
def : Pat<(brind (add JALRRegs:$rs1, simm12:$imm12)),
          (PseudoBRIND JALRRegs:$rs1, simm12:$imm12)>;

let EmitPriority = 0 in {
def : InstAlias<"lb $rd, (${rs1})",
                (LB  HartRegs:$rd, HartRegs:$rs1, 0)>;
def : InstAlias<"lh $rd, (${rs1})",
                (LH  HartRegs:$rd, HartRegs:$rs1, 0)>;
def : InstAlias<"lw $rd, (${rs1})",
                (LW  HartRegs:$rd, HartRegs:$rs1, 0)>;
def : InstAlias<"lbu $rd, (${rs1})",
                (LBU  HartRegs:$rd, HartRegs:$rs1, 0)>;
def : InstAlias<"lhu $rd, (${rs1})",
                (LHU  HartRegs:$rd, HartRegs:$rs1, 0)>;

def : InstAlias<"sb $rs2, (${rs1})",
                (SB  HartRegs:$rs2, HartRegs:$rs1, 0)>;
def : InstAlias<"sh $rs2, (${rs1})",
                (SH  HartRegs:$rs2, HartRegs:$rs1, 0)>;
def : InstAlias<"sw $rs2, (${rs1})",
                (SW  HartRegs:$rs2, HartRegs:$rs1, 0)>;
}

def IsOrAdd: PatFrag<(ops node:$A, node:$B), (or node:$A, node:$B), [{
  return isOrEquivalentToAdd(N);
}]>;

def AddrFI : ComplexPattern<iPTR, 1, "SelectAddrFI", [frameindex], []>;
def BaseAddr : ComplexPattern<iPTR, 1, "SelectBaseAddr">;

/// Simple arithmetic operations

def : PatGprGpr<add, ADD>;
def : PatGprSimm12<add, ADDI>;
def : PatGprGpr<sub, SUB>;
def : PatGprGpr<or, OR>;
def : PatGprSimm12<or, ORI>;
def : PatGprGpr<and, AND>;
def : PatGprSimm12<and, ANDI>;
def : PatGprGpr<xor, XOR>;
def : PatGprSimm12<xor, XORI>;
def : PatGprUimmLog2XLen<shl, SLLI>;
def : PatGprUimmLog2XLen<srl, SRLI>;
def : PatGprUimmLog2XLen<sra, SRAI>;

/// FrameIndex calculations

def : Pat<(add (XLenVT AddrFI:$Rs), simm12:$imm12),
          (ADDI (XLenVT AddrFI:$Rs), simm12:$imm12)>;
def : Pat<(IsOrAdd (XLenVT AddrFI:$Rs), simm12:$imm12),
          (ADDI (XLenVT AddrFI:$Rs), simm12:$imm12)>;

/// Simple optimization
def : Pat<(add HartRegs:$rs1, (AddiPair:$rs2)),
          (ADDI (ADDI HartRegs:$rs1, (AddiPairImmB AddiPair:$rs2)),
                (AddiPairImmA HartRegs:$rs2))>;

/// Loads

multiclass LdPat<PatFrag LoadOp, KodaInst Inst, ValueType vt = XLenVT> {
  def : Pat<(vt (LoadOp BaseAddr:$rs1)), (Inst BaseAddr:$rs1, 0)>;
  def : Pat<(vt (LoadOp (add BaseAddr:$rs1, simm12:$imm12))),
            (Inst BaseAddr:$rs1, simm12:$imm12)>;
  def : Pat<(vt (LoadOp (IsOrAdd AddrFI:$rs1, simm12:$imm12))),
            (Inst AddrFI:$rs1, simm12:$imm12)>;
}

defm : LdPat<sextloadi8, LB>;
defm : LdPat<extloadi8, LB>;
defm : LdPat<sextloadi16, LH>;
defm : LdPat<extloadi16, LH>;
defm : LdPat<load, LW, i32>;
defm : LdPat<zextloadi8, LBU>;
defm : LdPat<zextloadi16, LHU>;

/// Stores

multiclass StPat<PatFrag StoreOp, KodaInst Inst, RegisterClass StTy,
                 ValueType vt> {
  def : Pat<(StoreOp (vt StTy:$rs2), BaseAddr:$rs1),
            (Inst StTy:$rs2, BaseAddr:$rs1, 0)>;
  def : Pat<(StoreOp (vt StTy:$rs2), (add BaseAddr:$rs1, simm12:$imm12)),
            (Inst StTy:$rs2, BaseAddr:$rs1, simm12:$imm12)>;
  def : Pat<(StoreOp (vt StTy:$rs2), (IsOrAdd AddrFI:$rs1, simm12:$imm12)),
            (Inst StTy:$rs2, AddrFI:$rs1, simm12:$imm12)>;
}

defm : StPat<truncstorei8, SB, HartRegs, XLenVT>;
defm : StPat<truncstorei16, SH, HartRegs, XLenVT>;
defm : StPat<store, SW, HartRegs, i32>;

let Defs = [X2], Uses = [X2] in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2), [(callseq_start timm:$amt1, timm:$amt2)]>;
def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2), [(callseq_end timm:$amt1, timm:$amt2)]>;
} // Defs = [X2], Uses = [X2]