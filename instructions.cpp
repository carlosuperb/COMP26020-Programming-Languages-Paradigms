#include <sstream>
#include "instructions.h"

// ========== InstructionBase ==========
void InstructionBase::execute(ProcessorState& state) const {
  // virtual call that implements the actual functionality of the instruction
  _execute(state);

  // move the pc forward
  state.pc += INSTRUCTION_SIZE;

  // trim the accumulator and the PC to fit in number of bits of the architecture
  state.acc &= ARCH_BITMASK;
  state.pc &= ARCH_BITMASK;
}

addr_t InstructionBase::get_address() const {
  return _address;
}

void InstructionBase::_set_address(addr_t address) {
  _address = address & ARCH_BITMASK;
}

std::string InstructionBase::to_string() const {
    std::ostringstream oss;
    std::string instruction_name = name();

    if (instruction_name == "ADD") {
        oss << "ADD: ACC <- ACC + [" << get_address() << "]";
    } else if (instruction_name == "AND") {
        oss << "AND: ACC <- ACC & [" << get_address() << "]";
    } else if (instruction_name == "ORR") {
        oss << "ORR: ACC <- ACC | [" << get_address() << "]";
    } else if (instruction_name == "XOR") {
        oss << "XOR: ACC <- ACC ^ [" << get_address() << "]";
    } else if (instruction_name == "LDR") {
        oss << "LDR: ACC <- [" << get_address() << "]";
    } else if (instruction_name == "STR") {
        oss << "STR: ACC -> [" << get_address() << "]";
    } else if (instruction_name == "JMP") {
        oss << "JMP: PC  <- " << get_address();
    } else if (instruction_name == "JNE") {
        oss << "JNE: PC  <- " << get_address() << " if ACC != 0";
    }
    
    return oss.str();
}

std::unique_ptr<InstructionBase> InstructionBase::generateInstruction(InstructionData data) {
    switch(data.opcode) {
        case ADD: return std::make_unique<Iadd>(data.address);
        case AND: return std::make_unique<Iand>(data.address);
        case ORR: return std::make_unique<Iorr>(data.address);
        case XOR: return std::make_unique<Ixor>(data.address);
        case LDR: return std::make_unique<Ildr>(data.address);
        case STR: return std::make_unique<Istr>(data.address);
        case JMP: return std::make_unique<Ijmp>(data.address);
        case JNE: return std::make_unique<Ijne>(data.address);
        default: return nullptr;
    }
}

// ========== ADD Instruction ==========
Iadd::Iadd(addr_t address) {
  _set_address(address);
}

void Iadd::_execute(ProcessorState& state) const {
  state.acc += state.memory.at(get_address());
}

const std::string Iadd::name() const {
  return "ADD";
}

// ========== AND Instruction ==========
Iand::Iand(addr_t address) {
  _set_address(address);
}

void Iand::_execute(ProcessorState& state) const {
  state.acc &= state.memory.at(get_address());
}

const std::string Iand::name() const {
  return "AND";
}

// ========== ORR Instruction ==========
Iorr::Iorr(addr_t address) {
  _set_address(address);
}

void Iorr::_execute(ProcessorState& state) const {
  state.acc |= state.memory.at(get_address());
}

const std::string Iorr::name() const {
  return "ORR";
}

// ========== XOR Instruction ==========
Ixor::Ixor(addr_t address) {
  _set_address(address);
}

void Ixor::_execute(ProcessorState& state) const {
  state.acc ^= state.memory.at(get_address());
}

const std::string Ixor::name() const {
  return "XOR";
}

// ========== LDR Instruction ==========
Ildr::Ildr(addr_t address) {
  _set_address(address);
}

void Ildr::_execute(ProcessorState& state) const {
  state.acc = state.memory.at(get_address());
}

const std::string Ildr::name() const {
  return "LDR";
}

// ========== STR Instruction ==========
Istr::Istr(addr_t address) {
  _set_address(address);
}

void Istr::_execute(ProcessorState& state) const {
  state.memory.at(get_address()) = state.acc;
}

const std::string Istr::name() const {
  return "STR";
}

// ========== JMP Instruction ==========
Ijmp::Ijmp(addr_t address) {
  _set_address(address);
}

void Ijmp::_execute(ProcessorState& state) const {
  // Why minus two? Because execute() will increment PC by two,
  // so to make the PC take (eventually) the value `address`
  // I need to subtract two here. Same applies for JNE below
  // This kind of unintuitive behaviour is a clear sign of bad
  // class hierarchy design 
  state.pc = get_address() - 2;
}

const std::string Ijmp::name() const {
  return "JMP";
}

// ========== JNE Instruction ==========
Ijne::Ijne(addr_t address) {
  _set_address(address);
}

void Ijne::_execute(ProcessorState& state) const {
  // Same hack as above
  if (state.acc != 0)
    state.pc = get_address() - 2;
}

const std::string Ijne::name() const {
  return "JNE";
}

