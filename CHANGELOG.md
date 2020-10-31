# CHANGELOG.md

# 0.2.0 (10/31/2020)
## What's new
- Added ability to memorize multiple verses at once

## Changes
- MemorizeWidget now uses a QTextEdit-derived widget
  - MemorizeEdit automatically focuses
- MemorizeWidget's status label dropped in favor of the status bar
- Added CI for Linux and macOS

# 0.1.0 (10/30/2020)
## What's new
- Switched MainWindow from QWidget to QMainWindow
  - Added File menu with Exit command
  - Added Help menu with About and About Qt commands
  - Added a status bar (doesn't display much at the moment, though!)
- Added option to stop memorization exercise

## Changes
- The MemorizeWidget no longer expands horizontally as you type the verse
- The delay before the Bible data is freed is now 30 seconds instead of 3 minutes

# 0.0.1 (10/28/2020)
## What's new
- Everything. This release is simply the bare minimum required to memorize a Bible verse.