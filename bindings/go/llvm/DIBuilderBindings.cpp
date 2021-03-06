//===- DIBuilderBindings.cpp - Bindings for DIBuilder ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines C bindings for the DIBuilder class.
//
//===----------------------------------------------------------------------===//

#include "DIBuilderBindings.h"
#include "IRBindings.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;

DEFINE_SIMPLE_CONVERSION_FUNCTIONS(DIBuilder, LLVMDIBuilderRef)

LLVMDIBuilderRef LLVMNewDIBuilder(LLVMModuleRef mref) {
  Module *m = unwrap(mref);
  return wrap(new DIBuilder(*m));
}

void LLVMDIBuilderDestroy(LLVMDIBuilderRef dref) {
  DIBuilder *d = unwrap(dref);
  delete d;
}

void LLVMDIBuilderFinalize(LLVMDIBuilderRef dref) { unwrap(dref)->finalize(); }

LLVMMetadataRef LLVMDIBuilderCreateCompileUnit(LLVMDIBuilderRef Dref,
                                               unsigned Lang, const char *File,
                                               const char *Dir,
                                               const char *Producer,
                                               int Optimized, const char *Flags,
                                               unsigned RuntimeVersion) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createCompileUnit(Lang, File, Dir, Producer, Optimized, Flags,
                                   RuntimeVersion));
}

LLVMMetadataRef LLVMDIBuilderCreateFile(LLVMDIBuilderRef Dref, const char *File,
                                        const char *Dir) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createFile(File, Dir));
}

LLVMMetadataRef LLVMDIBuilderCreateLexicalBlock(LLVMDIBuilderRef Dref,
                                                LLVMMetadataRef Scope,
                                                LLVMMetadataRef File,
                                                unsigned Line,
                                                unsigned Column) {
  DIBuilder *D = unwrap(Dref);
  auto *LB = D->createLexicalBlock(unwrap<MDLocalScope>(Scope),
                                   unwrap<MDFile>(File), Line, Column);
  return wrap(LB);
}

LLVMMetadataRef LLVMDIBuilderCreateLexicalBlockFile(LLVMDIBuilderRef Dref,
                                                    LLVMMetadataRef Scope,
                                                    LLVMMetadataRef File,
                                                    unsigned Discriminator) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createLexicalBlockFile(unwrap<MDLocalScope>(Scope),
                                        unwrap<MDFile>(File), Discriminator));
}

LLVMMetadataRef LLVMDIBuilderCreateFunction(
    LLVMDIBuilderRef Dref, LLVMMetadataRef Scope, const char *Name,
    const char *LinkageName, LLVMMetadataRef File, unsigned Line,
    LLVMMetadataRef CompositeType, int IsLocalToUnit, int IsDefinition,
    unsigned ScopeLine, unsigned Flags, int IsOptimized, LLVMValueRef Func) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createFunction(unwrap<MDScope>(Scope), Name, LinkageName,
                                File ? unwrap<MDFile>(File) : nullptr, Line,
                                unwrap<MDSubroutineType>(CompositeType),
                                IsLocalToUnit, IsDefinition, ScopeLine, Flags,
                                IsOptimized, unwrap<Function>(Func)));
}

LLVMMetadataRef LLVMDIBuilderCreateLocalVariable(
    LLVMDIBuilderRef Dref, unsigned Tag, LLVMMetadataRef Scope,
    const char *Name, LLVMMetadataRef File, unsigned Line, LLVMMetadataRef Ty,
    int AlwaysPreserve, unsigned Flags, unsigned ArgNo) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createLocalVariable(
      Tag, unwrap<MDScope>(Scope), Name, unwrap<MDFile>(File), Line,
      unwrap<MDType>(Ty), AlwaysPreserve, Flags, ArgNo));
}

LLVMMetadataRef LLVMDIBuilderCreateBasicType(LLVMDIBuilderRef Dref,
                                             const char *Name,
                                             uint64_t SizeInBits,
                                             uint64_t AlignInBits,
                                             unsigned Encoding) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createBasicType(Name, SizeInBits, AlignInBits, Encoding));
}

LLVMMetadataRef LLVMDIBuilderCreatePointerType(LLVMDIBuilderRef Dref,
                                               LLVMMetadataRef PointeeType,
                                               uint64_t SizeInBits,
                                               uint64_t AlignInBits,
                                               const char *Name) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createPointerType(unwrap<MDType>(PointeeType), SizeInBits,
                                   AlignInBits, Name));
}

LLVMMetadataRef
LLVMDIBuilderCreateSubroutineType(LLVMDIBuilderRef Dref, LLVMMetadataRef File,
                                  LLVMMetadataRef ParameterTypes) {
  DIBuilder *D = unwrap(Dref);
  return wrap(
      D->createSubroutineType(File ? unwrap<MDFile>(File) : nullptr,
                              MDTypeRefArray(unwrap<MDTuple>(ParameterTypes))));
}

LLVMMetadataRef LLVMDIBuilderCreateStructType(
    LLVMDIBuilderRef Dref, LLVMMetadataRef Scope, const char *Name,
    LLVMMetadataRef File, unsigned Line, uint64_t SizeInBits,
    uint64_t AlignInBits, unsigned Flags, LLVMMetadataRef DerivedFrom,
    LLVMMetadataRef ElementTypes) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createStructType(
      unwrap<MDScope>(Scope), Name, File ? unwrap<MDFile>(File) : nullptr, Line,
      SizeInBits, AlignInBits, Flags,
      DerivedFrom ? unwrap<MDType>(DerivedFrom) : nullptr,
      ElementTypes ? DebugNodeArray(unwrap<MDTuple>(ElementTypes)) : nullptr));
}

