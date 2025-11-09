# Simple PostgreSQL-Style Database Architecture

This document describes the architecture for a simple PostgreSQL-style database with MVCC (Multi-Version Concurrency Control) that can be connected to the frontend visualizer.

## Overview

The database is designed with the following key components:

```
┌─────────────────────────────────────────────────────────────┐
│                      Frontend (GTK4 UI)                      │
│                  PostgreSQL MVCC Visualizer                  │
└────────────────────────────┬────────────────────────────────┘
                              │
                              │ DatabaseAPI
                              │ (JSON/HTTP or Direct C++ API)
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      DatabaseAPI Layer                       │
│  - Query execution                                           │
│  - Event callbacks                                           │
│  - Transaction info retrieval                                │
│  - Tuple version queries                                     │
└────────────────────────────┬────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                        Database Core                         │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │ Transaction  │  │    Storage   │  │     Lock     │     │
│  │  Manager     │  │  │   Manager   │  │   Manager   │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│  ┌──────────────┐  ┌──────────────┐                        │
│  │   Catalog    │  │   Vacuum     │                        │
│  │              │  │   Manager    │                        │
│  └──────────────┘  └──────────────┘                        │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      Storage Layer                           │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │  HeapFile    │  │     Page     │  │    Tuple     │     │
│  │  (Tables)    │  │   (Storage)  │  │  (Row Data)  │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
└─────────────────────────────────────────────────────────────┘
```

## Core Components

### 1. Database Core

**Database**
- Main entry point for all database operations
- Coordinates between storage, transactions, and locks
- Provides query execution interface

**StorageManager**
- Manages all heap files (tables)
- Handles page allocation and I/O
- Provides table creation and access

**HeapFile**
- Represents a single table
- Manages pages containing tuples
- Handles tuple insertion, updates, and deletes
- Maintains tuple version chains

**Page**
- Fixed-size storage unit (e.g., 8KB)
- Contains multiple tuple slots
- Tracks free space
- Handles tuple storage within page

**Tuple**
- Represents a single row of data
- Contains values for each column
- Has a header with MVCC metadata (xmin, xmax, ctid)

### 2. Transaction Management

**TransactionManager**
- Assigns unique transaction IDs
- Tracks active transactions
- Manages transaction snapshots
- Handles commit/rollback

**Transaction**
- Represents a single database transaction
- Maintains isolation level
- Tracks operations performed
- Holds snapshot for visibility checks

**Snapshot**
- Defines transaction visibility
- Contains xmin, xmax, and active transaction list
- Used to determine which tuple versions are visible

**Isolation Levels**
- READ_UNCOMMITTED
- READ_COMMITTED
- REPEATABLE_READ
- SERIALIZABLE

### 3. MVCC & Visibility

**TupleHeader**
- xmin: Transaction ID that created this version
- xmax: Transaction ID that deleted/updated this version
- ctid: Pointer to next version (for version chains)
- infomask: Flags for visibility and status

**TupleVersion**
- Represents one version of a row
- Links to next version in chain
- Can be checked for visibility to a transaction

**VisibilityChecker**
- Implements PostgreSQL visibility rules
- Determines if a tuple is visible to a transaction
- Uses snapshot to check active transactions

**VacuumManager**
- Identifies dead tuples (no longer visible to any transaction)
- Removes dead tuples and reclaims space
- Can run automatically or manually

### 4. Lock Management

**LockManager**
- Manages all locks in the system
- Handles lock acquisition and release
- Detects deadlocks
- Maintains wait queues

**Lock**
- Represents a lock on a resource (row, table, etc.)
- Tracks holders and waiters
- Implements lock compatibility rules

**Lock Types**
- ROW_LOCK: Lock on a specific row
- TABLE_LOCK: Lock on entire table
- ADVISORY_LOCK: Application-level lock

**Lock Modes**
- SHARED: Multiple readers can hold
- EXCLUSIVE: Only one writer can hold

### 5. Frontend API

