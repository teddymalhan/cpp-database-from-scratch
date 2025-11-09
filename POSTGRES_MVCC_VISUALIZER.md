# PostgreSQL MVCC Visualizer

## Project Overview

A live, interactive visualizer application that demonstrates how PostgreSQL's Multi-Version Concurrency Control (MVCC) system works and how specific features handle database concurrency. This tool provides real-time visualization of transaction isolation, version management, and concurrency control mechanisms.

## Objectives

- **Educational**: Help developers and database administrators understand PostgreSQL's MVCC internals
- **Interactive**: Provide real-time visualization of concurrent transactions
- **Comprehensive**: Cover key MVCC concepts including transaction IDs, visibility rules, vacuum operations, and isolation levels
- **Modern**: Built with modern C++ (C++17/20) and GTK4 for a native, performant user experience

## MVCC Concepts to Visualize

### Core Concepts

1. **Transaction IDs (XID)**
   - Transaction identifier assignment
   - Transaction ID wraparound
   - Frozen transaction IDs

2. **Tuple Versions**
   - Multiple versions of the same row
   - Tuple header fields (xmin, xmax, ctid)
   - Heap-only tuples (HOT)
   - Visibility of tuple versions

3. **Visibility Rules**
   - Which tuple versions are visible to which transactions
   - Snapshot isolation
   - Read committed vs. Repeatable read vs. Serializable

4. **Concurrency Control**
   - Lock types (row-level, table-level, advisory locks)
   - Deadlock detection and resolution
   - Lock waiting queues
   - Lock escalation

5. **Vacuum Operations**
   - Dead tuple identification
   - Vacuum process visualization
   - Autovacuum triggers
   - Free space map updates

6. **Isolation Levels**
   - Read Uncommitted (PostgreSQL behavior)
   - Read Committed
   - Repeatable Read
   - Serializable

### Advanced Features

7. **Snapshot Isolation**
   - Snapshot creation and management
   - Visibility snapshots across transactions
   - Snapshot conflicts in serializable mode

8. **Write-Ahead Logging (WAL)**
   - WAL record generation
   - Checkpoint operations
   - Replication slot visualization

9. **Index Maintenance**
   - Index tuple visibility
   - Index-only scans
   - Index bloat visualization

## Technology Stack

### Core Technologies

- **C++17/20**: Modern C++ features including:
  - Smart pointers (`std::shared_ptr`, `std::unique_ptr`)
  - RAII principles
  - Move semantics
  - `std::optional`, `std::variant`
  - Coroutines (C++20) for async operations
  - Concepts (C++20) for type safety

- **GTK4**: Modern GUI framework
  - Native look and feel
  - Hardware-accelerated rendering
  - CSS styling support
  - Event-driven architecture
  - Multi-threading support

### Libraries and Dependencies

- **libpqxx** (PostgreSQL C++ client library)
  - Real-time connection to PostgreSQL
  - Query execution and result processing
  - Event-driven notifications

- **Cairo** (via GTK)
  - 2D graphics rendering
  - Custom drawing for visualizations
  - Vector graphics support

- **GObject Introspection** (via GTK)
  - Type system and signals
  - Property bindings

- **Threading**
  - `std::thread` for worker threads
  - `std::mutex`, `std::condition_variable` for synchronization
  - `std::atomic` for lock-free operations

- **JSON** (optional: `nlohmann/json` or `rapidjson`)
  - Configuration file parsing
  - Data serialization

- **CMake**: Build system (already configured)

## Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    GTK4 UI Layer                        │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌─────────┐│
│  │ Main     │  │ Visual   │  │ Control  │  │ Status  ││
│  │ Window   │  │ Canvas   │  │ Panel    │  │ Bar     ││
│  └──────────┘  └──────────┘  └──────────┘  └─────────┘│
└─────────────────────────────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│              Visualization Engine                       │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │ MVCC     │  │ Lock     │  │ Vacuum   │             │
│  │ Renderer │  │ Renderer │  │ Renderer │             │
│  └──────────┘  └──────────┘  └──────────┘             │
└─────────────────────────────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│              Data Collection Layer                      │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐             │
│  │ PG       │  │ Query    │  │ Event    │             │
│  │ Monitor  │  │ Executor │  │ Handler  │             │
│  └──────────┘  └──────────┘  └──────────┘             │
└─────────────────────────────────────────────────────────┘
                        │
                        ▼