LLVMMetadataRef LLVMDIBuilderCreateReplaceableCompositeType(
    LLVMDIBuilderRef Dref, unsigned Tag, const char *Name,
    LLVMMetadataRef Scope, LLVMMetadataRef File, unsigned Line,
    unsigned RuntimeLang, uint64_t SizeInBits, uint64_t AlignInBits,
    unsigned Flags) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createReplaceableCompositeType(
      Tag, Name, unwrap<MDScope>(Scope), File ? unwrap<MDFile>(File) : nullptr,
      Line, RuntimeLang, SizeInBits, AlignInBits, Flags));
}

LLVMMetadataRef
LLVMDIBuilderCreateMemberType(LLVMDIBuilderRef Dref, LLVMMetadataRef Scope,
                              const char *Name, LLVMMetadataRef File,
                              unsigned Line, uint64_t SizeInBits,
                              uint64_t AlignInBits, uint64_t OffsetInBits,
                              unsigned Flags, LLVMMetadataRef Ty) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createMemberType(
      unwrap<MDScope>(Scope), Name, File ? unwrap<MDFile>(File) : nullptr, Line,
      SizeInBits, AlignInBits, OffsetInBits, Flags, unwrap<MDType>(Ty)));
}

LLVMMetadataRef LLVMDIBuilderCreateArrayType(LLVMDIBuilderRef Dref,
                                             uint64_t SizeInBits,
                                             uint64_t AlignInBits,
                                             LLVMMetadataRef ElementType,
                                             LLVMMetadataRef Subscripts) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createArrayType(SizeInBits, AlignInBits,
                                 unwrap<MDType>(ElementType),
                                 DebugNodeArray(unwrap<MDTuple>(Subscripts))));
}

LLVMMetadataRef LLVMDIBuilderCreateTypedef(LLVMDIBuilderRef Dref,
                                           LLVMMetadataRef Ty, const char *Name,
                                           LLVMMetadataRef File, unsigned Line,
                                           LLVMMetadataRef Context) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createTypedef(unwrap<MDType>(Ty), Name,
                               File ? unwrap<MDFile>(File) : nullptr, Line,
                               Context ? unwrap<MDScope>(Context) : nullptr));
}

LLVMMetadataRef LLVMDIBuilderGetOrCreateSubrange(LLVMDIBuilderRef Dref,
                                                 int64_t Lo, int64_t Count) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->getOrCreateSubrange(Lo, Count));
}

LLVMMetadataRef LLVMDIBuilderGetOrCreateArray(LLVMDIBuilderRef Dref,
                                              LLVMMetadataRef *Data,
                                              size_t Length) {
  DIBuilder *D = unwrap(Dref);
  Metadata **DataValue = unwrap(Data);
  ArrayRef<Metadata *> Elements(DataValue, Length);
  DebugNodeArray A = D->getOrCreateArray(Elements);
  return wrap(A.get());
}

LLVMMetadataRef LLVMDIBuilderGetOrCreateTypeArray(LLVMDIBuilderRef Dref,
                                                  LLVMMetadataRef *Data,
                                                  size_t Length) {
  DIBuilder *D = unwrap(Dref);
  Metadata **DataValue = unwrap(Data);
  ArrayRef<Metadata *> Elements(DataValue, Length);
  MDTypeRefArray A = D->getOrCreateTypeArray(Elements);
  return wrap(A.get());
}

LLVMMetadataRef LLVMDIBuilderCreateExpression(LLVMDIBuilderRef Dref,
                                              int64_t *Addr, size_t Length) {
  DIBuilder *D = unwrap(Dref);
  return wrap(D->createExpression(ArrayRef<int64_t>(Addr, Length)));
}

LLVMValueRef LLVMDIBuilderInsertDeclareAtEnd(LLVMDIBuilderRef Dref,
                                             LLVMValueRef Storage,
                                             LLVMMetadataRef VarInfo,
                                             LLVMMetadataRef Expr,
                                             LLVMBasicBlockRef Block) {
  // Fail immediately here until the llgo folks update their bindings.  The
  // called function is going to assert out anyway.
  llvm_unreachable("DIBuilder API change requires a DebugLoc");

  DIBuilder *D = unwrap(Dref);
  Instruction *Instr = D->insertDeclare(
      unwrap(Storage), unwrap<MDLocalVariable>(VarInfo),
      unwrap<MDExpression>(Expr), /* DebugLoc */ nullptr, unwrap(Block));
  return wrap(Instr);
}

LLVMValueRef LLVMDIBuilderInsertValueAtEnd(LLVMDIBuilderRef Dref,
                                           LLVMValueRef Val, uint64_t Offset,
                                           LLVMMetadataRef VarInfo,
                                           LLVMMetadataRef Expr,
                                           LLVMBasicBlockRef Block) {
  // Fail immediately here until the llgo folks update their bindings.  The
  // called function is going to assert out anyway.
  llvm_unreachable("DIBuilder API change requires a DebugLoc");

  DIBuilder *D = unwrap(Dref);
  Instruction *Instr = D->insertDbgValueIntrinsic(
      unwrap(Val), Offset, unwrap<MDLocalVariable>(VarInfo),
      unwrap<MDExpression>(Expr), /* DebugLoc */ nullptr, unwrap(Block));
  return wrap(Instr);
}
