# Multithreaded Watchdog System

This project implements a multithreaded watchdog system designed to monitor critical processes, ensuring their health and reviving them in case of failures. The system leverages signal handling, inter-process communication (IPC), and a custom scheduler to handle process monitoring efficiently.

## Features

- **Process Monitoring**: Tracks the health of critical process and detects failures.
- **Automatic Recovery**: Automatically revives the process in the event of a crash or unresponsiveness.
- **Multithreading**: Efficient use of threads for concurrent monitoring.
- **Signal Handling**: Uses signals for inter-process communication and failure detection.
- **Custom Scheduler**: Controls the timing of health checks and recoveries.
  
## Technologies Used

- **C Programming Language**: Core system implementation.
- **Multithreading**: Ensures efficient, concurrent monitoring of a process.
- **Signal Handling**: Uses signals to manage process failures and communicate with the watchdog.
- **Inter-Process Communication (IPC)**: Allows communication between the watchdog and the monitored process.
- **Scheduler**: Custom timing logic for monitoring processes at regular intervals.
- **Git**: Version control for tracking the progress of development.
- **Testing**: Comprehensive testing to ensure the robustness and reliability of the watchdog system.

## Getting Started

### Prerequisites

- **GCC** or any C compiler supporting multithreading and signal handling.
- Basic understanding of multithreading and IPC in C.

### Installation

Clone the repository:

    ```bash
    git clone https://github.com/yiratb/WatchDog.git
    cd Watchdog
    ```

### Compiling and running the System

To compile the watchdog, execute: ```make all```

to run, excecute: ```make run```

to clean, excecute: ```make clean```