┌─────────────────────────────────────────────────────────┐
│              PostgreSQL Database                        │
└─────────────────────────────────────────────────────────┘
```

### Component Design

#### 1. UI Components

**Main Window (`MainWindow`)**
- Application entry point
- Menu bar and toolbar
- Layout management
- Window state persistence

**Visualization Canvas (`VisualizationCanvas`)**
- Custom GTK widget for drawing
- Cairo-based rendering
- Zoom and pan capabilities
- Interactive element selection

**Control Panel (`ControlPanel`)**
- Transaction controls (start, commit, rollback)
- Isolation level selection
- Refresh rate controls
- Filter options

**Status Bar (`StatusBar`)**
- Current transaction information
- Database connection status
- Performance metrics

#### 2. Core Engine Components

**MVCC Renderer (`MvccRenderer`)**
- Tuple version visualization
- Transaction timeline
- Visibility rule rendering
- Color coding for different states

**Lock Renderer (`LockRenderer`)**
- Lock graph visualization
- Wait queue display
- Deadlock highlighting
- Lock type indicators

**Vacuum Renderer (`VacuumRenderer`)**
- Dead tuple visualization
- Vacuum progress
- Free space map display

**Data Model (`DataModel`)**
- Transaction state tracking
- Tuple version storage
- Lock information
- Snapshot management

#### 3. Data Collection

**PostgreSQL Monitor (`PostgresMonitor`)**
- Connection management
- Query execution
- System catalog queries
- Real-time event monitoring

**Query Executor (`QueryExecutor`)**
- Prepared statement management
- Result parsing
- Error handling
- Connection pooling

**Event Handler (`EventHandler`)**
- LISTEN/NOTIFY handling
- Periodic polling
- Event queue management

## Key Features

### 1. Real-Time Transaction Visualization

- **Transaction Timeline**: Visual representation of active transactions
- **Tuple Versions**: Show all versions of a row with their xmin/xmax
- **Visibility**: Highlight which versions are visible to which transactions
- **Color Coding**:
  - Green: Visible tuple
  - Red: Dead tuple
  - Yellow: Invisible to current transaction
  - Blue: Locked tuple

### 2. Interactive Controls

- **Start Transaction**: Begin a new transaction with selected isolation level
- **Execute Query**: Run SELECT/INSERT/UPDATE/DELETE queries
- **Commit/Rollback**: Control transaction lifecycle
- **Step Through**: Animate transaction progression step-by-step
- **Pause/Resume**: Control real-time updates

### 3. Lock Visualization

- **Lock Graph**: Visual representation of lock dependencies
- **Wait Queues**: Show which transactions are waiting for locks
- **Deadlock Detection**: Highlight deadlock cycles
- **Lock Types**: Display different lock types (row, table, advisory)

### 4. Vacuum Operations

- **Dead Tuple Identification**: Show tuples marked for deletion
- **Vacuum Progress**: Real-time vacuum operation visualization
- **Free Space**: Display reclaimed space
- **Autovacuum Triggers**: Show when autovacuum is triggered

### 5. Isolation Level Comparison

- **Side-by-Side View**: Compare behavior across isolation levels
- **Anomaly Demonstration**: Show dirty reads, phantom reads, etc.
- **Snapshot Comparison**: Visualize different snapshots

### 6. Performance Metrics

- **Transaction Duration**: Track transaction execution time
- **Lock Wait Time**: Monitor lock contention
- **Tuple Count**: Track tuple versions per row
- **Database Size**: Monitor table and index sizes

## Implementation Plan

### Phase 1: Foundation (Weeks 1-2)

1. **Project Setup**
   - Update CMakeLists.txt for GTK4 and libpqxx
   - Configure dependencies (vcpkg or Conan)
   - Set up C++17/20 standard
   - Create basic project structure

2. **Core Infrastructure**
   - PostgreSQL connection management
   - Basic GTK4 window setup
   - Event loop integration
   - Logging framework

3. **Data Collection**
   - Implement PostgreSQL monitor
   - Query system catalogs (pg_class, pg_attribute, pg_stat_user_tables)
   - Transaction ID queries
   - Basic tuple version retrieval

### Phase 2: Basic Visualization (Weeks 3-4)

1. **Canvas Widget**
   - Custom GTK widget for drawing
   - Cairo integration
   - Basic rendering pipeline
   - Coordinate system setup

2. **MVCC Renderer**
   - Tuple version drawing
   - Transaction timeline
   - Basic visibility rules
   - Color scheme implementation

3. **UI Controls**
   - Control panel implementation
   - Transaction controls
   - Isolation level selector
   - Basic status display

### Phase 3: Advanced Features (Weeks 5-6)

1. **Lock Visualization**
   - Lock graph rendering
   - Wait queue display
   - Deadlock detection visualization
   - Lock type indicators

2. **Vacuum Visualization**
   - Dead tuple identification
   - Vacuum progress display
   - Free space visualization

3. **Interactive Features**
   - Click-to-inspect tuples
   - Zoom and pan
   - Tooltips with detailed information
   - Selection highlighting

### Phase 4: Polish and Optimization (Weeks 7-8)

1. **Performance**
   - Optimize rendering pipeline
   - Implement caching
   - Reduce database query overhead
   - Multi-threading for data collection

2. **User Experience**
   - Improve UI/UX
   - Add keyboard shortcuts
   - Configuration persistence
   - Export capabilities (screenshots, data)

3. **Documentation**
   - User manual
   - Code documentation
   - Example scenarios
   - Tutorial videos

## Database Schema Requirements

### Test Database Setup

```sql
-- Create test table
CREATE TABLE test_table (
    id SERIAL PRIMARY KEY,
    value INTEGER,
    description TEXT
);

