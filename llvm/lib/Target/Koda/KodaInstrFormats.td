//===----------------------------------------------------------------------===//
//  Describe Koda instructions format
//
//  Architecture is based on RISC-V
//===----------------------------------------------------------------------===//
class InstFormat<bits<5> val> {
  bits<5> Value = val;
}
def InstFormatPseudo : InstFormat<0>;
def InstFormatR      : InstFormat<1>;
def InstFormatI      : InstFormat<3>;
def InstFormatS      : InstFormat<4>;
def InstFormatB      : InstFormat<5>;
def InstFormatU      : InstFormat<6>;
def InstFormatJ      : InstFormat<7>;

class KodaInst<dag outs, dag ins, string asmstr, string argstr, list<dag> pattern, InstFormat format>
    : Instruction {
  let Namespace = "Koda";
  let DecoderNamespace = "Koda";
  let Size = 4;

  field bits<32> SoftFail = 0; // Expected by tablegen, not used in koda
  field bits<32> Inst;
  field bits<7> Opcode = 0;

  let Inst{6 - 0} = Opcode;

  let OutOperandList = outs;
  let InOperandList = ins;
  let AsmString = asmstr # "\t" # argstr;
  let Pattern = pattern;
  let TSFlags{4-0} = format.Value;
}

class KodaOpcode<string name, bits<7> val> {
  string Name = name;
  bits<7> Value = val;
}

def OPC_LOAD : KodaOpcode<"LOAD", 0b0000011>;
def OPC_LOAD_FP : KodaOpcode<"LOAD_FP", 0b0000111>;
def OPC_MISC_MEM : KodaOpcode<"MISC_MEM", 0b0001111>;
def OPC_OP_IMM : KodaOpcode<"OP_IMM", 0b0010011>;
def OPC_AUIPC : KodaOpcode<"AUIPC", 0b0010111>;
def OPC_OP_IMM_32 : KodaOpcode<"OP_IMM_32", 0b0011011>;
def OPC_STORE : KodaOpcode<"STORE", 0b0100011>;
def OPC_STORE_FP : KodaOpcode<"STORE_FP", 0b0100111>;
def OPC_AMO : KodaOpcode<"AMO", 0b0101111>;
def OPC_OP : KodaOpcode<"OP", 0b0110011>;
def OPC_LUI : KodaOpcode<"LUI", 0b0110111>;
def OPC_OP_32 : KodaOpcode<"OP_32", 0b0111011>;
def OPC_MADD : KodaOpcode<"MADD", 0b1000011>;
def OPC_MSUB : KodaOpcode<"MSUB", 0b1000111>;
def OPC_NMSUB : KodaOpcode<"NMSUB", 0b1001011>;
def OPC_NMADD : KodaOpcode<"NMADD", 0b1001111>;
def OPC_BRANCH : KodaOpcode<"BRANCH", 0b1100011>;
def OPC_JALR : KodaOpcode<"JALR", 0b1100111>;
def OPC_JAL : KodaOpcode<"JAL", 0b1101111>;
def OPC_SYSTEM : KodaOpcode<"SYSTEM", 0b1110011>;

def BareSymbol : AsmOperandClass {
  let Name = "BareSymbol";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = "InvalidBareSymbol";
  let ParserMethod = "parseBareSymbol";
}

// A bare symbol.
def bare_symbol : Operand<XLenVT> {
  let ParserMatchClass = BareSymbol;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, list<dag> pattern, string opcodestr = "", string argstr = "">
    : KodaInst<outs, ins, opcodestr, argstr, pattern, InstFormatPseudo> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// Pseudo load instructions.
class PseudoLoad<string opcodestr, RegisterClass rdty = HartRegs>
    : Pseudo<(outs rdty:$rd), (ins bare_symbol:$addr), [], opcodestr, "$rd, $addr"> {
  let hasSideEffects = 0;
  let mayLoad = 1;
  let mayStore = 0;
  let isCodeGenOnly = 0;
  let isAsmParserOnly = 1;
}

// Pseudo store instructions.
class PseudoStore<string opcodestr, RegisterClass rsty = HartRegs>
    : Pseudo<(outs HartRegs:$tmp), (ins rsty:$rs, bare_symbol:$addr), [], opcodestr, "$rs, $addr, $tmp"> {
  let hasSideEffects = 0;
  let mayLoad = 0;
  let mayStore = 1;
  let isCodeGenOnly = 0;
  let isAsmParserOnly = 1;
}

// Base Instruction Formats described in Volume I: RISC-V Unprivileged ISA

class KodaInstR<bits<7> funct7, bits<3> funct3, KodaOpcode op, dag outs,
                dag ins, string opstr, string asmstr, list<dag> pattern>
    : KodaInst<outs, ins, opstr, asmstr, pattern, InstFormatR> {
  bits<5> rs2;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31 - 25} = funct7;
  let Inst{24 - 20} = rs2;
  let Inst{19 - 15} = rs1;
  let Inst{14 - 12} = funct3;
  let Inst{11 - 7} = rd;
  let Opcode = op.Value;
}

