# Project Progress

This file tracks the development progress against the project roadmap.

- `[x]` - Completed
- `[ ]` - Pending

## Phase 1: Core Data Structures and Local User Setup [COMPLETED]

- [x] Define Core Structs (`Message`, `Conversation`, `User`)
- [x] Implement Local User Creation (`create_user`)
- [x] Implement Local User Verification (`verify_user` - placeholder)

## Phase 2: Local Message & Conversation Management [COMPLETED]

- [x] Conversation Metadata (`meta.dat` read/write)
- [x] Message Storage (`messages_local.txt` append)
- [x] Testing Utility & CLI
  - [x] `createuser` command
  - [x] `createconv` command
  - [x] `sendlocal` command
  - [x] `listconv` command
  - [x] `readconv` command (to view messages)

## Phase 3: Basic TCP Server [PENDING]

- [ ] TCP Listener Setup
- [ ] Multi-client Handling (e.g., pthreads)
- [ ] Server-side Directory Setup
- [ ] `CONNECT`/`DISCONNECT` command handling

## Phase 4: Implementing the Communication Protocol (Plain Text) [PENDING]

- [ ] Server-Side: `CREATE`, `SEND`, `SYNC`
- [ ] Client-Side: `CREATE`, `SEND`, `SYNC`
- [ ] Error Handling

## Phase 5: End-to-End Integration and Testing [PENDING]

- [ ] Interactive Client CLI
- [ ] Full System Test (2+ clients)

## Phase 6: Encryption Foundation [PENDING]

- [ ] Secure Hashing (SHA256)
- [ ] Key Derivation (PBKDF2)

## Phase 7: Message Encryption Layer [PENDING]

- [ ] Encryption/Decryption Logic (AES-256)
- [ ] Update `SEND` and `SYNC` with encryption

## Phase 8: Final Review and Refinement [PENDING]

- [ ] Security Review
- [ ] Code Cleanup
