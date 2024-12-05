#pragma once
// -----------------------------------------------------------------------------
// Project: 8-bit accumulator-based emulator
// File: instructions.h
// Author: Pavlos Petoumenos
// Date: 01/08/2024
//
// The classes in this file describe the instructions supported by our emulator.
// Our instructions are simple:
// - Arithmetic, logical, and memory instructions operate on the accumulator and
//   a memory address
// - Only one arithemetic instruction is implemented: ADD. For subtraction, you
//   need to add with the two's complement of the second value. To invert the
//   second argument, you need to XOR with 0xff.
// - Only three logical instructions are implemented: AND, ORR, XOR. Other
//   operations are composed from those three
// - Only one conditional branch is implemented: JNE (Jump if Not Equal). This
//   is generally not enough for real algorithms, but supporting more branches
//   would make this codebase unnecessarily long
// -----------------------------------------------------------------------------

#include "common.h"
#include <memory>

/** 
 * Enum representing the various opcodes.
 */
enum InstructionOpcode {
  ADD = 0,
  AND,
  ORR,
  XOR,
  LDR,
  STR,
  JMP,
  JNE,
  NUM_OPCODES
};

//------------------------------------------------------------------------------
//--------------------        INSTRUCTION SUBCLASSES        --------------------
//------------------------------------------------------------------------------

/**
 * Class representing an ADD instruction
 */
class Iadd final : public InstructionBase {
  public:
    explicit Iadd(addr_t address);
    void _execute(ProcessorState& state) const override;
    const std::string name() const override;
};

/**
 * Class representing an AND instruction
 */
class Iand final : public InstructionBase {
public:
  explicit Iand(addr_t address);
  void _execute(ProcessorState& state) const override;
  const std::string name() const override;
};

/**
 * Class representing an ORR instruction
 */
class Iorr final : public InstructionBase {
public:
  explicit Iorr(addr_t address);
  void _execute(ProcessorState& state) const override;
  const std::string name() const override;
};

/**
 * Class representing a XOR instruction
 */
class Ixor final : public InstructionBase {
  public:
    explicit Ixor(addr_t address);
    void _execute(ProcessorState& state) const override;
    const std::string name() const override;
};

/**
 * Class representing an LDR instruction
 */
class Ildr final : public InstructionBase {
  public:
    explicit Ildr(addr_t address);
    void _execute(ProcessorState& state) const override;
    const std::string name() const override;
};

/**
 * Class representing an STR instruction
 */
class Istr final : public InstructionBase {
  public:
    explicit Istr(addr_t address);
    void _execute(ProcessorState& state) const override;
    const std::string name() const override;
};

/**
 * Class representing an unconditional JMP
 */
class Ijmp final : public InstructionBase {
  public:
    explicit Ijmp(addr_t address);
    void _execute(ProcessorState& state) const override;
    const std::string name() const override;
};

/**
 * Class representing a conditional JNE
 */
class Ijne final : public InstructionBase {
public:
    explicit Ijne(addr_t address);
    void _execute(ProcessorState& state) const override;
    const std::string name() const override;
};
