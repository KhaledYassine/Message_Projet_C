# C Messaging App: Code Explanation

This document explains how the C messaging application works, from the project structure to the key concepts used in the code. It's designed to make the codebase easy to understand for beginners.

## 1. Project Structure

The project is organized into several directories and files, each with a specific purpose. This separation makes the code easier to manage.

### Directories
*   `src/`: Contains all the C source code files (`.c`). This is where the program's logic lives.
*   `include/`: Contains all the header files (`.h`). These files declare functions and data structures so they can be shared and used across different `.c` files.
*   `data/`: This directory is created when the program runs. It stores all the user and conversation data.
    *   `data/users/`: Contains a `.user` file for each registered user, storing their password.
    *   `data/conversations/`: Contains a `.conv` file for each conversation between two users, storing the message history.
*   `bin/`: Contains the final compiled executable file (`messaging_app`).
*   `obj/`: Contains intermediate object files (`.o`) created during compilation.

### Source Files (`.c`)
*   `src/main.c`: The entry point of the application. It contains the main `while` loop that keeps the program running.
*   `src/globals.c`: Defines all the global variables used across the application, like the `users` array and the `logged_in` flag.
*   `src/command_handler.c`: Takes the user's menu choice and calls the appropriate function to handle it.
*   `src/interactive.c`: Handles all the multi-step user interactions, like prompting for a username and password during registration.
*   `src/user.c`: Contains the core logic for user management (`register_user`, `login_user`).
*   `src/send.c`: Contains the core logic for sending a message.
*   `src/view.c`: Contains all the logic for displaying information to the user (`check_notification`, `check_conve`, `list_users`, `list_conv`).
*   `src/storage.c`: Handles all reading from and writing to files.
*   `src/ui.c`: Responsible for printing the menus to the screen.
*   `src/visuals.c`: Contains the logic for adding colors to the terminal output.

### Header Files (`.h`)
Each `.h` file in the `include/` directory declares the functions from its corresponding `.c` file, making them available for other parts of the program to use. The `common.h` file is special because it contains data structures (`User`, `Message`) and `extern` declarations for global variables that are used everywhere.

## 2. How It Works: A Walkthrough

Let's trace what happens when a logged-in user chooses **"1. Send Message"**.

1.  **Showing the Menu (`ui.c`)**: The `while` loop in `main.c` calls `show_loggedin_prompt()` from `ui.c` to display the numbered menu.
2.  **Getting Input (`main.c`)**: `main.c` uses `fgets()` to read the user's choice (e.g., "1") from the terminal.
3.  **Handling the Command (`command_handler.c`)**: The input "1" is passed to `handle_command()`. This function converts "1" to an integer and uses a `switch` statement to find the correct action. For choice `1`, it calls `interactive_send()`.
4.  **Interactive Flow (`interactive.c`)**:
    *   `interactive_send()` is called. It first calls another function, `select_user_from_list()`, to display a numbered list of users and get the recipient.
    *   Once the recipient is chosen, `interactive_send()` prompts the user to "Enter your message: ".
    *   Finally, it calls the core `send_message()` function, passing it the recipient and the message content.
5.  **Sending the Message (`send.c`)**:
    *   `send_message()` is called. It creates a `Message` struct, fills it with the sender (the `current_username`), the recipient, the content, a "read" status of `0` (unread), and the current `time(NULL)` as the timestamp.
    *   It then calls `save_message_to_file()`, passing it the new message.
6.  **Saving to File (`storage.c`)**:
    *   `save_message_to_file()` gets the correct, sorted filename for the conversation (e.g., `data/conversations/alice_bob.conv`).
    *   It opens this file in "append" mode (`"a"`) and uses `fprintf()` to write the new message as a new line at the end of the file.

This example shows how the different files, each with a single responsibility, work together to perform a complex action.

## 3. Key Concepts Explained

### Global Variables and `extern` (`globals.c` & `common.h`)
In C, when you split a project into multiple files, variables defined in one file are not automatically visible in others.
*   **Definition (`globals.c`):** We define all our shared variables (like `User users[MAX_USERS];` and `int logged_in;`) in one place: `globals.c`. This is the single source of truth for the application's state.
*   **Declaration (`common.h`):** To make these variables accessible to other files, we declare them with the `extern` keyword in `common.h` (e.g., `extern int logged_in;`). `extern` tells the compiler, "This variable exists, but it's defined in another file. Just trust me, you'll find it when you link everything together."
*   By including `common.h` in our other `.c` files, they all gain access to the same shared global variables.

### Reading and Writing Files (`storage.c`)
File I/O in C generally follows a simple pattern:
1.  **Open:** Use `fopen("filename", "mode")` to open a file. The "mode" is important:
    *   `"r"`: Read (starts from the beginning).
    *   `"w"`: Write (deletes the file if it exists and starts fresh).
    *   `"a"`: Append (adds new content to the end of the file).
2.  **Read/Write:** Use functions like `fprintf()` (to write formatted text), `fgets()` (to read a whole line), or `fscanf()` (to read formatted text).
3.  **Close:** Always use `fclose()` to close the file when you are done. This saves the changes and releases the file.

### Parsing File Content with `strtok` (`storage.c`)
When we read a line from a conversation file (e.g., `"alice bob 0 1670000000 Hello there"`), we need to break it into its parts. `sscanf` can be tricky, so we use a more robust method:
1.  Read the entire line into a `line_buffer` using `fgets()`.
2.  Make a copy of `line_buffer` into `temp_line`. **This is important because `strtok` modifies the string it is parsing.**
3.  Call `strtok(temp_line, " ")` repeatedly. The first call gets the text up to the first space (the sender). Subsequent calls with `NULL` as the first argument (`strtok(NULL, " ")`) continue from where the last one left off.
4.  The final call, `strtok(NULL, "")`, is a small trick. An empty delimiter `""` tells `strtok` to just return the rest of the entire string, which is perfect for our message content that can contain spaces.

### The "Mark as Read" Logic (`view.c`)
The `check_notification` function is one of the most complex. Here is its logic broken down:
1.  First, it finds all unique conversation partners by looking through the messages loaded in memory.
2.  Then, it loops through each partner.
3.  For each partner, it loads the entire conversation history from its `.conv` file into a temporary `conversation_buffer` array.
4.  It loops through this temporary array. If it finds a message that is for the `current_user` and has a `read_status` of `0` (unread), it:
    *   Prints the notification to the screen.
    *   Changes the `read_status` of that message *in the temporary buffer* to `1`.
    *   Sets a flag, `needs_rewrite`, to `true`.
5.  After checking all the messages for that partner, if `needs_rewrite` is `true`, it calls a helper function (`rewrite_conversation_file`) to open the same `.conv` file in write mode (`"w"`), which erases it. It then writes the entire (now modified) `conversation_buffer` back to the file.
This process ensures that the "read" status is permanently saved.
