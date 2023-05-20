include "llvm/Target/Target.td"
include "KodaRegisterInfo.td"
include "KodaInstrInfo.td"
include "KodaCallingConv.td"

def KodaInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

defvar Koda32 = DefaultMode;

def : ProcessorModel<"generic-32", NoSchedModel, []>;

def simAsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 1;
  let AllowDuplicateRegisterNames = 1;
}

def Koda : Target {
    let InstructionSet = KodaInstrInfo;
}