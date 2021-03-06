//===- lib/ReaderWriter/ELF/Cpu0/Cpu0TargetHandler.cpp ----------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Atoms.h"
#include "Cpu0ExecutableWriter.h"
#include "Cpu0DynamicLibraryWriter.h"
#include "Cpu0TargetHandler.h"
#include "Cpu0LinkingContext.h"

using namespace lld;
using namespace elf;

Cpu0TargetHandler::Cpu0TargetHandler(Cpu0LinkingContext &context)
    : DefaultTargetHandler(context), _context(context),
      _cpu0TargetLayout(new Cpu0TargetLayout<Cpu0ELFType>(context)),
      _cpu0RelocationHandler(
          new Cpu0TargetRelocationHandler(*_cpu0TargetLayout.get(), context)) {}

void Cpu0TargetHandler::registerRelocationNames(Registry &registry) {
  registry.addKindTable(Reference::KindNamespace::ELF,
                        Reference::KindArch::Cpu0, kindStrings);
}

std::unique_ptr<Writer> Cpu0TargetHandler::getWriter() {
  switch (this->_context.getOutputELFType()) {
  case llvm::ELF::ET_EXEC:
    return std::unique_ptr<Writer>(new Cpu0ExecutableWriter<Cpu0ELFType>(
        _context, *_cpu0TargetLayout.get()));
  case llvm::ELF::ET_DYN:
    return std::unique_ptr<Writer>(
        new Cpu0DynamicLibraryWriter<Cpu0ELFType>(
            _context, *_cpu0TargetLayout.get()));
  case llvm::ELF::ET_REL:
    llvm_unreachable("TODO: support -r mode");
  default:
    llvm_unreachable("unsupported output type");
  }
}

const Registry::KindStrings Cpu0TargetHandler::kindStrings[] = {
  LLD_KIND_STRING_ENTRY(R_CPU0_NONE),
  LLD_KIND_STRING_ENTRY(R_CPU0_24),
  LLD_KIND_STRING_ENTRY(R_CPU0_32),
  LLD_KIND_STRING_ENTRY(R_CPU0_HI16),
  LLD_KIND_STRING_ENTRY(R_CPU0_LO16),
  LLD_KIND_STRING_ENTRY(R_CPU0_GPREL16),
  LLD_KIND_STRING_ENTRY(R_CPU0_LITERAL),
  LLD_KIND_STRING_ENTRY(R_CPU0_GOT16),
  LLD_KIND_STRING_ENTRY(R_CPU0_PC16),
  LLD_KIND_STRING_ENTRY(R_CPU0_GPREL32),
  LLD_KIND_STRING_ENTRY(R_CPU0_CALL16),
  LLD_KIND_STRING_ENTRY(R_CPU0_PC24),
  LLD_KIND_STRING_ENTRY(R_CPU0_GOT_HI16),
  LLD_KIND_STRING_ENTRY(R_CPU0_GOT_LO16),
  LLD_KIND_STRING_ENTRY(R_CPU0_RELGOT),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_GD),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_LDM),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_DTP_HI16),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_DTP_LO16),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_GOTTPREL),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_TPREL32),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_TP_HI16),
  LLD_KIND_STRING_ENTRY(R_CPU0_TLS_TP_LO16),
  LLD_KIND_STRING_ENTRY(R_CPU0_GLOB_DAT),
  LLD_KIND_STRING_ENTRY(R_CPU0_JUMP_SLOT),
  LLD_KIND_STRING_ENTRY(LLD_R_CPU0_GOTRELINDEX),
  LLD_KIND_STRING_END
};
