//===-- KodaMCTargetDesc.h - Koda Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Koda specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCTARGETDESC_H
#define LLVM_LIB_TARGET_Koda_MCTARGETDESC_KodaMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Config/config.h"

#include <memory>

namespace llvm {
class Target;
class Triple;
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;

extern Target TheKodaTarget;

MCCodeEmitter *createKodaMCCodeEmitter(const MCInstrInfo &MCII,
                                        const MCRegisterInfo &MRI,
                                        MCContext &Ctx);

std::unique_ptr<MCObjectTargetWriter> createKodaELFObjectWriter(uint8_t OSABI,
                                                                 bool Is64Bit);

MCAsmBackend *createKodaAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                    const MCRegisterInfo &MRI,
                                    const MCTargetOptions &Options);

} // End llvm namespace

// Defines symbolic names for Koda registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "KodaGenRegisterInfo.inc"

// Defines symbolic names for the Koda instructions.
#define GET_INSTRINFO_ENUM
#include "KodaGenInstrInfo.inc"

#endif