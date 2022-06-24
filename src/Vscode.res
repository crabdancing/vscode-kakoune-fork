@module external vscode: 'a = "vscode"

type disposable = unit

module Position = {
  type t = {
    character: int,
    line: int,
  }

  @module("vscode") @new external make: (~line: int, ~character: int) => t = "Position"
  @get external character: t => int = "character"
  @get external line: t => int = "line"

  @send external isEqual: (t, t) => bool = "isEqual"

  // We switch these so that the preferred pipe-last operator makes
  // more sense.
  @send external isAfter: (t, t) => bool = "isBefore"
  @send external isBefore: (t, t) => bool = "isAfter"
}

module Selection = {
  type t = {
    active: Position.t,
    anchor: Position.t,
    @as("end") end_: Position.t,
    isEmpty: bool,
    isReversed: bool,
    isSingleLine: bool,
    start: Position.t,
  }

  @module("vscode") @new
  external make: (~anchor: Position.t, ~active: Position.t) => t = "Selection"
  @get external anchor: t => Position.t = "anchor"
  @get external active: t => Position.t = "active"
  @get external start: t => Position.t = "start"
  @get external end_: t => Position.t = "end"
}

module Range = {
  type t = {
    start: Position.t,
    @as("end") end_: Position.t,
  }

  @module("vscode") @new external make: (~start: Position.t, ~end_: Position.t) => t = "Range"

  let fromSelection = (selection: Selection.t) => make(~start=selection.start, ~end_=selection.end_)
}

type textCommandArgs = {text: string}

type extension_context = {subscriptions: array<disposable>}

type textDocumentContentChangeEvent = {
  range: Range.t,
  rangeLength: int,
  rangeOffset: int,
  text: string,
}

module Commands = {
  module CursorMoveArguments = {
    type t = {
      @as("to") to_: string,
      by: string,
      value: int,
      select: bool,
    }

    let make = (~to_, ~by, ~value=1, ~select=false, ()) => {
      to_: to_,
      by: by,
      value: value,
      select: select,
    }
  }

  module EditorScrollArguments = {
    type t = {
      @as("to") to_: string,
      by: string,
      value: int,
      revealCursor: bool,
    }

    let make = (~to_, ~by, ~value=1, ~revealCursor=true, ()) => {
      to_: to_,
      by: by,
      value: value,
      revealCursor: revealCursor,
    }
  }

  let registerCommand: (string, 'a => unit) => disposable = (name, callback) =>
    vscode["commands"]["registerCommand"](. name, callback)

  let executeCommand: string => unit = command => vscode["commands"]["executeCommand"](. command)

  let executeCommandWithArg: (string, textCommandArgs) => unit = (command, arg) =>
    vscode["commands"]["executeCommand"](. command, arg)

  let executeEditorScrollCommand: EditorScrollArguments.t => unit = arg =>
    vscode["commands"]["executeCommand"](. "editorScroll", arg)

  let executeCursorMoveCommand: CursorMoveArguments.t => unit = arg =>
    vscode["commands"]["executeCommand"](. "cursorMove", arg)

  let cancelSelection = () => "cancelSelection" |> executeCommand
  let expandLineSelection = () => "expandLineSelection" |> executeCommand
  let insertCursorBelow = () => "editor.action.insertCursorBelow" |> executeCommand

  let moveCharacterRight = () => "cursorRight" |> executeCommand
  let selectCharacterRight = () => "cursorRightSelect" |> executeCommand

  let moveCharacterLeft = () => "cursorLeft" |> executeCommand
  let selectCharacterLeft = () => "cursorLeftSelect" |> executeCommand

  let moveLineDown = () => "cursorDown" |> executeCommand
  let moveLineUp = () => "cursorUp" |> executeCommand
  let selectLineDown = () => "cursorDownSelect" |> executeCommand
  let selectLineUp = () => "cursorUpSelect" |> executeCommand

  let moveCursorLineStart = () => "cursorLineStart" |> executeCommand
  let moveCursorLineEnd = () => "cursorLineEnd" |> executeCommand
  let selectToLineStart = () => "cursorLineStartSelect" |> executeCommand
  let selectToLineEnd = () => "cursorLineEndSelect" |> executeCommand

  let moveCursorHome = () => "cursorHome" |> executeCommand

  let moveCursorTop = () => "cursorTop" |> executeCommand
  let moveCursorBottom = () => "cursorBottom" |> executeCommand
  let selectToTop = () => "cursorTopSelect" |> executeCommand
  let selectToBottom = () => "cursorBottomSelect" |> executeCommand

  let selectWordStartRight = () => "cursorWordStartRightSelect" |> executeCommand

  let selectWordStartLeft = () => "cursorWordStartLeftSelect" |> executeCommand

  let copy = () => "editor.action.clipboardCopyAction" |> executeCommand
  let cut = () => "editor.action.clipboardCutAction" |> executeCommand
  let paste = () => "editor.action.clipboardPasteAction" |> executeCommand
  let undo = () => "undo" |> executeCommand
  let redo = () => "redo" |> executeCommand

  let insertLineBelow = () => "editor.action.insertLineAfter" |> executeCommand
  let insertLineAbove = () => "editor.action.insertLineBefore" |> executeCommand

  let scrollHalfPageDown = () =>
    EditorScrollArguments.make(~to_="down", ~by="halfPage", ()) |> executeEditorScrollCommand
  let scrollHalfPageUp = () =>
    EditorScrollArguments.make(~to_="up", ~by="halfPage", ()) |> executeEditorScrollCommand

  let find = () => "actions.find" |> executeCommand
  let toggleFindInSelection = () => "toggleFindInSelection" |> executeCommand
  let toggleFindRegex = () => "toggleFindRegex" |> executeCommand

  let increaseIndentation = () => "editor.action.indentLines" |> executeCommand
  let decreaseIndentation = () => "editor.action.outdentLines" |> executeCommand
}

module Uri = {
  type t = {toString: (. unit) => string}
}

module TextLine = {
  type t = {
    text: string,
    isEmptyOrWhitespace: bool,
    lineNumber: int,
    range: Range.t,
    rangeIncludingLineBreak: Range.t,
    firstNonWhitespaceCharacterIndex: int,
  }
}

module TextDocument = {
  type t = {
    uri: Uri.t,
    fileName: string,
    lineAt: int => TextLine.t,
    lineCount: int,
  }

