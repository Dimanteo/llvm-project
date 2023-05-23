//===-- KodaTargetStreamer.cpp - Koda Target Streamer Methods -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides Koda specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "KodaTargetStreamer.h"
#include "KodaInfo.h"
#include "KodaMCTargetDesc.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/KodaAttributes.h"
#include "llvm/Support/KodaISAInfo.h"

using namespace llvm;

KodaTargetStreamer::KodaTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

void KodaTargetStreamer::finish() { finishAttributeSection(); }

void KodaTargetStreamer::emitDirectiveOptionPush() {}
void KodaTargetStreamer::emitDirectiveOptionPop() {}
void KodaTargetStreamer::emitDirectiveOptionPIC() {}
void KodaTargetStreamer::emitDirectiveOptionNoPIC() {}
void KodaTargetStreamer::emitDirectiveOptionRVC() {}
void KodaTargetStreamer::emitDirectiveOptionNoRVC() {}
void KodaTargetStreamer::emitDirectiveOptionRelax() {}
void KodaTargetStreamer::emitDirectiveOptionNoRelax() {}
void KodaTargetStreamer::emitAttribute(unsigned Attribute, unsigned Value) {}
void KodaTargetStreamer::finishAttributeSection() {}
void KodaTargetStreamer::emitTextAttribute(unsigned Attribute,
                                            StringRef String) {}
void KodaTargetStreamer::emitIntTextAttribute(unsigned Attribute,
                                               unsigned IntValue,
                                               StringRef StringValue) {}

void KodaTargetStreamer::emitTargetAttributes(const MCSubtargetInfo &STI) {
//   if (STI.hasFeature(Koda::FeatureRV32E))
//     emitAttribute(KodaAttrs::STACK_ALIGN, KodaAttrs::ALIGN_4);
//   else
    emitAttribute(KodaAttrs::STACK_ALIGN, KodaAttrs::ALIGN_16);

  unsigned XLen = 32;
  std::vector<std::string> FeatureVector;
  KodaFeatures::toFeatureVector(FeatureVector, STI.getFeatureBits());

  auto ParseResult = llvm::KodaISAInfo::parseFeatures(XLen, FeatureVector);
  if (!ParseResult) {
    /* Assume any error about features should handled earlier.  */
    consumeError(ParseResult.takeError());
    llvm_unreachable("Parsing feature error when emitTargetAttributes?");
  } else {
    auto &ISAInfo = *ParseResult;
    emitTextAttribute(KodaAttrs::ARCH, ISAInfo->toString());
  }
}

// This part is for ascii assembly output
KodaTargetAsmStreamer::KodaTargetAsmStreamer(MCStreamer &S,
                                               formatted_raw_ostream &OS)
    : KodaTargetStreamer(S), OS(OS) {}

void KodaTargetAsmStreamer::emitDirectiveOptionPush() {
  OS << "\t.option\tpush\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionPop() {
  OS << "\t.option\tpop\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionPIC() {
  OS << "\t.option\tpic\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionNoPIC() {
  OS << "\t.option\tnopic\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionRVC() {
  OS << "\t.option\trvc\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionNoRVC() {
  OS << "\t.option\tnorvc\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionRelax() {
  OS << "\t.option\trelax\n";
}

void KodaTargetAsmStreamer::emitDirectiveOptionNoRelax() {
  OS << "\t.option\tnorelax\n";
}

void KodaTargetAsmStreamer::emitAttribute(unsigned Attribute, unsigned Value) {
  OS << "\t.attribute\t" << Attribute << ", " << Twine(Value) << "\n";
}

void KodaTargetAsmStreamer::emitTextAttribute(unsigned Attribute,
                                               StringRef String) {
  OS << "\t.attribute\t" << Attribute << ", \"" << String << "\"\n";
}

void KodaTargetAsmStreamer::emitIntTextAttribute(unsigned Attribute,
                                                  unsigned IntValue,
                                                  StringRef StringValue) {}

void KodaTargetAsmStreamer::finishAttributeSection() {}