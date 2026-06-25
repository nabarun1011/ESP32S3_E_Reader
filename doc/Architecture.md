# E-Reader Architecture

## Vision

The goal of this project is to build a modular, extensible, and efficient e-reader for ESP32-S3 with an e-paper display.

The architecture is designed so that new document formats, UI screens, storage backends, and hardware features can be added with minimal changes to existing code.

The guiding principles are:

* Single Responsibility Principle
* Composition over duplication
* Platform-independent domain logic
* Low RAM usage
* Fast UI response
* Future-proof design

---

# Project Structure

```
Core/
Domain/
Interfaces/
Platform/
UI/
Utils/
```

---

# Layer Responsibilities

## Interfaces

Defines contracts between components.

Interfaces contain no hardware-specific code and no application logic.

Examples:

```
IBookSettingsRepository
IDisplay
IStorage
IFile
IDocument
IScreen

```

Everything outside Platform should depend on interfaces instead of concrete implementations.

---

## Platform

Contains hardware-specific implementations.

Examples:

```
EPDDisplay
SDStorage
Buttons
USB MSC
Battery
RTC
WiFi
```

Only this layer should know about ESP32 libraries.

Changing display hardware or storage backend should only require replacing Platform implementations.

---

## Domain

Contains the application's business logic.

Examples:

```
TextDocument
TextRenderer
BookSettingsRepository
DocumentFactory
```

The Domain layer should never know about buttons, GPIO, SPI, or display drivers.

---

## UI

Responsible for presentation and user interaction.

Examples:

```
LibraryScreen
ReaderScreen
SettingsScreen
ScreenManager
```

Screens receive user input and render data.

They do not contain storage or rendering algorithms.

---

## Core

Coordinates the application.

```
App
```

Responsibilities:

* Initialize components
* Connect dependencies
* Main loop
* Screen transitions

---

## Utils

Reusable helper classes.

Examples:

```
NaturalSort
String utilities
Math helpers
```

Utilities should remain generic and reusable.

---

# Reader Pipeline

```
Book File
    │
    ▼
TextDocument
    │
ReadAll()
    │
    ▼
String
    │
WrapDocument()
    │
    ▼
WrappedLine[]
    │
RebuildPageIndex()
    │
    ▼
PageInfo[]
    │
BuildPageFromLines()
    │
    ▼
Display
```

---

# Why WrappedLine Exists

Rendering performs word wrapping only once.

Each wrapped line stores:

```
Text
CharacterOffset
```

Benefits:

* Fast page changes
* Fast redraws
* Reading progress tracking
* Font and orientation changes only require rebuilding the wrapped document

---

# Why PageInfo Exists

A rendered page is temporary.

The application only needs to remember where each page begins.

Each PageInfo contains:

```
StartLine
CharacterOffset
```

Benefits:

* Resume reading
* Reading progress
* Go to page
* Bookmarks
* Efficient page navigation

---

# Document Model

Each document format implements:

```
IDocument
```

Examples:

```
TextDocument
MarkdownDocument
EpubDocument
```

ReaderScreen never needs to know which format is being displayed.

DocumentFactory chooses the correct implementation.

---

# Screen Layout

Every screen derives its layout from a common ScreenLayout.

```
Header
Content
Footer
```

Benefits:

* Consistent UI
* Easy addition of battery, clock, title bars
* Shared rendering logic

Each screen may extend this layout with additional information.

Example:

Library

```
ItemWidth
ItemHeight
Columns
```

Reader

```
Content area
Footer
```

---

# Library Design

The library is independent of how books are displayed.

Supported views:

* List
* Grid
* Book Covers

The scrollbar and navigation operate on visible items rather than rows.

This allows the same navigation code to work across all view modes.

---

# Settings

Settings are separated by responsibility.

## DeviceSettings

Device-wide configuration.

Examples:

* Orientation
* Theme
* UI font
* Sleep timeout

---

## LibrarySettings

Library behavior.

Examples:

* Sort order
* View mode
* Show hidden files

---

## BookSettings

Per-book configuration.

Examples:

* Font
* Font size
* Last reading position
* Bookmarks (future)

---

# Performance Philosophy

Operations that are expensive should happen once.

Examples:

* Word wrapping
* Page indexing
* Natural sorting

Operations that occur frequently should be lightweight.

Examples:

* Page turn
* Screen redraw
* Cursor movement
* Menu navigation

---

# Memory Philosophy

Avoid duplicated data.

The renderer stores wrapped lines once.

Pages are rebuilt from wrapped lines instead of caching page contents.

Metadata should be lightweight.

Example:

```
PageInfo
```

instead of storing every rendered page.

---

# Future Features

Reader

* Bookmarks
* Search
* Dictionary
* Notes
* Highlighting
* TOC

Library

* Collections
* Recent Books
* Covers
* Search
* Filters

Documents

* Markdown
* EPUB
* PDF (if practical)

Device

* Battery indicator
* Clock
* USB MSC
* OTA
* WiFi synchronization
* Light Sleep
* Deep Sleep

---

# Design Philosophy

Every component should have one clear responsibility.

Prefer extending the system rather than modifying existing code.

A new feature should usually involve adding a new class instead of rewriting old ones.

The architecture should remain understandable even after years of development.