  let getTextLine = (index, document) =>
    index >= 0 && index < document.lineCount ? Some(document.lineAt(index)) : None

  let getLineTextAt = (index, document) =>
    (document |> getTextLine(index))->Belt.Option.map((tl: TextLine.t) => tl.text)

  let getAllTextLines = document =>
    Belt.List.makeBy(document.lineCount, i => i)
    ->Belt.List.map(index => document |> getTextLine(index))
    ->Belt.List.map(tl => tl->Belt.Option.map((t: TextLine.t) => t.text))
    ->Belt.List.mapWithIndex((lineNumber, line) => (
      lineNumber,
      line->Belt.Option.getWithDefault(""),
    ))

  @send external lineAt: Selection.t => TextLine.t = "lineAt"

  @send external getTextInRange: (t, Range.t) => string = "getText"
  @send external getAllText: (t, unit) => string = "getText"
}

module TextDocumentChangeEvent = {
  type t = {
    contentChanges: array<textDocumentContentChangeEvent>,
    document: TextDocument.t,
  }
}

module TextEditor = {
  type t = {
    document: TextDocument.t,
    selection: Selection.t,
  }

  @deriving(accessors)
  type cursorStyle =
    | @as(1) Line
    | @as(2) Block
    | @as(3) Underline
    | @as(4) LineThin
    | @as(5) BlockOutline
    | @as(6) UnderlineThin

  type textEditorOptions = {mutable cursorStyle: cursorStyle}

  let options: unit => option<textEditorOptions> = () =>
    Js.toOption(vscode["window"]["activeTextEditor"]["options"])

  let document: unit => option<TextDocument.t> = () =>
    Js.toOption(vscode["window"]["activeTextEditor"]["document"])

  @get external getSelection: t => Selection.t = "selection"
  @set external setSelection: (t, Selection.t) => unit = "selection"
  @get external getSelections: t => array<Selection.t> = "selections"
  @set external setSelections: (t, array<Selection.t>) => unit = "selections"
}

module InputBox = {
  type t = {value: string}

  @send external hide: t => unit = "hide"
  @send external dispose: t => unit = "dispose"
  @send external show: t => unit = "show"

  @send external onDidChangeValue: (t, string => unit) => unit = "onDidChangeValue"
  @send external onDidAccept: (t, unit => unit) => unit = "onDidAccept"
}

module Window = {
  type event<'a> = option<'a>

  let activeTextEditor: unit => option<TextEditor.t> = () =>
    Js.toOption(vscode["window"]["activeTextEditor"])

  let showError: string => unit = message => vscode["window"]["showErrorMessage"](. message)

  let onDidChangeActiveTextEditor: (event<TextEditor.t> => unit) => unit = event =>
    vscode["window"]["onDidChangeActiveTextEditor"](. event)

  let createInputBox: unit => InputBox.t = () => vscode["window"]["createInputBox"]()
}

module Workspace = {
  let onDidChangeTextDocument: (TextDocumentChangeEvent.t => unit) => unit = event =>
    vscode["workspace"]["onDidChangeTextDocument"](. event)
}

let overrideCommand = (command, callback, context) => {
  Commands.registerCommand(command, callback)->Js.Array.push(context.subscriptions)->ignore
}

let setCursorStyle = (style: TextEditor.cursorStyle) => {
  switch TextEditor.options() {
  | None => ()
  | Some(o) => o.cursorStyle = style
  }
}

let setSelections = selections => {
  switch Window.activeTextEditor() {
  | Some(ed) => ed->TextEditor.setSelections(selections)
  | None => ()
  }
}
