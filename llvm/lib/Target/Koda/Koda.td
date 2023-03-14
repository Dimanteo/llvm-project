include "llvm/Target/Target.td"
include "KodaRegisterInfo.td"
include "KodaInstrInfo.td"

def KodaInstrInfo : InstrInfo;
def Koda : Target {
    let InstructionSet = KodaInstrInfo;
}