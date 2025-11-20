# Secure Messaging Application (C Project)

This is a terminal-based secure messaging application written in C. It is a project designed to demonstrate core concepts of client-server architecture, network programming, and eventually, end-to-end encryption.

## Current Status

The project is in the initial local-only development phase. The command-line interface (CLI) for managing a local user, their conversations, and messages is partially implemented. The server-side components have not yet been started.

## Features Implemented

The `messenger.exe` executable currently supports the following local commands:

*   `createuser <username> <password>`: Creates a new local user directory and authentication files.
*   `createconv <username> <conv_name> <participants>`: Creates a new conversation for a user, generating a `meta.dat` file.
*   `listconv <username>`: Lists all conversations for a given user.
*   `sendlocal <username> <cnv_id> "<message>"`: Appends a message to a local conversation log (`messages_local.txt`).

## How to Build

This project is designed to be built with `gcc` (e.g., via MinGW on Windows) and `make`.

1.  Navigate to the project root directory.
2.  Run the `make` command:
    ```bash
    make
    ```
3.  This will compile the source files and create the `messenger.exe` executable in the root directory.

## How to Use

After building the project, you can use the executable from your terminal.

**Example Workflow:**

```bash
# 1. Create a user named "alex"
./messenger createuser alex mypass123

# 2. Create a new conversation for alex
./messenger createconv alex "Test-Chat" "alex,bob,cindy"

# 3. List alex's conversations to find the ID
./messenger listconv alex
# Output will show the conversation ID, e.g., 1678886400

# 4. Send a local message to that conversation
./messenger sendlocal alex 1678886400 "This is a test message."
```

## Project Roadmap

The complete project plan is outlined in `roadmap.md`. The next major phases include:
*   Implementing a multi-client TCP server.
*   Defining the client-server communication protocol.
*   Integrating encryption for secure communication.
