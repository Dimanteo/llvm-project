include "llvm/Target/Target.td"
include "KodaRegisterInfo.td"
include "KodaInstrInfo.td"
include "KodaCallingConv.td"

def KodaInstrInfo : InstrInfo;

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def Koda : Target {
    let InstructionSet = KodaInstrInfo;
}