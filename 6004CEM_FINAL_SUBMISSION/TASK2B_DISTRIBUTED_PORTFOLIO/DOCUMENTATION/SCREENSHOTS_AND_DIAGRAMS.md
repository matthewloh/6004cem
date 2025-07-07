# Screenshot and Diagram Guide

## 📸 Part A: MPI Hello World Enhanced

### Key Screenshots to Capture

- **Program Execution Output**:
  - Terminal showing the full program run with `mpirun -np 4`.
  - Shows cluster verification info, process mapping, and system details.
- **Process Mapping Section**:
  - The part showing each process running on different CPU cores.
  - Demonstrates MPI process-to-processor mapping.

### Key Code Sections

- **MPI Initialization and Communication**

### Recommended Diagrams

- Process Flow Diagram showing 4 processes executing in parallel.
- System Architecture showing how processes map to CPU cores.

```mermaid
graph TD
    subgraph "MPI Program Flow"
        direction LR
        A[Start] --> B{MPI_Init};
        
        subgraph "Parallel Execution (4 Processes)"
            direction TB
            P0["Process 0"]
            P1["Process 1"]
            P2["Process 2"]
            P3["..."]
            
            P0 --> P0_Work["enhancedMPIHello()"];
            P1 --> P1_Work["enhancedMPIHello()"];
            P2 --> P2_Work["enhancedMPIHello()"];
            P3 --> P3_Work["enhancedMPIHello()"];
        end
        
        B --> P0;
        B --> P1;
        B --> P2;
        B --> P3;

        P0_Work --> C{MPI_Barrier};
        P1_Work --> C;
        P2_Work --> C;
        P3_Work --> C;

        C --> D{MPI_Finalize};
        D --> E[End];
    end
```

```mermaid
graph TD
    subgraph "System Architecture Example"
        subgraph "Physical Node ('hostname')"
            direction TB
            CPU1["CPU Core 1"]
            CPU2["CPU Core 2"]
            CPU3["CPU Core 3"]
            CPU4["CPU Core 4"]
        end

        subgraph "MPI Processes"
            direction TB
            P0["Process 0 (rank 0)"]
            P1["Process 1 (rank 1)"]
            P2["Process 2 (rank 2)"]
            P3["Process 3 (rank 3)"]
        end

        P0 -->|mapped to| CPU1;
        P1 -->|mapped to| CPU2;
        P2 -->|mapped to| CPU3;
        P3 -->|mapped to| CPU4;

        note["Note: The OS scheduler and MPI runtime<br/>determine the exact process-to-core mapping,<br/>which may vary between runs."]
    end
```

---

## 📸 Part B: MPI Master-Slave Enhanced

### Key Screenshots to Capture

- **Master-Slave Communication Output**:
  - Basic communication where slaves send "Hello back".
  - Personalized communication with different names.
  - Enhanced structured messages.
- **Core Communication Code**
- **Slave Sending Code**
- **Enhanced Message Structure**

### Recommended Diagrams

- Master-Slave Architecture showing 1 master + 3 slaves.
- Communication Flow showing bidirectional message passing.
- Message Structure showing the `EnhancedMessage` class fields.

```mermaid
graph TD
    Master["Master (Rank 0)"]
    
    subgraph "Slave Processes"
        direction LR
        Slave1["Slave (Rank 1)"]
        Slave2["Slave (Rank 2)"]
        Slave3["Slave (Rank 3)"]
        SlaveN["..."]
    end

    Master -- "Sends work/requests" --> Slave1;
    Master -- "Sends work/requests" --> Slave2;
    Master -- "Sends work/requests" --> Slave3;
    Master -- "..." --> SlaveN

    Slave1 -- "Sends results/messages" --> Master;
    Slave2 -- "Sends results/messages" --> Master;
    Slave3 -- "Sends results/messages" --> Master;
    SlaveN -- "..." --> Master;

    style Master fill:#cde4ff
```

```mermaid
sequenceDiagram
    participant M as Master (Rank 0)
    participant S1 as Slave 1
    participant S2 as Slave 2
    participant S3 as Slave ...

    loop For each Slave (i = 1 to N)
        M->>M: Posts MPI_Recv(source=i)
    end

    S1->>M: MPI_Send(message, dest=0)
    S2->>M: MPI_Send(message, dest=0)
    S3->>M: MPI_Send(message, dest=0)
    
    loop For each Slave
        M-->>M: Receives and prints message
    end
```

```mermaid
classDiagram
    class EnhancedMessage {
        +int sender_rank
        +int message_id
        +double timestamp
        +string message_text
        +string sender_name
        +int work_units_completed
    }
```

---

## 📸 Part C: MPI Message Tags Enhanced

### Key Screenshots to Capture

- **Working Tag Communication (100↔100)**:
  - Terminal output showing successful message exchange.
  - All processes sending/receiving with matching tags.
- **Hanging Tag Communication (100→101)**:
  - The explanation text about why it hangs.
  - Timeout mechanism preventing infinite hang.
