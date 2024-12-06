#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>
#include "emulator.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

// ============= Breakpoint ==============
Breakpoint::Breakpoint() { }

Breakpoint::Breakpoint(addr_t address, const std::string& name) 
: _address(address & ARCH_BITMASK), _name(name) {}

// Copy constructor
Breakpoint::Breakpoint(const Breakpoint& other)
: _address(other._address), _name(other._name) {}

// Move constructor
Breakpoint::Breakpoint(Breakpoint&& other) noexcept
: _address(other._address), _name(std::move(other._name)) {}

// Copy assignment
Breakpoint& Breakpoint::operator=(const Breakpoint& other) {
  if (this != &other){
    _address = other._address;
    _name = other._name;
  }
  return *this;
}

// Move assignment
Breakpoint& Breakpoint::operator=(Breakpoint&& other) noexcept {
  if (this != &other){
    _address = other._address;
    _name = std::move(other._name);
  }
  return *this;
}

addr_t Breakpoint::get_address() const {
  return _address;
}

const std::string Breakpoint::get_name() const {
  return _name;
}

bool Breakpoint::has(addr_t address) const {
  return _address == (address & ARCH_BITMASK);
}

bool Breakpoint::has(const std::string& name) const {
  return _name == name;
}
// ============= Emulator ==============

// ----------> Initialisation
Emulator::Emulator()
    : state(),
      breakpoints(),
      total_cycles(0) {
    breakpoints.reserve(MAX_INSTRUCTIONS);
}

// Copy Constructor
Emulator::Emulator(const Emulator& other)
    : state(other.state),
      breakpoints(other.breakpoints),
      total_cycles(other.total_cycles) {}

// Move Constructor
Emulator::Emulator(Emulator&& other) noexcept
    : state(std::move(other.state)),
      breakpoints(std::move(other.breakpoints)),
      total_cycles(other.total_cycles) {}

// Copy Assignment Operator
Emulator& Emulator::operator=(const Emulator& other) {
  if (this != &other) {
    state = other.state;
    breakpoints = other.breakpoints;
    total_cycles = other.total_cycles;
  }
  return *this;
}

// Move Assignment Operator
Emulator& Emulator::operator=(Emulator&& other) noexcept {
  if (this != &other) {
    state = std::move(other.state);
    breakpoints = std::move(other.breakpoints);
    total_cycles = other.total_cycles;
  }
  return *this;
}

// ----------> Main emulation loop

InstructionData Emulator::fetch() const {
  return {state.memory.at(state.pc), state.memory.at(state.pc + 1)};
}

std::unique_ptr<InstructionBase> Emulator::decode(InstructionData data) const {
    return InstructionBase::generateInstruction(data);
}

int Emulator::execute(const InstructionBase* instr) {
    instr->execute(state);
    return 1;
}

int Emulator::run(int steps) {
    for (; steps > 0; --steps) {
        if ((state.pc % 2) == 1)
          return 0;

        auto instr = decode(fetch());
        if (!instr) return 0; // Handle null case

        int success = execute(instr.get());
        if (success == 0) return 0;

        ++total_cycles;
        if (is_breakpoint()) return 1;
    }
    return 1;
}

// ----------> Breakpoint management
int Emulator::insert_breakpoint(addr_t address, const std::string& name) {
    // Breakpoint already exists
    if (find_breakpoint(address) != nullptr)
        return 0;

    // Breakpoint name already used
    if (find_breakpoint(name) != nullptr)
        return 0;

    // Insert breakpoint
    breakpoints.emplace_back(address, name);
    return 1;
}

const Breakpoint* Emulator::find_breakpoint(addr_t address) const {
    auto it = std::find_if(breakpoints.begin(), breakpoints.end(),
                           [address](const Breakpoint& bp) { return bp.has(address); });

    return (it != breakpoints.end()) ? &(*it) : nullptr;
}

const Breakpoint* Emulator::find_breakpoint(const std::string& name) const {
    auto it = std::find_if(breakpoints.begin(), breakpoints.end(),
                           [name](const Breakpoint& bp) { return bp.has(name); });

    return (it != breakpoints.end()) ? &(*it) : nullptr;
}

int Emulator::delete_breakpoint(addr_t address) {
    auto it = std::remove_if(breakpoints.begin(), breakpoints.end(),
                             [address](const Breakpoint& bp) { return bp.has(address); });

    if (it == breakpoints.end())
        return 0; // Not found

    breakpoints.erase(it, breakpoints.end());
    return 1;
}

int Emulator::delete_breakpoint(const std::string& name) {
    auto it = std::remove_if(breakpoints.begin(), breakpoints.end(),
                             [name](const Breakpoint& bp) { return bp.has(name); });

    if (it == breakpoints.end())
        return 0; // Not found

    breakpoints.erase(it, breakpoints.end());
    return 1;
}

int Emulator::num_breakpoints() const {
    return static_cast<int>(breakpoints.size());
}

// ----------> Manage state
int Emulator::cycles() const {
  return total_cycles;
}

data_t Emulator::read_acc() const {
  return state.acc;
}

addr_t Emulator::read_pc() const {
  return state.pc;
}

addr_t Emulator::read_mem(addr_t address) const {
  // limit address to the allowed range of values
  address &= ARCH_BITMASK;
  return state.memory[address];
}

// ----------> Utilities

int Emulator::is_zero() const {
  return state.acc == 0;
}

int Emulator::is_breakpoint() const {
  return find_breakpoint(state.pc) != NULL;
}

int Emulator::print_program() const {
  for (int offset = 0; offset < MEMORY_SIZE; offset += INSTRUCTION_SIZE) {
    InstructionData data{state.memory[offset], state.memory[offset + 1]};

    auto instr = decode(data);

    if (!instr || (data.opcode == 0 && data.address == 0)) {
      std::cout << offset << ":\t" << static_cast<int>(data.opcode) 
                << "\t" << static_cast<int>(data.address) << "\n";
    } else {
        std::cout << offset << ":\t" << static_cast<int>(data.opcode) 
                  << "\t" << static_cast<int>(data.address) << "\t:\t" 
                  << instr->to_string() << "\n";
    }
  }
  return 1;
}

int Emulator::load_state(const char* filename) {
    // Clear all breakpoints
    breakpoints.clear();

    std::ifstream file(filename);
    if (!file.is_open())
        return 0;

    if (!(file >> total_cycles) || total_cycles < 0)
        return 0;

    if (!(file >> state.acc) || state.acc > ARCH_MAXVAL || state.acc < 0)
        return 0;

    if (!(file >> state.pc) || state.pc >= MEMORY_SIZE || state.pc < 0)
        return 0;

    int num = 0;
    for (int offset = 0; offset < MEMORY_SIZE; ++offset) {
        if (!(file >> num) || num > ARCH_MAXVAL || num < 0)
            return 0;
        state.memory[offset] = static_cast<byte_t>(num);
    }

    int address = 0;
    std::string name;
    while (file >> address >> name) {
        if (address < 0 || address >= MEMORY_SIZE)
            return 0;

        if (!insert_breakpoint(address, name))
            return 0;
    }

    return 1;
}

int Emulator::save_state(const char* filename) const {
    std::ofstream file(filename);
    if (!file.is_open())
        return 0;

    file << total_cycles << '\n';
    file << state.acc << '\n';
    file << state.pc << '\n';

    for (int offset = 0; offset < MEMORY_SIZE; ++offset) {
        file << static_cast<int>(state.memory[offset]) << '\n';
    }

    for (const auto& bp : breakpoints) {
        file << bp.get_address() << " " << bp.get_name() << '\n';
    }

    return 1;
}

