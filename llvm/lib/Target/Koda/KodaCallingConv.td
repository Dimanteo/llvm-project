//===----------------------------------------------------------------------===//
// This describes the calling conventions for sim architecture.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// sim Return Value Calling Convention
//===----------------------------------------------------------------------===//
def RetCC_Koda : CallingConv<[
  // i32 return values
  CCIfType<[i32], CCAssignToReg<[X10, X11]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 16-byte aligned.
  CCIfType<[i32], CCAssignToStack<4, 1>>
]>;

//===----------------------------------------------------------------------===//
// sim Argument Calling Conventions
//===----------------------------------------------------------------------===//
def CC_Koda : CallingConv<[
  // Promote i8/i16 arguments to i32.
  CCIfType<[i8, i16], CCPromoteToType<i32>>,

  // The first 4 integer arguments are passed in integer registers.
  CCIfType<[i32], CCAssignToReg<[X10, X11, X12, X13, X14, X15, X16, X17]>>,

  // Integer values get stored in stack slots that are 4 bytes in
  // size and 16-byte aligned.
  CCIfType<[i32], CCAssignToStack<4, 1>>
]>;

def CSR_Koda : CalleeSavedRegs<(add X2, X8, X9, X10, (sequence "X%u", 18, 27))>;
def CSR_NoRegs : CalleeSavedRegs<(add)>;

// Interrupt handler needs to save/restore all registers that are used,
// both Caller and Callee saved registers.
def CSR_Interrupt : CalleeSavedRegs<(add X1,
    (sequence "X%u", 3, 9),
    (sequence "X%u", 10, 11),
    (sequence "X%u", 12, 17),
    (sequence "X%u", 18, 27),
    (sequence "X%u", 28, 31))>;