-- Enable necessary extensions
CREATE EXTENSION IF NOT EXISTS pg_stat_statements;

-- Create indexes for demonstration
CREATE INDEX idx_test_table_value ON test_table(value);
```

### Required PostgreSQL Permissions

- `SELECT` on system catalogs
- `SELECT` on `pg_stat_*` views
- `SELECT` on `pg_locks`
- `SELECT` on `pg_stat_activity`
- Ability to create test tables and transactions

## CMake Configuration

### Required Dependencies

```cmake
# GTK4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# PostgreSQL
find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)

# Threading
find_package(Threads REQUIRED)
```

### Build Configuration

```cmake
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GTK4_LIBRARIES
        PostgreSQL::PostgreSQL
        pqxx::pqxx
        Threads::Threads
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GTK4_INCLUDE_DIRS}
)
```

## Modern C++ Features Usage

### Smart Pointers
```cpp
class PostgresMonitor {
    std::unique_ptr<pqxx::connection> connection_;
    std::shared_ptr<DataModel> data_model_;
};
```

### RAII
```cpp
class TransactionGuard {
    pqxx::transaction_base& txn_;
public:
    TransactionGuard(pqxx::transaction_base& txn) : txn_(txn) {}
    ~TransactionGuard() { if (!committed_) txn_.abort(); }
};
```

### Move Semantics
```cpp
class TupleVersion {
    TupleVersion(TupleVersion&&) = default;
    TupleVersion& operator=(TupleVersion&&) = default;
};
```

### Optional and Variant
```cpp
std::optional<TransactionId> get_xmin();
std::variant<Visible, Invisible, Dead> get_visibility();
```

### Coroutines (C++20)
```cpp
std::generator<TupleVersion> enumerate_tuples() {
    // Async tuple enumeration
}
```

## GTK4 Integration

### Widget Hierarchy

```
GtkApplication
└── GtkApplicationWindow
    ├── GtkBox (vertical)
    │   ├── GtkHeaderBar
    │   ├── GtkPaned (horizontal)
    │   │   ├── GtkBox (left panel)
    │   │   │   ├── ControlPanel
    │   │   │   └── StatusBar
    │   │   └── VisualizationCanvas (custom widget)
    │   └── GtkStatusBar
```

### Signal Handling

```cpp
class MainWindow : public Gtk::ApplicationWindow {
    void on_transaction_start();
    void on_isolation_level_changed();
    void on_refresh_clicked();
};
```

### Custom Widget

```cpp
class VisualizationCanvas : public Gtk::Widget {
protected:
    void snapshot_vfunc(Gtk::Snapshot& snapshot) override;
    void measure_vfunc(Gtk::Orientation orientation, 
                       int& minimum, int& natural, 
                       int& minimum_baseline, int& natural_baseline) override;
};
```

## Performance Considerations

1. **Rendering Optimization**
   - Use Cairo's surface caching
   - Implement dirty region tracking
   - Reduce redraw frequency
   - Use hardware acceleration

2. **Database Query Optimization**
   - Batch queries where possible
   - Use prepared statements
   - Implement query result caching
   - Limit result set sizes

3. **Memory Management**
   - Use object pools for frequently allocated objects
   - Implement efficient data structures
   - Monitor memory usage
   - Clean up old transaction data

4. **Threading**
   - Separate UI thread from data collection thread
   - Use message queues for thread communication
   - Minimize lock contention
   - Use lock-free data structures where possible

## Testing Strategy

1. **Unit Tests**
   - Data model tests
   - Visibility rule tests
   - Query executor tests

2. **Integration Tests**
   - PostgreSQL connection tests
   - End-to-end transaction tests
   - Rendering tests

3. **Manual Testing**
   - Various isolation level scenarios
   - Concurrent transaction scenarios
   - Vacuum operation scenarios
   - Deadlock scenarios

## Future Enhancements

1. **Multi-Database Support**
   - Support for other databases with MVCC
   - Comparison mode

2. **Replay Functionality**
   - Record transaction sequences
   - Replay for demonstration
   - Export/import scenarios

3. **Advanced Analytics**
   - Performance profiling
   - Bottleneck identification
   - Optimization suggestions

4. **Web Interface**
   - Optional web-based UI
   - Remote monitoring capabilities

## Resources

- PostgreSQL MVCC Documentation
- GTK4 Documentation
- libpqxx Documentation
- PostgreSQL Internals Books
- MVCC Research Papers

## License

[To be determined - align with project license]

