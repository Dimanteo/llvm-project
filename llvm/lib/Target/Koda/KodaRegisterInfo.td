//===----------------------------------------------------------------------===//
//  Declarations that describe the Koda register files
//===----------------------------------------------------------------------===//

let Namespace = "Koda" in {
class KodaReg<bits<5> Enc, string n> : Register<n> {
  let HWEncoding{4-0} = Enc;
}
}

//===----------------------------------------------------------------------===//
//@Registers
//===----------------------------------------------------------------------===//
//@ All registers definition
let Namespace = "Koda" in {
  //@ General purpose registers
  foreach i = 0...31 in {
    def X#i : KodaReg<i, "x"#i>, DwarfRegNum<[i]>;
  }
}

//===----------------------------------------------------------------------===//
//@Register Classes
//===----------------------------------------------------------------------===//
defvar Koda32 = DefaultMode;

def XLenVT : ValueTypeByHwMode<[Koda32],
                               [i32]>;
def XLenRI : RegInfoByHwMode<
      [Koda32],
      [RegInfo<32,32,32>]>;

def HartRegs : RegisterClass<"Koda", [XLenVT], 32,
    (add (sequence "X%u", 0, 31))> {
    let RegInfos = XLenRI;
  }

// Don't use X1 or X5 for JALR since that is a hint to pop the return address
// stack on some microarchitectures. Also remove the reserved registers X0, X2,
// X3, and X4 as it reduces the number of register classes that get synthesized
// by tablegen.
def JALRRegs : RegisterClass<"Koda", [XLenVT], 32, (sub HartRegs, (sequence "X%u", 0, 5))> {
  let RegInfos = XLenRI;
}