**DatabaseAPI**
- Provides interface for frontend connection
- Can use JSON/HTTP or direct C++ API
- Supports event callbacks for real-time updates
- Provides methods to query:
  - Active transactions
  - Tuple versions
  - Lock information
  - Vacuum status

**EventCallback**
- Notifies frontend of events:
  - Transaction start/commit/rollback
  - Tuple insert/update/delete
  - Lock acquisition/release
  - Deadlock detection

## Key Data Structures

### Transaction ID (TransactionId)
- Unique identifier for each transaction
- Monotonically increasing
- Used in tuple headers (xmin, xmax)

### Tuple ID (TupleId)
- Unique identifier for a tuple
- Consists of (PageId, SlotNumber)
- Used in ctid to chain versions

### Snapshot
```cpp
struct Snapshot {
    TransactionId xmin;  // Oldest active transaction
    TransactionId xmax;  // Next transaction ID to be assigned
    std::set<TransactionId> xip;  // Active transaction IDs
};
```

### Tuple Header
```cpp
struct TupleHeader {
    TransactionId xmin;   // Creating transaction
    TransactionId xmax;   // Deleting/updating transaction (or 0)
    TupleId ctid;         // Next version in chain
    bool deleted;         // Deletion flag
    uint16_t infomask;    // Visibility flags
};
```

## MVCC Visibility Rules

A tuple version is visible to a transaction if:

1. **xmin is committed** AND **xmin < snapshot.xmin** OR **xmin is in snapshot.xip**
2. **xmax is 0** OR **xmax is not committed** OR **xmax > snapshot.xmax** OR **xmax is not in snapshot.xip**

In simpler terms:
- The tuple was created by a committed transaction that started before this transaction
- The tuple has not been deleted, or was deleted by a transaction that hasn't committed yet

## Implementation Phases

### Phase 1: Core Storage
1. Implement Page and Tuple storage
2. Create HeapFile for table management
3. Implement basic insert/read operations

### Phase 2: Transaction Management
1. Implement TransactionManager
2. Add transaction ID assignment
3. Implement basic commit/rollback

### Phase 3: MVCC
1. Add TupleHeader with xmin/xmax
2. Implement version chaining (ctid)
3. Implement VisibilityChecker
4. Add snapshot support

### Phase 4: Locking
1. Implement LockManager
2. Add row-level and table-level locks
3. Implement deadlock detection

### Phase 5: Frontend Integration
1. Create DatabaseAPI
2. Implement event callbacks
3. Add query interface for visualizer
4. Connect to GTK4 frontend

## Usage Example

```cpp
// Create database
Database db("mydb");

// Create table
auto table_id = db.getStorageManager()->createTable("users", schema);

// Begin transaction
auto txn_id = db.beginTransaction(IsolationLevel::READ_COMMITTED);

// Insert tuple
auto tuple = createTuple({1, "John", "john@example.com"});
auto tuple_id = db.getStorageManager()->getTable(table_id)->insertTuple(tuple, txn_id);

// Commit
db.commitTransaction(txn_id);

// Query tuple versions (for visualization)
auto versions = db.getTupleVersions(table_id, key);
for (const auto& version : versions) {
    std::cout << "Version: xmin=" << version.xmin 
              << ", xmax=" << version.xmax 
              << ", visible=" << version.isVisibleTo(current_txn, snapshot) << std::endl;
}
```

## Frontend Connection

The frontend can connect via:

1. **Direct C++ API** (same process)
   - Direct function calls
   - Shared memory
   - Fastest option

2. **JSON/HTTP API** (separate process)
   - RESTful API
   - JSON serialization
   - More flexible, easier to debug

3. **Event Stream**
   - WebSocket or similar
   - Real-time updates
   - Push notifications for transactions

## Next Steps

1. Review the UML diagram (`database_uml_diagram.puml`)
2. Start implementing core storage components
3. Add transaction management
4. Implement MVCC visibility rules
5. Create DatabaseAPI for frontend connection
6. Integrate with existing GTK4 visualizer