class KodaInstI<bits<3> funct3, KodaOpcode op, dag outs, dag ins, string opstr,
                string asmstr, list<dag> pattern>
    : KodaInst<outs, ins, opstr, asmstr, pattern, InstFormatI> {
  bits<12> imm12;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31 - 20} = imm12;
  let Inst{19 - 15} = rs1;
  let Inst{14 - 12} = funct3;
  let Inst{11 - 7} = rd;
  let Opcode = op.Value;
}

class KodaInstS<bits<3> funct3, KodaOpcode op, dag outs, dag ins, string opstr,
                string asmstr, list<dag> pattern>
    : KodaInst<outs, ins, opstr, asmstr, pattern, InstFormatS> {
  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31 - 25} = imm12{11 - 5};
  let Inst{24 - 20} = rs2;
  let Inst{19 - 15} = rs1;
  let Inst{14 - 12} = funct3;
  let Inst{11 - 7} = imm12{4 - 0};
  let Opcode = op.Value;
}

class KodaInstB<bits<3> funct3, KodaOpcode opcode, dag outs, dag ins,
                string opcodestr, string asmstr, list<dag> pattern>
    : KodaInst<outs, ins, opcodestr, asmstr, pattern, InstFormatB> {
  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31} = imm12{11};
  let Inst{30 - 25} = imm12{9 - 4};
  let Inst{24 - 20} = rs2;
  let Inst{19 - 15} = rs1;
  let Inst{14 - 12} = funct3;
  let Inst{11 - 8} = imm12{3 - 0};
  let Inst{7} = imm12{10};
  let Opcode = opcode.Value;
}

class KodaInstU<KodaOpcode opcode, dag outs, dag ins, string opcodestr,
                string asmstr, list<dag> pattern>
    : KodaInst<outs, ins, opcodestr, asmstr, pattern, InstFormatU> {
  bits<20> imm20;
  bits<5> rd;

  let Inst{31 - 12} = imm20;
  let Inst{11 - 7} = rd;
  let Opcode = opcode.Value;
}

class KodaInstJ<KodaOpcode opcode, dag outs, dag ins, string opcodestr,
                string asmstr, list<dag> pattern>
    : KodaInst<outs, ins, opcodestr, asmstr, pattern, InstFormatJ> {
  bits<5> rd;
  bits<20> imm20;

  let Inst{31} = imm20{19};
  let Inst{30 - 21} = imm20{9 - 0};
  let Inst{20} = imm20{10};
  let Inst{19 - 12} = imm20{18 - 11};
  let Inst{11 - 7} = rd;
  let Opcode = opcode.Value;
}

class KodaInstIShift<bits<5> imm11_7, bits<3> funct3, KodaOpcode opcode,
                     dag outs, dag ins, string opcodestr, string asmstr>
    : KodaInst<outs, ins, opcodestr, asmstr, [], InstFormatI> {
  bits<6> shamt;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31 - 27} = imm11_7;
  let Inst{26} = 0;
  let Inst{25 - 20} = shamt;
  let Inst{19 - 15} = rs1;
  let Inst{14 - 12} = funct3;
  let Inst{11 - 7} = rd;
  let Opcode = opcode.Value;
}

class KodaInstIShiftW<bits<7> imm11_5, bits<3> funct3, KodaOpcode opcode,
                      dag outs, dag ins, string opcodestr, string asmstr>
    : KodaInst<outs, ins, opcodestr, asmstr, [], InstFormatI> {
  bits<5> shamt;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31 - 25} = imm11_5;
  let Inst{24 - 20} = shamt;
  let Inst{19 - 15} = rs1;
  let Inst{14 - 12} = funct3;
  let Inst{11 - 7} = rd;
  let Opcode = opcode.Value;
}
