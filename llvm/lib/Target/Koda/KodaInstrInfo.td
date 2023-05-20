include "KodaInstrFormats.td"

//===----------------------------------------------------------------------===//
//
// This file describes the Koda instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Type requirements for Koda instructions
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Operand and SDNode transformation definitions.
//===----------------------------------------------------------------------===//

def simm12 : Operand<i32> {}

def uimm12 : Operand<i32> {}

def simm20 : Operand<i32> {}

def uimmlog2 : Operand<i32> {}

def uimm20_lui : Operand<i32> {}

def uimm20_auipc : Operand<i32> {}

def fencearg : Operand<i32> {}

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
                 : $rs2), "$rd, $rs1, $rs2", []>;

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
                      : $rs1, uimmlog2 : $shamt),
                     opcodestr, "$rd, $rs1, $shamt">;

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
                                        (ins simm20 : $imm20),
                                        "jal", "$rd, $imm20", []>;

  let isCall = 1 in def JALR : KodaInstI<0b000, OPC_JALR,
                                         (outs HartRegs
                                          : $rd),
                                         (ins HartRegs
                                          : $rs1, simm12 : $imm12),
                                         "jalr", "$rd, ${imm12}(${rs1})", []>;
}

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class BranchCC_rri<bits<3> funct3, string opcodestr>
    : KodaInstB<funct3, OPC_BRANCH, (outs),
              (ins HartRegs:$rs1, HartRegs:$rs2, simm12 : $imm12),
              opcodestr, "$rs1, $rs2, $imm12", []> {
  let isBranch = 1;
  let isTerminator = 1;
}

def BEQ  : BranchCC_rri<0b000, "beq">;

def BNE  : BranchCC_rri<0b001, "bne">;
def BLT  : BranchCC_rri<0b100, "blt">;
def BGE  : BranchCC_rri<0b101, "bge">;
def BLTU : BranchCC_rri<0b110, "bltu">;
def BGEU : BranchCC_rri<0b111, "bgeu">;

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

let Defs = [X2], Uses = [X2] in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2), []>;
def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2), []>;
} // Defs = [X2], Uses = [X2]