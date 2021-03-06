//===-- Cpu0Subtarget.h - Define Subtarget for the Cpu0 ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Cpu0 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef CPU0SUBTARGET_H
#define CPU0SUBTARGET_H

#include "llvm/Target/TargetSubtargetInfo.h"
#include "llvm/MC/MCInstrItineraries.h"
#include <string>

#define GET_SUBTARGETINFO_HEADER
#include "Cpu0GenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class Cpu0Subtarget : public Cpu0GenSubtargetInfo {
  virtual void anchor();

public:
  // NOTE: O64 will not be supported.
  enum Cpu0ABIEnum {
    UnknownABI, O32
  };

protected:
  enum Cpu0ArchEnum {
    Cpu032I,
    Cpu032II
  };

  // Cpu0 architecture version
  Cpu0ArchEnum Cpu0ArchVersion;

  // Cpu0 supported ABIs
  Cpu0ABIEnum Cpu0ABI;

  // IsLittle - The target is Little Endian
  bool IsLittle;

  // HasCmp - cmp instructions.
  bool HasCmp;

  // HasSlt - slt instructions.
  bool HasSlt;

  InstrItineraryData InstrItins;

  // Relocation Model
  Reloc::Model RM;

public:
  unsigned getTargetABI() const { return Cpu0ABI; }

  /// This constructor initializes the data members to match that
  /// of the specified triple.
  Cpu0Subtarget(const std::string &TT, const std::string &CPU,
                const std::string &FS, bool little, Reloc::Model _RM);

//- Vitual function, must have
  /// ParseSubtargetFeatures - Parses features string setting specified
  /// subtarget options.  Definition of function is auto generated by tblgen.
  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  bool isLittle() const { return IsLittle; }
  bool hasCpu032I() const { return Cpu0ArchVersion >= Cpu032I; }
  bool isCpu032I() const { return Cpu0ArchVersion == Cpu032I; }
  bool hasCpu032II() const { return Cpu0ArchVersion >= Cpu032II; }
  bool isCpu032II() const { return Cpu0ArchVersion == Cpu032II; }

  /// Features related to the presence of specific instructions.
  bool hasCmp()   const { return HasCmp; }
  bool hasSlt()   const { return HasSlt; }
};
} // End llvm namespace

#endif
