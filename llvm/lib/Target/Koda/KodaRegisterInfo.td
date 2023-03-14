//===----------------------------------------------------------------------===//
//  Declarations that describe the Koda register files
//===----------------------------------------------------------------------===//

class KodaReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
}

//===----------------------------------------------------------------------===//
//@Registers
//===----------------------------------------------------------------------===//
//@ All registers definition
let Namespace = "Koda" in {
  //@ General purpose registers
  foreach i = 1...31 in {
    def X#i : KodaReg<i, "x#i">, DwarfRegNum<[i]>;
  }
}

//===----------------------------------------------------------------------===//
//@Register Classes
//===----------------------------------------------------------------------===//
def HartRegs : RegisterClass<"Koda", [i32], 32, (add
  X1, X10, X11, X12, X13, X14, X15, X16, X17, X18, X19, X2, X20, X21, X22, X23, 
  X24, X25, X26, X27, X28, X29, X3, X30, X31, X4, X5, X6, X7, X8, X9)>;