- **Tag Constants and Structure**
- **Tag Mismatch Explanation Code**
- **Advanced Tag Usage**:
  - Priority system with `HIGH_PRIORITY_TAG` and `LOW_PRIORITY_TAG`.
  - `MPI_ANY_TAG` wildcard demonstration.

### Recommended Diagrams

- Tag Matching Matrix showing working vs. hanging scenarios.
- Message Queue Visualization showing how unmatched messages accumulate.
- Tag-based Message Routing showing different message types.

```mermaid
graph TD
    subgraph "Tag Matching Logic"
        A{Start} --> B{Master Sends Msg w/ Tag 100};
        B --> C{Slave Posts Receive};
        C --> D{Receive Tag == 100?};
        D -- Yes --> E[SUCCESS<br/>Message Received];
        D -- No (e.g., Tag 101) --> F[HANG / DEADLOCK<br/>Message remains in queue, unseen];
        E --> G{End};
        F --> G;
    end
    
    style E fill:#d4edda,stroke:#155724
    style F fill:#f8d7da,stroke:#721c24
```

```mermaid
sequenceDiagram
    participant Master
    participant Slave
    participant MPI_Queue as "Slave's Message Queue"

    Master->>+Slave: MPI_Send(Message, tag=100)
    Note over Master,Slave: Master sends message with Tag 100
    
    Slave->>MPI_Queue: Message(tag=100) arrives
    Note right of MPI_Queue: Message is stored in<br/>the MPI internal queue.
    
    Slave->>+Slave: MPI_Recv(source=Master, tag=101)
    Note over Slave: Slave is now blocked,<br/>waiting ONLY for a message with Tag 101.
    
    Slave-->>MPI_Queue: Search for message with tag=101...
    MPI_Queue-->>Slave: Not Found!
    
    loop Indefinite Wait (Hang)
        Slave-->>MPI_Queue: Search for message with tag=101...
        MPI_Queue-->>Slave: Not Found!
    end
```

```mermaid
sequenceDiagram
    participant Master
    participant Slave

    Master->>Slave: MPI_Send(Data, tag=DATA_TAG)
    Master->>Slave: MPI_Send(Command, tag=CONTROL_TAG)
    Master->>Slave: MPI_Send(Status Request, tag=STATUS_TAG)

    Slave->>Slave: MPI_Recv(tag=DATA_TAG)
    Note right of Slave: Processes Data
    
    Slave->>Slave: MPI_Recv(tag=CONTROL_TAG)
    Note right of Slave: Executes Command
    
    Slave->>Slave: MPI_Recv(tag=STATUS_TAG)
    Note right of Slave: Returns Status
```

---

## 📊 Summary Tables

### 1. Program Comparison

| Feature              | Part A                           | Part B                     | Part C                      |
| -------------------- | -------------------------------- | -------------------------- | --------------------------- |
| **Purpose**          | Hello World + Process Mapping    | Master-Slave Communication | Message Tag Demonstration   |
| **Processes**        | All equal (SPMD)                 | 1 Master + N Slaves        | 1 Master + N Slaves         |
| **Communication**    | Collective (Broadcast, Reduce)   | Point-to-Point (Send/Recv) | Tagged Point-to-Point       |
| **Key MPI Features** | Process topology, Collective ops | Master-slave pattern       | Message tags, Deadlock demo |

### 2. MPI Communication Patterns

| Pattern             | Used In   | Description               | Code Example                  |
| ------------------- | --------- | ------------------------- | ----------------------------- |
| **Collective**      | Part A    | All processes participate | `MPI_Bcast()`, `MPI_Reduce()` |
| **Point-to-Point**  | Part B, C | Direct process-to-process | `MPI_Send()`, `MPI_Recv()`    |
| **Tagged Messages** | Part C    | Message filtering by tag  | `MPI_Send(..., tag, ...)`     |

### 3. C++ Features Used

| Feature                | Part A             | Part B             | Part C             | Benefit                |
| ---------------------- | ------------------ | ------------------ | ------------------ | ---------------------- |
| **Classes**            | `✓ MPIHelloWorld`  | `✓ MPIMasterSlave` | `✓ MPIMessageTags` | Code organization      |
| **STL Containers**     | `✓ vector, string` | `✓ vector, string` | `✓ string`         | Memory management      |
| **Exception Handling** | `✓ try-catch`      | `✓ try-catch`      | `✓ try-catch`      | Error handling         |
| **`constexpr`**        | `✓`                | `✓`                | `✓`                | Compile-time constants |

---

## 🎯 Specific Screenshot Recommendations

### For Academic Reports

- **Split-screen shots showing**:
  - Code on left, execution output on right.
  - Before/after comparisons (C vs C++).
- **Annotated code snippets with**:
  - Arrows pointing to key MPI functions.
  - Colored highlighting of important sections.
  - Comments explaining the flow.
- **Terminal outputs showing**:
  - Full program execution from start to finish.
  - Process IDs and CPU core assignments.
  - Timing information and performance metrics.

### For Presentations

- High-level architecture diagrams.
- Code flow charts showing decision points.
- Communication sequence diagrams.
- Performance comparison charts.

---

_This document provides a guide for what to capture. Feel free to ask for help creating any specific diagrams or identifying exact line numbers for screenshots._
