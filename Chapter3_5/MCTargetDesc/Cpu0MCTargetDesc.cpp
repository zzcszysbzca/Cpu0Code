//===-- Cpu0MCTargetDesc.cpp - Cpu0 Target Descriptions -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Cpu0 specific target descriptions.
//
//===----------------------------------------------------------------------===//
// #include
#include "Cpu0MCAsmInfo.h"
#include "Cpu0MCTargetDesc.h"
#include "InstPrinter/Cpu0InstPrinter.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/MC/MCELF.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "Cpu0GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "Cpu0GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "Cpu0GenRegisterInfo.inc"

using namespace llvm;

static std::string ParseCpu0Triple(StringRef TT, StringRef CPU) {
  std::string Cpu0ArchFeature;
  size_t DashPosition = 0;
  StringRef TheTriple;

  // Let's see if there is a dash, like cpu0-unknown-linux.
  DashPosition = TT.find('-');

  if (DashPosition == StringRef::npos) {
    // No dash, we check the string size.
    TheTriple = TT.substr(0);
  } else {
    // We are only interested in substring before dash.
    TheTriple = TT.substr(0,DashPosition);
  }

  if (TheTriple == "cpu0" || TheTriple == "cpu0el") {
    if (CPU.empty() || CPU == "cpu032II") {
      Cpu0ArchFeature = "+cpu032II";
    } else if (CPU == "cpu032I") {
      Cpu0ArchFeature = "+cpu032I";
    }
  }
  return Cpu0ArchFeature;
}

static MCInstrInfo *createCpu0MCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitCpu0MCInstrInfo(X); // defined in Cpu0GenInstrInfo.inc
  return X;
}

static MCRegisterInfo *createCpu0MCRegisterInfo(StringRef TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitCpu0MCRegisterInfo(X, Cpu0::LR); // defined in Cpu0GenRegisterInfo.inc
  return X;
}

static MCSubtargetInfo *createCpu0MCSubtargetInfo(StringRef TT, StringRef CPU,
                                                  StringRef FS) {
  std::string ArchFS = ParseCpu0Triple(TT,CPU);
  if (!FS.empty()) {
    if (!ArchFS.empty())
      ArchFS = ArchFS + "," + FS.str();
    else
      ArchFS = FS;
  }
  MCSubtargetInfo *X = new MCSubtargetInfo();
  InitCpu0MCSubtargetInfo(X, TT, CPU, ArchFS); // defined in Cpu0GenSubtargetInfo.inc
  return X;
}

static MCAsmInfo *createCpu0MCAsmInfo(const MCRegisterInfo &MRI, StringRef TT) {
  MCAsmInfo *MAI = new Cpu0MCAsmInfo(TT);

  unsigned SP = MRI.getDwarfRegNum(Cpu0::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::createDefCfa(0, SP, 0);
  MAI->addInitialFrameState(Inst);

  return MAI;
}

static MCCodeGenInfo *createCpu0MCCodeGenInfo(StringRef TT, Reloc::Model RM,
                                              CodeModel::Model CM,
                                              CodeGenOpt::Level OL) {
  MCCodeGenInfo *X = new MCCodeGenInfo();
  if (CM == CodeModel::JITDefault)
    RM = Reloc::Static;
  else if (RM == Reloc::Default)
    RM = Reloc::PIC_;
  X->InitMCCodeGenInfo(RM, CM, OL); // defined in lib/MC/MCCodeGenInfo.cpp
  return X;
}

static MCInstPrinter *createCpu0MCInstPrinter(const Target &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI,
                                              const MCSubtargetInfo &STI) {
  return new Cpu0InstPrinter(MAI, MII, MRI);
} // lbd document - mark - createCpu0MCInstPrinter

extern "C" void LLVMInitializeCpu0TargetMC() {
  // Register the MC asm info.
  RegisterMCAsmInfoFn X(TheCpu0Target, createCpu0MCAsmInfo);
  RegisterMCAsmInfoFn Y(TheCpu0elTarget, createCpu0MCAsmInfo);

  // Register the MC codegen info.
  TargetRegistry::RegisterMCCodeGenInfo(TheCpu0Target,
                                        createCpu0MCCodeGenInfo);
  TargetRegistry::RegisterMCCodeGenInfo(TheCpu0elTarget,
                                        createCpu0MCCodeGenInfo);
  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(TheCpu0Target, createCpu0MCInstrInfo);
  TargetRegistry::RegisterMCInstrInfo(TheCpu0elTarget, createCpu0MCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheCpu0Target, createCpu0MCRegisterInfo);
  TargetRegistry::RegisterMCRegInfo(TheCpu0elTarget, createCpu0MCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(TheCpu0Target,
                                          createCpu0MCSubtargetInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheCpu0elTarget,
                                          createCpu0MCSubtargetInfo);
  // Register the MCInstPrinter.
  TargetRegistry::RegisterMCInstPrinter(TheCpu0Target,
                                        createCpu0MCInstPrinter);
  TargetRegistry::RegisterMCInstPrinter(TheCpu0elTarget,
                                        createCpu0MCInstPrinter);
  // lbd document - mark - RegisterMCInstPrinter
}
