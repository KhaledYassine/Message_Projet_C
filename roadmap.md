# Messaging Application Development Log

This document serves as a development log and roadmap for the C-based messaging application. It tracks implemented features and outlines future development phases.

## Current Status (Implemented Features)

The project currently implements a beginner-friendly command-line interface (CLI) messaging application with local file-based persistence.

**Key Implemented Features:**

*   **User Management:**
    *   User registration (`register <username> <passwd> <passwd>`) with basic validation.
    *   User login (`login <username> <passwd>`) with password verification.
    *   User logout (`logout`).
    *   Listing of all registered users (`list_users`).
    *   User data (username and password) is stored in individual `.user` files within `data/users/`.
*   **Message & Conversation Management:**
    *   Sending messages (`send <recipient> <message>`).
    *   Messages are stored in `.conv` files within `data/conversations/`.
    *   Conversation files are named consistently (e.g., `user1_user2.conv`) by sorting participants' usernames alphabetically.
    *   **"Read" Status for Notifications:** `check_notification` now only displays previously unread messages for the logged-in user. Once displayed, messages are marked as "read" and will not reappear in subsequent `check_notification` calls.
    *   **Timestamps:** All messages are stored with a timestamp, which is displayed in notifications and conversation history.
    *   Viewing full conversation history with a specific user (`check_conve <conv_name>`). All messages in the conversation (read or unread) are displayed with timestamps.
    *   Listing all unique conversation partners for the current user (`list_conv`).
*   **Persistence:** All user registrations and message exchanges are saved to the local filesystem and are reloaded automatically when the application starts.
*   **Modular Codebase:** The project has been refactored into multiple `.c` and `.h` files for better organization and maintainability (`main.c`, `user.c`, `message.c`, `ui.c`, `storage.c`).

## Project Roadmap: Secure Messaging Application (Future Phases)

This section outlines the original development plan, with completed aspects noted and future goals still in scope.

### Phase 1: Core Data Structures and Local User Setup
**Goal:** Establish the fundamental data structures and create a local-only user management system.

1.  **Define Core Structs:**
    *   `struct Message` (`sender`, `recipient`, `content`, `read_status`, `timestamp`) - **COMPLETED & MODIFIED**
    *   `struct User` (`username`, `passwd`) - **COMPLETED & MODIFIED** (Password hashing is pending for future phases, currently plain text for simplicity.)
    *   `struct Conversation` (`cnv_id`, `name`, `participants`, `last_timestamp`) - **PARTIALLY COMPLETED** (Concept of conversation storage is done, but metadata beyond messages is not yet implemented.)

2.  **Implement Local User Creation:**
    *   Develop a `register_user(username, password)` function - **COMPLETED**
    *   This function will:
        *   Create the local directory structure: `data/users/` and `data/conversations/`. - **COMPLETED & MODIFIED** (Structure is `data/users/<username>.user` and `data/conversations/<user1_user2>.conv`)
        *   Calculate and save `passwd_hash` (simple hash for now). - **MODIFIED** (Currently plain text password for simplicity; actual hashing is Phase 6.)
    *   Implement a `login_user(username, password)` function for local authentication. - **COMPLETED**

### Phase 2: Local Message & Conversation Management
**Goal:** Implement functionality to manage conversations and messages on the local client, with local persistence.

1.  **Conversation Metadata:**
    *   Implement functions to read from and write to `meta.dat` files. - **PENDING** (Currently, conversation data is handled directly in `.conv` files.)
2.  **Message Storage:**
    *   Implement functions to append messages to conversation files. - **COMPLETED & MODIFIED** (Messages are stored in `data/conversations/<user1_user2>.conv` files.)
    *   Implemented `check_notification`, `check_conve`, `list_conv` commands. - **COMPLETED**
3.  **Testing Utility:**
    *   The core application itself now serves as the CLI for testing functionalities. - **COMPLETED & MODIFIED**

## Future Phases (Pending)

