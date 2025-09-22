# COMP26020 Part5 5 - Paylock, Supplier, and Rental Contracts in Solidity

## Overview
This lab introduces the Solidity programming language (v0.6.x) and smart contract development using Remix IDE.  
The exercises focus on modeling contract interactions with deadlines, payments, and security considerations.  
We implement and extend three contracts:

- **Paylock** – manages deadlines and discounts for collecting work.
- **Supplier** – interacts with both Paylock and Rental to finish its task.
- **Rental** – provides resources with a refundable deposit, and is tested for reentrancy attacks.

---

## File Structure
- **paylock.sol** – Base implementation of Paylock, Supplier, and Rental with deadlines and deposits.
- **supplier.txt** – Initial starter code provided for the lab.
- **supplier2.sol** – Modified Supplier contract exploiting a reentrancy vulnerability in Rental.
- **supplier3.sol** – Fixed Rental contract that prevents reentrancy attacks.

---

## Exercises & Implementations
### Exercise 1 & 2 – Time Modeling in Paylock
- Added `int public clock` and a `tick()` function.
- Added `address timeAdd` to restrict `tick()` to a trusted third party.
- Enforced deadlines:
  - `collect_1_Y` only before clock < 4.
  - `collect_1_N` only when clock ≥ 4.
  - Second deadline handled by `collect_2_Y` and `collect_2_N`.

### Exercise 3 – Supplier and Rental Interaction
- Supplier acquires a resource with `aquire_resource()` (pays 1 wei deposit).
- Supplier returns the resource with `return_resource()`.
- Supplier finishes work with `finish()` (signals Paylock).

### Exercise 4 – Deposit Handling
- Rental requires **1 wei deposit** (`rent_out_resource`).
- Deposit returned in `retrieve_resource` using low-level `.call`.
- Supplier implements `receive()` to accept deposit refunds.

### Exercise 5 – Reentrancy Exploit
- `supplier2.sol`: Modified Supplier re-enters `retrieve_resource` via its `receive()` fallback to drain Rental.
- Demonstrates the classic **reentrancy attack**.

### Exercise 6 – Fixing the Vulnerability
- `supplier3.sol`: Reordered `retrieve_resource` in Rental to update state **before** returning deposit.
- This prevents reentrancy while preserving functionality.

---

## How to Compile & Run
1. Open [Remix IDE](https://remix.ethereum.org/) or the provided Blackboard version.
2. Load each `.sol` file and select the **Solidity compiler (0.6.x)**.
3. Deploy contracts in the following order:
   1. **Paylock** (constructor requires an address for the third party).
   2. **Rental**.
   3. **Supplier** (constructor requires Paylock and Rental addresses).
4. Interact with functions:
   - Use `tick()` to simulate time passing.
   - Call `aquire_resource()` with `1 wei` from Supplier.
   - Call `return_resource()` to return the resource and receive deposit back.
   - Call `finish()` to signal Paylock.
5. For reentrancy (supplier2.sol):
   - Fund Rental with extra Ether.
   - Deploy Supplier2 and call `return_resource()`.
   - Observe repeated calls draining Rental’s balance.
6. For fix (supplier3.sol):
   - Repeat the above test; Rental now resists reentrancy.

---

## Security Lessons
- Smart contracts must **update state before transferring Ether** to prevent reentrancy.
- Deposits and external calls should use defensive programming patterns (`checks-effects-interactions`).
- The lab demonstrates both the exploit and the fix.