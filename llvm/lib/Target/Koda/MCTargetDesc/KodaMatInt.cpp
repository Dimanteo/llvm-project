//===- KodaMatInt.cpp - Immediate materialisation -------------*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "KodaMatInt.h"
#include "MCTargetDesc/KodaMCTargetDesc.h"
#include "llvm/ADT/APInt.h"
#include "llvm/Support/MathExtras.h"
using namespace llvm;

static int getInstSeqCost(KodaMatInt::InstSeq &Res, bool HasRVC) {
  if (!HasRVC)
    return Res.size();

  int Cost = 0;
  for (auto Instr : Res) {
    bool Compressed;
    switch (Instr.Opc) {
    default:
      llvm_unreachable("Unexpected opcode");
    case Koda::SLLI:
    case Koda::SRLI:
      Compressed = true;
      break;
    case Koda::ADDI:
    case Koda::LUI:
      Compressed = isInt<6>(Instr.Imm);
      break;
    }
    // Two RVC instructions take the same space as one RVI instruction, but
    // can take longer to execute than the single RVI instruction. Thus, we
    // consider that two RVC instruction are slightly more costly than one
    // RVI instruction. For longer sequences of RVC instructions the space
    // savings can be worth it, though. The costs below try to model that.
    if (!Compressed)
      Cost += 100; // Baseline cost of one RVI instruction: 100%.
    else
      Cost += 70; // 70% cost of baseline.
  }
  return Cost;
}

// Recursively generate a sequence for materializing an integer.
static void generateInstSeqImpl(int64_t Val,
                                const FeatureBitset &ActiveFeatures,
                                KodaMatInt::InstSeq &Res) {
  if (isInt<32>(Val)) {
    // Depending on the active bits in the immediate Value v, the following
    // instruction sequences are emitted:
    //
    // v == 0                        : ADDI
    // v[0,12) != 0 && v[12,32) == 0 : ADDI
    // v[0,12) == 0 && v[12,32) != 0 : LUI
    // v[0,32) != 0                  : LUI+ADDI(W)
    int64_t Hi20 = ((Val + 0x800) >> 12) & 0xFFFFF;
    int64_t Lo12 = SignExtend64<12>(Val);

    if (Hi20)
      Res.push_back(KodaMatInt::Inst(Koda::LUI, Hi20));

    if (Lo12 || Hi20 == 0) {
      unsigned AddiOpc = Koda::ADDI;
      Res.push_back(KodaMatInt::Inst(AddiOpc, Lo12));
    }
    return;
  }

  llvm_unreachable("Can't emit >32-bit imm for non-RV64 target");
}

static unsigned extractRotateInfo(int64_t Val) {
  // for case: 0b111..1..xxxxxx1..1..
  unsigned LeadingOnes = countLeadingOnes((uint64_t)Val);
  unsigned TrailingOnes = countTrailingOnes((uint64_t)Val);
  if (TrailingOnes > 0 && TrailingOnes < 64 &&
      (LeadingOnes + TrailingOnes) > (64 - 12))
    return 64 - TrailingOnes;

  // for case: 0bxxx1..1..1...xxx
  unsigned UpperTrailingOnes = countTrailingOnes(Hi_32(Val));
  unsigned LowerLeadingOnes = countLeadingOnes(Lo_32(Val));
  if (UpperTrailingOnes < 32 &&
      (UpperTrailingOnes + LowerLeadingOnes) > (64 - 12))
    return 32 - UpperTrailingOnes;

  return 0;
}

namespace llvm {
namespace KodaMatInt {
InstSeq generateInstSeq(int64_t Val, const FeatureBitset &ActiveFeatures) {
  KodaMatInt::InstSeq Res;
  generateInstSeqImpl(Val, ActiveFeatures, Res);
  return Res;
}

int getIntMatCost(const APInt &Val, unsigned Size,
                  const FeatureBitset &ActiveFeatures, bool CompressionCost) {
  bool HasRVC = false;
  int PlatRegSize = 32;

  // Split the constant into platform register sized chunks, and calculate cost
  // of each chunk.
  int Cost = 0;
  for (unsigned ShiftVal = 0; ShiftVal < Size; ShiftVal += PlatRegSize) {
    APInt Chunk = Val.ashr(ShiftVal).sextOrTrunc(PlatRegSize);
    InstSeq MatSeq = generateInstSeq(Chunk.getSExtValue(), ActiveFeatures);
    Cost += getInstSeqCost(MatSeq, HasRVC);
  }
  return std::max(1, Cost);
}
} // namespace KodaMatInt
} // namespace llvm
