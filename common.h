#pragma once

// -----------------------------------------------------------------------------
// Project: 8-bit accumulator-based emulator
// File: common.h
// Author: Pavlos Petoumenos
// Date: 01/08/2024
//
// A few definitions that are used in both source files
// -----------------------------------------------------------------------------

#include <inttypes.h>
#include <array>
#include <string>
#include <memory>

//------------------------------------------------------------------------------
//--------------------               CONSTANTS              --------------------
//------------------------------------------------------------------------------
constexpr int ARCH_BITS = 8;
constexpr int ARCH_BITMASK = ((1 << ARCH_BITS) - 1);
constexpr int ARCH_MAXVAL = ARCH_BITMASK;
constexpr int INSTRUCTION_SIZE = 2;
constexpr int MEMORY_SIZE = 256;
constexpr int MAX_NAME = 96;


//------------------------------------------------------------------------------
//--------------------             HELPER TYPES             --------------------
//------------------------------------------------------------------------------

/**
 * Use the type `addr_t` for variables holding addresses.
 *
 * We define addr_t as just another name for `int`. `int` is an overkill
 * (addresses are supposed to be 8-bit), but C++ finds arithmetic operations on
 * 8-bit data suspicious, so making it an int will silence the warnings.
 *
 * Could have also used plain `int` for addresses, with no effect on
 * functionality, but it's considered better practice to use names that match
 * what the type represents, regardless of how that type is interpreted by the
 * compiler.
 *
 * Beyond the lectures:
 *
 * An even better practice would be to create a new custom **struct** `addr_t`
 * that encapsulates the actual value.
 *
 * typedef'ed types are syntactically interchangable, i.e., we can pass an
 * `int` to a function expecting a typedef'ed `addr_t`, becayse `addr_t` is just
 * another name for `int`.
 *
 * If `addr_t` was a struct containing an `int`, calling the function with an
 * `int` would be a compile-time error. Passing a plain `int` to a function
 * expecting an address is likely to be an error, so capturing this error at
 * compile-time is good!
 */
using addr_t = int;

/**
 * Use the type `data_t` for variables holding generic data.
 *
 * Similar issues as discussed for `addr_t`
 */
using data_t = int;

/**
 * Use the type byte_t for memory locations.
 * Here, for storage efficiency, we actually use only 8 bits for each byte.
 * Btw, uint8_t is an integer type guaranteed to be unsigned and 8 bits wide.
 */
using byte_t = uint8_t;

/**
 * A basic struct that just holds the two bytes representing the instruction in the memory
 *
 * Is this needed? Not really. But it simplifies old-style C++ programming.
 * You could replace this with an std::pair (and it might be a better choice), but
 * the testing code relies on this struct existing and being used in the right
 * places, so DON'T replace it.
 *
 */
struct InstructionData {
  byte_t opcode;
  byte_t address;
};

/**
 * A struct containing all the state of the processor.
 *
 * It's split to its own separate struct so that we can easily access it from
 * both the emulator engine and the instructions' code. It's a struct because
 * it offers little encapsulation or functionality.
 */
struct ProcessorState {
  /**
   * This is the only general purpose register.
   */
  data_t acc;

  /**
   * Holds the address of the instruction to be executed next.
   */
  addr_t pc;

  /**
   * Byte array representing the memory of the system
   */
  std::array<byte_t, MEMORY_SIZE> memory{};

  /**
   * The default constructor.
   * It resets the state of the machine.
   * There might be a more elegant way to achieve the same effect.
   */
  ProcessorState() 
    : acc(0), pc(0), memory{} {}
};

//------------------------------------------------------------------------------
//--------------------               CLASSES                --------------------
//------------------------------------------------------------------------------

/**
 * An interface for all the different kinds of instructions.
 *
 * All instruction classes inherit directly or indirectly from this abstract
 * class. This allows us to abstract many of the peculiarities of each
 * instruction type behind the two virtual calls of this base class:
 * _execute() and name().
 *
 * Beyond the lectures:
 *
 * There are more elegant and concise ways for building a class hierarchy of
 * instructions. Unfortunately, they involve heavy use of templates and/or
 * multiple inheritance. Ask me what this means in the lab. A verbose, but
 * easy to understand implementation is better in the context of this
 * assignment.
 *
 * If you feel like having fun, try to make the implementation of this class
 * hierarchy more concise. Ideally, make these changes separately from your
 * submission, since it's likely that making this code concise will break the
 * tests. Send me your attempt by email. I will give you feedback and, if
 * you've done something clever, you will get an extra mark.
 *
 * Is this extra mark worth the time and mental anguish? No, not really.
 * ¯\_(ツ)_/¯
 * So, do this only if you actually want to have "fun" and you have the
 * necessary bandwidth.
 */
class InstructionBase {
  public:
    /**
     * Modifies the system state by executing the instruction
     *
     * This is a wrapper around _execute() which is virtual and unimplemented
     * in InstructionBase. Inheriting classes are supposed to override _execute()
     * to implement the right semantics for the instruction.
     *
     * This method exists to do some instruction-independent bookkeeping after the state has changed:
     * 1. Move the pc to the next instruction
     * 2. Keep only the lower 8-bits for the pc and the accumulator
     *
     * @param state the processor state we operate on
     */
    void execute(ProcessorState& state) const;

    /**
     * Convenience getter for _address
     *
     * @return The address associated with this instruction
     */
    addr_t get_address() const;

    /** 
     * Generates a string describing the instruction
     *
     * @return the string (and its ownership)
     */
    // char* to_string() const;
    std::string to_string() const;

    /**
     * The instruction-specific functionality of executing an instruction.
     *
     * Declared unimplemented and virtual here.
     * Subclasses provide the concrete, instruction-specific behaviour.
     * @param state the processor state we operate on
     */
    virtual void _execute(ProcessorState& state) const = 0;

    /**
     * The name of this instruction class
     *
     * Declared unimplemented and virtual here.
     * Subclasses provide the concrete, instruction-specific behaviour.
     * @return The instruction mnemonic
     */
    virtual const std::string name() const = 0;

    /**
     * A class method translating opcodes into InstructionBase objects
     *
     * The method dynamically allocates an object of the right InstuctionBase
     * subclass and returns an owning pointer to it.
     *
     * @param opcode A number identifying the type of the instruction
     * @return A pointer to an object whose dynamic type matches the type requested
     */
    // static InstructionBase* generateInstruction(InstructionData data);
    static std::unique_ptr<InstructionBase> generateInstruction(InstructionData data);

    // Destructor
    virtual ~InstructionBase() = default;

  protected:
    /**
     * The default constructor
     *
     * Protected means it can only be used by its subclasses (for initialisation).
     * Other code is not allowed to create plain Instruction objects. Since it's
     * protected you can remove it, if your code does not need it.
     */
    // InstructionBase() { };
    InstructionBase() = default;

    /**
     * A convenience protected setter for _address
     *
     * This is needed only for initialisation in the derived classes constructors,
     * so we need at least `protected` visibility.
     * _address is not meant to change after initialisation, so this setter
     * should not be public.
     * Since this is protected you can remove it, if your code does not need it.
     *
     * @param address The address to set
     */
    void _set_address(addr_t address);

  private:
    addr_t _address{0};
};
