#pragma once
// -----------------------------------------------------------------------------
// Project: 8-bit accumulator-based emulator
// File: emulator.h
// Author: Pavlos Petoumenos
// Date: 01/08/2024
//
// The classes in this file implement an emulator for an unrealistically simple
// processor architecture. While the general code organisation matches how real
// simulators are structured, the actual operations are, deliberately, much
// simpler. 
//
// To make this implementation slightly more interesting, the processor supports
// step execution and breakpoints.
//
// Separating the decode and execute stages and transfering dynamically created
// instruction objects between them is an overkill in this simple design. But
// this is exactly how it's done in real simulators where the flow of
// instructions through the processor can be much more complicated
// (e.g., out-of-order execution)
// -----------------------------------------------------------------------------

#include "common.h"
#include <vector>

//------------------------------------------------------------------------------
//--------------------               CONSTANTS              --------------------
//------------------------------------------------------------------------------
constexpr int MAX_INSTRUCTIONS = ((MEMORY_SIZE) / (INSTRUCTION_SIZE));

//------------------------------------------------------------------------------
//--------------------               CLASSES                --------------------
//------------------------------------------------------------------------------

/**
 * A representation of a breakpoint.
 *
 * This includes the actual address we break on and a symbolic name that
 * the application frontend and the user might use to refer to the breakpoint
 */
class Breakpoint {
  public:
    /**
     * The default constructor
     *
     * Only needed because creating an array of breakpoints, below, requires a default constructor.
     * The testing code does not use this, so remove it if your code does not need it
     */
    Breakpoint();

    /**
     * The actually useful constructor: creates a breakpoint with the given address and name.
     *
     * @param address The address on which we break
     * @param name A symbolic name for the breakpoint. We are not allowed to modify or take ownership of this string. The name can contain any alphanumeric character (no spaces allowed). The name is guaranteed to be valid (i.e. not null)
     */
    Breakpoint(addr_t address, const std::string& name);

    // Copy/Move Constructors
    Breakpoint(const Breakpoint& other);
    Breakpoint(Breakpoint&& other) noexcept;

    // Copy/Move Assignment Operators
    Breakpoint& operator=(const Breakpoint& other);
    Breakpoint& operator=(Breakpoint&& other) noexcept;

    // Destructor
    ~Breakpoint() = default;

    /**
     * Getter for the address
     */
    addr_t get_address() const;

    /**
     * Getter for the name
     */
    const std::string get_name() const;

    /**
     * Testing whether the breakpoint targets this address
     */
    bool has(addr_t address) const;

    /**
     * Testing whether the breakpoint targets this name
     */
    bool has(const std::string& name) const;

  private:
    addr_t _address{0};
    std::string _name;
};

/**
 * The actual emulator
 *
 * It provides four groups of methods:
 * a) Initialisation
 * b) Main emulation loop
 * c) Controlling the emulation
 * d) Managing the breakpoints
 * e) Accessing and printing the processor state
 */
class Emulator {
  public:
    // ----------> Initialisation
    
    /**
     * Default constructor
     *
     * Sets the state of the emulator to something reasonable.
     * The testing code expects that a default constructor exists, though not necessarily one that you've written
     */
    Emulator();

    /**
     * Copy constructor
     *
     * Replicates the full state of other
     */
    Emulator(const Emulator& other);

    /**
     * Move constructor
     *
     * Moves all the internal state of other into this
     */
    Emulator(Emulator&& other) noexcept;

    /**
     * Copy assignment operator
     *
     * Replaces the state of this with a copy of the state of other
     */
    Emulator& operator=(const Emulator& other);

    /**
     * Move assignment operator
     *
     * Replaces the state of this with the state of other
     */
    Emulator& operator=(Emulator&& other) noexcept;

    // Destructor
    ~Emulator() = default;



    // ----------> Main emulation loop

    /**
     * Get the two bytes representing the next instruction to execute
     *
     * @return a struct holding the instruction's two bytes
     */
    InstructionData fetch() const;

    /**
     * Transforms the instruction bytes into an InstructionBase object of the right dynamic type
     *
     * @param instruction The byte representation of the instruction
     * @return an **owning** object pointer inheriting from InstructionBase that has a) the dynamic type indicated by the instruction opcode and b) the target address indicated by the instruction's second byte
     */
    std::unique_ptr<InstructionBase> decode(InstructionData data) const;