### Phase 3: Basic TCP Server
**Goal:** Set up a multi-client TCP server that can handle basic connections.

1.  **TCP Listener:**
    *   Implement a TCP socket that listens for incoming client connections.
2.  **Client Handling:**
    *   Use `pthreads` (or another multi-client method) to handle each connected client in a separate thread.
3.  **Server Setup:**
    *   Implement the server-side directory structure: `/server/conversations/`.
    *   Implement the `CONNECT <username>` and `DISCONNECT <username>` commands and log server activity.

### Phase 4: Implementing the Communication Protocol (Plain Text)
**Goal:** Build the core logic for client-server communication.

1.  **Server-Side Implementation:**
    *   `CREATE <cnv_name> <participants>`: Creates a new `<cnv_id>.txt` on the server.
    *   `SEND <cnv_id> <message_line>`: Appends the plain text message line to the corresponding conversation file.
    *   `SYNC <cnv_id> <last_timestamp>`: Reads the appropriate file and sends new messages back to the client using the `BEGIN <count> ... END` format.
    *   **Error Handling:** Implement basic error responses (e.g., `ERROR NOT_FOUND`, `ERROR INVALID_CMD`).

2.  **Client-Side Implementation:**
    *   Develop functions to send `CREATE`, `SEND`, and `SYNC` commands to the server.
    *   Implement logic to parse all server responses, including `CONNECTED OK`, `BEGIN/END` blocks, and error messages.
    *   Integrate the sync logic to update `messages_local.txt` and `meta.dat` after a successful `SYNC`.

### Phase 5: End-to-End Integration and Testing
**Goal:** Create a functional client application and test the full message flow.

1.  **Client Interface:**
    *   Develop a basic interactive CLI for the client.
    *   The interface should allow a user to: log in, view their conversations, send a message, and automatically receive new messages.
2.  **Full System Test:**
    *   Start the server.
    *   Run two separate client instances.
    *   Test creating a conversation, sending messages back and forth, and ensuring both clients are correctly synchronized.

### Phase 6: Encryption Foundation
**Goal:** Replace placeholder hashing with a secure standard and build the key derivation mechanism.

1.  **Secure Hashing:**
    *   Integrate a proven hashing library (e.g., OpenSSL) to implement **SHA256**.
    *   Update the `create_user` and `verify_user` functions to use SHA256 for `passwd_hash` instead of the simple hash.
2.  **Key Derivation:**
    *   Implement a Key Derivation Function (**PBKDF2**) to generate a strong symmetric encryption key from the shared conversation password (`hello123`).

### Phase 7: Message Encryption Layer
**Goal:** Encrypt all message content before transmission and storage.

1.  **Choose Algorithm:**
    *   Select a strong, standard symmetric encryption algorithm (e.g., **AES-256-CBC**).
2.  **Modify `SEND` Logic:**
    *   On the client, before sending, derive the conversation key using the function from Phase 6.
    *   Encrypt the `content` of the message.
    *   Send the message in the format: `user::timestamp::length::encrypted_content`.
3.  **Modify `SYNC` Logic:**
    *   The server will send the encrypted messages as-is.
    *   On the client, after receiving messages, derive the conversation key.
    *   Decrypt the `content` of each new message before displaying it or saving it to the local cache.
4.  **Update Server Storage:**
    *   The server will now store conversations in `<cnv_id>.enc` files to signify that the content is encrypted.

### Phase 8: Final Review and Refinement
**Goal:** Polish the application and review for security and stability.

1.  **Security Review:**
    *   Ensure no sensitive data (passwords, keys) is stored in plain text or hardcoded.
    *   Verify that all dynamically allocated memory is correctly managed to prevent leaks or vulnerabilities.
2.  **Code Cleanup:**
    *   Add comments to complex sections (especially encryption and networking).
    *   Ensure consistent code style and formatting.
3.  **Final Testing:**
    *   Conduct a full end-to-end test of all features, with a focus on the encryption and decryption flow.