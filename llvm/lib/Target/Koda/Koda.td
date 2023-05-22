include "llvm/Target/Target.td"
include "KodaRegisterInfo.td"
include "KodaSchedule.td"
include "KodaCallingConv.td"
include "KodaInstrInfo.td"

def KodaInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : ProcessorModel<"generic-32", NoSchedModel, []>;

def simAsmParser : AsmParser {
  let ShouldEmitMatchRegisterAltName = 1;
  let AllowDuplicateRegisterNames = 1;
}

def Koda : Target {
    let InstructionSet = KodaInstrInfo;
}