    /**
     * A simple function just calling the instructions execute function
     *
     * @param instr The instruction to execute
     * @return whether the execution was successful (1 means success, 0 failure)
     */
    // int execute(InstructionBase* instr);
    int execute(const InstructionBase* instr);

    /**
     * Run iterations for a certain number of steps, until an error happens, or we reach a breakpoint
     *
     * @param steps The maximum number of cycles to execute 
     * @return whether we stopped normally or abnormally (1 means normally due to a breakpoint or after the maximum number of steps, 0 means abnormally due to an error)
     */
    int run(int steps);

    // ----------> Breakpoint management

    /**
     * Register a new breakpoint with the given address and name
     *
     * Fail if the name or the address are already registered. Also if we ran out of breakpoint storage but this should never happen.
     *
     * @param address The address to register
     * @param name The name of the breakpoints (non-owning pointer)
     * @return whether the operation was successful (1 means success, 0 failure)
     */
    int insert_breakpoint(addr_t address, const std::string& name);

    /**
     * Find the breakpoint with the given address in our breakpoint storage
     *
     * @param address The breakpoint address
     * @return A non-owning pointer to the Breakpoint or null if the address was not found
     */
    const Breakpoint* find_breakpoint(addr_t address) const;

    /**
     * Find the breakpoint with the given name in our breakpoint storage
     *
     * @param name The name of the breakpoint (non-owning pointer)
     * @return A non-owning pointer to the Breakpoint or null if the name was not found
     */
    const Breakpoint* find_breakpoint(const std::string& name) const;

    /**
     * Unregister the breakpoint with the given address
     *
     * @param address The address of the breakpoint
     * @return Whether a breakpoint was removed (1 means removed, 0 means none removed)
     */
    int delete_breakpoint(addr_t address);

    /**
     * Unregister the breakpoint with the given name
     *
     * @param name The name of the breakpoint (non-owning pointer)
     * @return Whether a breakpoint was removed (1 means removed, 0 means none removed)
     */
    int delete_breakpoint(const std::string& name);

    /**
     * Get the number of registered breakpoints
     *
     * @return The number of registered breakpoints
     */
    int num_breakpoints() const;

    // ----------> Manage state

    /**
     * Return the total number of cycles executes so far
     *
     * @return number of cycles
     */
    int cycles() const;

    /**
     * Getter for the accumulator
     */
    data_t read_acc() const;

    /** 
     * Getter for the PC
     */
    addr_t read_pc() const;

    /**
     * Read a memory byte
     *
     * @param address The address to read
     * @return the byte in that address
     */
    addr_t read_mem(addr_t address) const;

    // ----------> Utilities

    /**
     * Check whether the accumulator is zero
     *
     * This is useful for deciding what the JNE instruction will do
     * @return 1 if acc is zero, 0 if acc is not zero
     */
    int is_zero() const;

    /**
     * Do we have a breakpoint for the current PC
     *
     * @return 1 if this is a breakpoint, 0 otherwise
     */
    int is_breakpoint() const;

    /**
     * Prints on stdout all the state of the program
     *
     * @return 1 for success, 0 otherwise
     */
    int print_program() const;

    /**
     * Reads the processor state from a file
     *
     * The format is:
     * line 1 -> total number of cycles executed so far
     * line 2 -> value of acc
     * line 3 -> value of pc
     * line 4-259 -> All 256 memory bytes in their memory order. Each byte is printed as an unsigned number in its own line.
     * line 260-end -> One line for each active breakpoint, each line containing the address and name of the breakpoint separated by one space
     *
     * @param state_filename A string containing the name of the file to read
     * @return 1 for success, 0 otherwise
     */
    int load_state(const char* state_filename);

    /**
     * Stores the processor state in a file, in the same format used by load_state
     *
     * Same format as above
     *
     * @param state_filename A string containing the name of the file to write
     * @return 1 for success, 0 otherwise
     */
    int save_state(const char* state_filename) const;
  
  private:
    ProcessorState state;
    std::vector<Breakpoint> breakpoints;
    int total_cycles {0};
};
