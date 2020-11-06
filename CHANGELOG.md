## 0.4.1 (11/6/2020)
### What's new
- Changelog view added to Help menu
- Added maddy library for Markdown support (used in rendering CHANGELOG.md)
- Added "About maddy" to Help menu

### Changes
- CHANGELOG.md headers taken one level smaller
- Title removed from CHANGELOG.md

## 0.4.0 (11/5/2020)
### What's new
- Added settings interface
- Display verse and Memorize verse buttons can now be swapped
- Choose between last verse, random verse, or a set verse to load at startup
- Added icon
- Added version info to qmake
- Added option to mark word as wrong and continue if the wrong key is typed

### Changes
- Removed help label in MemorizeWidget in favor of placeholder text in MemorizeEdit
- Tab now changes focus in MemorizeWidget
- Focus changes to "continue" button after the verse has been completely entered
- Qt now handles text navigation events in MemorizeEdit

### Bugfixes
- Buttons in the settings and MemorizeWidget no longer are stretched

## 0.3.0 (11/2/2020)
### What's new
- Added [GitHub Pages](https://lorendb.github.io)
- Window centers in screen at startup
- On startup, the window size is restored to the last set size
- On startup, the last verse selected is displayed
- Verse combos now in format "Book Chapter:StartVerse-EndVerse"
- Added global application/organization info (with QCoreApplication)

### Changes
- The verse display box is now a read-only QTextEdit
- The memorize dialog no longer automatically closes; instead, press a button to continue
- The MemorizeEdit now ignores keys such as Alt, Control, and Page Up
- Arrow keys now work as expected in MemorizeEdit
- The Bible now waits 1 minute before freeing data (instead of 30 seconds)

### Bugfixes
- The window width now remains constant when displaying a verse

## 0.2.0 (10/31/2020)
### What's new
- Added ability to memorize multiple verses at once

### Changes
- MemorizeWidget now uses a QTextEdit-derived widget
  - MemorizeEdit automatically focuses
- MemorizeWidget's status label dropped in favor of the status bar
- Added CI for Linux and macOS

## 0.1.0 (10/30/2020)
### What's new
- Switched MainWindow from QWidget to QMainWindow
  - Added File menu with Exit command
  - Added Help menu with About and About Qt commands
  - Added a status bar (doesn't display much at the moment, though!)
- Added option to stop memorization exercise

### Changes
- The MemorizeWidget no longer expands horizontally as you type the verse
- The delay before the Bible data is freed is now 30 seconds instead of 3 minutes

## 0.0.1 (10/28/2020)
### What's new
- Everything. This release is simply the bare minimum required to memorize a Bible verse.