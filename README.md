# ZTOS
Project to learn and develop a small RTOS.

Currently only supports - ARM Cortex M3

## Getting Started

### Prerequisites

Install the following tools to compile and program STM32 devices via STLink:

```bash
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch openocd stlink-tools make
```

## Project Structure

```
ZTOS/
├── linker.ld
├── Makefile
├── README.md
├── app/
│   ├── include/
│   └── src/
├── build/
│   ├── app/
│   ├── drivers/
│   ├── HAL/
│   └── startup/
├── docs/
├── drivers/
│   ├── include/
│   └── src/
├── HAL/
│   ├── include/
│   └── src/
├── startup/
│   ├── include/
│   └── src/
└── ZTOS
    ├── include/
    └── src/
```

## Roadmap

- [ ] Initialize System Clock and MCU Configuration
- [ ] Set up Hardware Timers for Tick Generation
- [ ] Implement Tick Interrupt Handler
- [ ] Create Task Control Block (TCB) Structure
- [ ] Implement Context Switching Mechanism
- [ ] Develop Task Scheduler (Round Robin / Priority-based)
- [ ] Add Task Creation and Deletion Functions
- [ ] Implement Task States (Ready, Running, Blocked, Suspended)
- [ ] Create Synchronization Primitives (Semaphores)
- [ ] Implement Mutexes for Resource Protection
- [ ] Add Intertask Communication (Message Queues)
- [ ] Develop Memory Management (Heap/Stack Allocation)
- [ ] Add Task Priority Management
- [ ] Implement System Time and Delay Functions
- [ ] Add Error Handling and Exception Management
- [ ] Create Debugging and Logging Utilities

---

### Build

To build use the following:
```bash
make
```
This will create the object files and the .elf file. Additionally, dependency and .map files are generated.

### Flash

To flash to the MCU use the following:
```bash
make flash
```

### Clean

To clear all object and elf files:
```bash
make clean
```

### Documentation

Refer to the project documentation for system clarification.