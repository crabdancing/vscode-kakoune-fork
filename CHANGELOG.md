# Change Log

## [1.1.1]

**Bug fixes**

- Fix: `s` now correctly handles multiline selections.

## [1.1.0]

**New features**

- Add: You can now use `a` to enter insert mode after a selection.
- Add: You can now use `I` to enter insert mode at the start of a line.
- Add: You can now use `o` to add a new line after your current one and enter insert mode.
- Add: You can now use `O` to add a new line before your current line and enter insert mode.
- Add: Regex search through the whole document with `/`.
- Add: Regex search through current selections with `s`.
- Add: `<esc>` can now be used to go back to normal mode.
- Add: Use `<ctrl>+u` and `<ctrl>+d` to scroll up and down respectively.
- Add: Increase indentation using `>`.
- Add: Decrease indentation using `<`.
- Add: Cursor style is now a line in insert mode, block otherwise.

**Bug fixes**

- Fix: Deleting via `d` now works like Kakoune.
- Fix: Pressing `c` to enter insert mode without a selection now correctly deletes the character under the cursor.
- Fix: Multiple selections no longer disappear when using `w`/`b`.
- Fix: Pasting via `p`/`P` now pastes after/before the current selection respectively.
- Fix: If you don't press 'k' after 'j' while in insert mode, the 'j' is now inserted.

## [1.0.1]

- Use webpack to optimize bundle size.

## [1.0.0]

- Initial release
- Has support for basic movement and selections.
- Has support for basic goto commands and selections.
- Supports multiple ways to enter insert mode.
- Supports exiting insert mode by using `jk`
- Support basic clipboard actions, i.e. copy/cut/paste via `y`/`d`/`p`.
