# Kakoune mode

[![builds.sr.ht status](https://builds.sr.ht/~reykjalin/vscode-kakoune.svg)](https://builds.sr.ht/~reykjalin/vscode-kakoune?)

_There is currently no issue tracking._

**WARNING: This VSCode extension is still under heavy development and should only be considered ready for testing. It is _not_ ready for real use.**

`vscode-kakoune` emulates the input behavior of the [Kakoune](https://kakoune.org) text editor.

# Table of Contents

- [Motivation](#motivation)
- [Why Reason?](#why-reason)
- [Build Instructions](#build-instructions)
    - [Script to paste into your shell of choice](#script-to-paste-into-your-shell-of-choice)
- [Current functionality](#current-functionality)
- [Upcoming functionality](#upcoming-functionality)

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
- Basic goto commands via `gh`, `gj`, `gk`, `gl`.
- Basic goto selections via `Gh`, `Gj`, `Gk`, `Gl`.

# Upcoming functionality

1. Extending selections.
1. Multiple selections via regex search with `/`.
1. _Edit_ multiple selections via regex search with `/`.
1. (Maybe) `:` commands, e.g. `:w` for saving, etc.
    - Show pop up (Ctrl+P like menu) during search and `:` commands.
        - Show suggestions from the Kakoune clippy in pop up.
