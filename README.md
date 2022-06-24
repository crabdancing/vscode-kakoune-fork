# Kakoune mode

[![builds.sr.ht status](https://builds.sr.ht/~reykjalin/vscode-kakoune.svg)](https://builds.sr.ht/~reykjalin/vscode-kakoune?)

_Please send bug reports and feature requests to the [mailing list](https://lists.sr.ht/~reykjalin/vscode-kakoune) via [~reykjalin/vscode-kakoune@lists.sr.ht](mailto:~reykjalin/vscode-kakoune@lists.sr.ht)._

`vscode-kakoune` emulates the input behavior of the [Kakoune](https://kakoune.org) text editor.

# Table of Contents

- [Keymappings](#keymappings)
  - [Enter insert mode](#enter-insert-mode)
  - [Movements and selections](#movements-and-selections)
  - [Goto mode](#goto-mode)
  - [Editing the file](#editing-the-file)
  - [Searching for things to select](#searching-for-things-to-select)
- [Differences between Kakoune and this plugin](#differences-between-kakoune-and-this-plugin)
- [Motivation](#motivation)
- [Why Reason?](#why-reason)
- [Build Instructions](#build-instructions)
  - [Script to paste into your shell of choice](#script-to-paste-into-your-shell-of-choice)
- [Current functionality](#current-functionality)
- [Upcoming functionality](#upcoming-functionality)

# Keymappings

This plugin emulates modal editing, and will start in normal mode.
You can not type in normal mode, until you [enter insert mode](#enter-insert-mode)

## Enter insert mode

| Key | Effect                                                                                               |
| --: | :--------------------------------------------------------------------------------------------------- |
| `i` | Moves the cursor to the beginning of the selection and enters insert mode                            |
| `I` | Moves the cursor to the start of the line and enters insert mode                                     |
| `a` | Moves the cursor to the end of the selection and enters insert mode                                  |
| `A` | Moves the cursor to the end of the line and enters insert mode                                       |
| `o` | Creates a new empty line below the current selection, moves the cursor there, and enters insert mode |
| `O` | Creates a new empty line above the current selection, moves the cursor there, and enters insert mode |
| `r` | Enters insert mode without clearing the current selections                                           |
| `c` | Clears the current selection and enters insert mode                                                  |

## Movements and selections

|       Key | Effect                                                                                                                          |
| --------: | :------------------------------------------------------------------------------------------------------------------------------ |
|       `w` | Select the word to the right                                                                                                    |
|       `W` | Extend selection one word to the right                                                                                          |
|       `b` | Select the word to the left                                                                                                     |
|       `B` | Extend selection one word to the left                                                                                           |
|       `h` | Move caret left                                                                                                                 |
|       `H` | Extend selection one character to the right                                                                                     |
|       `j` | Move caret down                                                                                                                 |
|       `J` | Extend selection to the character below the caret                                                                               |
|       `k` | Move caret up                                                                                                                   |
|       `K` | Extend selection to the character above the caret                                                                               |
|       `l` | Move caret right                                                                                                                |
|       `L` | Extend selection one character to the left                                                                                      |
|       `x` | Select the line the current selection is in. If a full line is currently selected, selects the line below the current selection |
|       `X` | Extend the selection to include the line below the current selection                                                            |
|       `C` | Extend the selection one line down                                                                                              |
|       `g` | Enter [goto mode](#goto-mode)                                                                                                   |
| `<space>` | Cancel all selections except the primary selection                                                                              |
|       `;` | Reduce all selections to just the cursor                                                                                        |
|       `f` | Move the selection from the cursor on to the next instance of the provided character                                            |
|       `F` | Extend the selection from the cursor on to the next instance of the provided character                                          |
|       `t` | Move the selection from the cursor to the next instance of the provided character                                               |
|       `T` | Extend the selection from the cursor to the next instance of the provided character                                             |

## Goto mode

When in goto mode you can quickly jump around the file.
Pressing any button (even if it's not part of goto mode) will put you back in normal mode.

| Key | Effect                                     |
| --: | :----------------------------------------- |
| `h` | Move caret to the start of the line        |
| `H` | Extend selection to the start of the line  |
| `j` | Move caret to the bottom of the file       |
| `J` | Extend selection to the bottom of the file |
| `k` | Move caret to the start of the file        |
| `K` | Extend selection to the start of the file  |
| `l` | Move caret to the end of the line          |
| `L` | Extend selection to the end of the line    |

## Editing the file

| Key | Effect                                                 |
| --: | :----------------------------------------------------- |
| `d` | Cuts the current selections                            |
| `y` | Copies the current selections                          |
| `p` | Pastes from the clipboard after the current selection  |
| `P` | Pastes from the clipboard before the current selection |
| `u` | Undo                                                   |
| `U` | Redo                                                   |
| `>` | Increase indentation                                   |
| `<` | Decrease indentation                                   |

## Searching for things to select

| Key | Effect                                                                             |
| --: | :--------------------------------------------------------------------------------- |
| `s` | Regex search the current selections and select every instance of text that matches |
| `/` | Regex search the whole file and select every instance of text that matches         |

# Differences between Kakoune and this plugin

The goal is not to emulate Kakoune perfectly, but to emulate Kakoune using VSCode's default-ish behaviors.
This is mostly done to make my life easy when implementing any features; I try to use VSCode's built in movements, selections, etc. instead of implementing my own.
Doing has several benefits:

1. The code is easier to understand.
1. There is _way_ less code.
1. There will be fewer bugs.

Of course the downside is that VSCode will be some sort of mish-mash between Kakoune and VSCode functionality, and there **will be inconsitencies**.

All of that said I'm very open to implementing some custom functionality if there is a valid reason to do so, e.g. if some very useful Kakoune feature would not work using VSCode functionality.

Just to make it easier to figure out what's actually different, here's a table with the documented differences:

|               Command | vscode-kakoune                                                     | Kakoune                                                                         |
| --------------------: | :----------------------------------------------------------------- | :------------------------------------------------------------------------------ |
|       `h`/`j`/`k`/`l` | Moves the _cursor_ left/down/up/right.                             | Selects one character left/down/up/right.                                       |
|                   `p` | Pastes after the current selections. Does not restore selections.  | Pastes after the current selections without affecting them.                     |
|                   `P` | Pastes before the current selections. Does not restore selections. | Pastes before the current selections without affecting them.                    |
|                   `d` | Deletes and copies all selections.                                 | Deletes all selections, but only copies the first.                              |
| `jk` (in insert mode) | Exits insert mode.                                                 | N/A                                                                             |
|                   `r` | Enters insert mode without clearing the selection.                 | Replaces every character in the selection with the character pressed after `r`. |

# Motivation

I've been [trying to find a good code editor](https://thorlaksson.com/post/its-2019-why-dont-we-have-good-code-editors) for day to day use and haven't really been able to find one.
VSCode provides the best overall experience, but I want a modal editor so I need to rely on extensions.
Currently I use the Vim extension for VSCode to simulate Vim, but I like the Kakoune way of doing things so much better.
Since there's no proper Kakoune mode plugin I decided I might as well just work on it myself!

# Why Reason?

I've been wanting to learn a functional language for a long time and while working on this project I came across [Reason](https://reasonml.github.io/) and [Fable](https://fable.io/).
Reason allows you to compile OCaml to JavaScript, and Fable compiles F# to JavaScript.
Both languages looked like good candidates, and at first I decided to go with F# because I liked the structure and syntax of the language more than OCaml and Reason.

However, after using Fable for a bit, I felt that the documentation was [a bit lacking](https://thorlaksson.com/post/calling-fable-from-typescript), and the JavaScript interoperability—while usable—isn't great.
So I tried to use Reason, and I find the JavaScript interop much more user friendly and terse.
The codebase in Reason is a bit smaller, mostly thanks to less glue code for JavaScript interop.

The Reason tooling is also much better.
The F# tooling for VSCode can feel a bit slow, often taking ~1sec to think for auto-completions and types, and you need to compile manually (or use a watcher) while developing.
The Reason tooling is relatively fast, auto-completions come in instantly, and the toolchain is fast enough to **recompile automatically after every save, without needing to run a watcher**, which is an immense productivity boost.

My reason for looking into these 2 frameworks is that I wanted to use a functional language for features such as pattern matching, currying, and pipes (`|>` and `->` in Reason).
I think there are many functions that can be made simpler and smaller by using these constructs and unfortunately JavaScript isn't optimal for this; it's more difficult to write functional code in JavaScript than it is to write imperative code.

Type safety is also a big consideration, although you do get that with TypeScript, just not to the same extent.
JavaScript/TypeScript concepts like `undefined`, `null`, and `any` make the type system more complicated than I'd like it to be.

**There are some downsides to this**, particularly in the form of glue code.
The best example is probably `src/VSCode.re` where I've mapped the part of the VSCode API I use to Reason types.
Most of this code still looks like Reason, which was not the case with F#.

# Build instructions

You'll need to have `npm` or `yarn` installed.

## Script to paste into your shell of choice

```sh
npm install
npm run build
```

# Current functionality

- Basic movements and selections via `w`/`b`, `h`/`j`/`k`/`l`.
- Basic selections extensions via `W`/`B`, `H`/`J`/`K`/`L`.
- Basic goto commands via `gh`, `gj`, `gk`, `gl`.
- Basic goto selections via `Gh`, `Gj`, `Gk`, `Gl`.
- Enter insert mode with `i`, `I`, `r`, `o`, `O`, and `A`.
- Copy/cut/paste via `y`/`d`/`p`.
- Select and edit multiple selections with a regex via `s` and `/`.

# Upcoming functionality

1. Repeat previous command via `.`.
1. (Maybe) Make some indication of what mode you're in?
1. (Maybe) Don't overwrite selection in insert mode.
1. (Maybe) Handle pasting the same as Kakoune does.
1. (Maybe) Handle replace mode the same as Kakoune.
1. (Maybe) `:` commands, e.g. `:w` for saving, etc.
   - Show pop up (Ctrl+P like menu) during search and `:` commands.
     - Show suggestions from the Kakoune clippy in pop up.
1. (Maybe) Record macros.
