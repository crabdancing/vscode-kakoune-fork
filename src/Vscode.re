[@bs.module] external vscode: Js.t({..}) = "vscode";

type disposable = unit;

module Position = {
  type t = {
    character: int,
    line: int,
  };

  [@bs.module "vscode"] [@bs.new]
  external make: (~line: int, ~character: int) => t = "Position";
  [@bs.get] external character: t => int = "character";
  [@bs.get] external line: t => int = "line";

  [@bs.send] external isEqual: (t, t) => bool = "isEqual";

  // We switch these so that the preferred pipe-last operator makes
  // more sense.
  [@bs.send] external isAfter: (t, t) => bool = "isBefore";
  [@bs.send] external isBefore: (t, t) => bool = "isAfter";
};

module Selection = {
  type t = {
    active: Position.t,
    anchor: Position.t,
    [@bs.as "end"]
    end_: Position.t,
    isEmpty: bool,
    isReversed: bool,
    isSingleLine: bool,
    start: Position.t,
  };

  [@bs.module "vscode"] [@bs.new]
  external make: (~anchor: Position.t, ~active: Position.t) => t = "Selection";
  [@bs.get] external anchor: t => Position.t = "anchor";
  [@bs.get] external active: t => Position.t = "active";
  [@bs.get] external start: t => Position.t = "start";
  [@bs.get] external end_: t => Position.t = "end";
};

module Range = {
  type t = {
    start: Position.t,
    [@bs.as "end"]
    end_: Position.t,
  };

  [@bs.module "vscode"] [@bs.new]
  external make: (~start: Position.t, ~end_: Position.t) => t = "Range";

  let fromSelection = (selection: Selection.t) =>
    make(~start=selection.start, ~end_=selection.end_);
};

type textCommandArgs = {text: string};

type extension_context = {subscriptions: array(disposable)};

type textDocumentContentChangeEvent = {
  range: Range.t,
  rangeLength: int,
  rangeOffset: int,
  text: string,
};

module Commands = {
  module CursorMoveArguments = {
    type t = {
      [@bs.as "to"]
      to_: string,
      by: string,
      value: int,
      select: bool,
    };

    let make = (~to_, ~by, ~value=1, ~select=false, ()) => {
      to_,
      by,
      value,
      select,
    };
  };

  module EditorScrollArguments = {
    type t = {
      [@bs.as "to"]
      to_: string,
      by: string,
      value: int,
      revealCursor: bool,
    };

    let make = (~to_, ~by, ~value=1, ~revealCursor=true, ()) => {
      to_,
      by,
      value,
      revealCursor,
    };
  };

  let registerCommand: (string, 'a => unit) => disposable =
    (name, callback) => vscode##commands##registerCommand(name, callback);

  let executeCommand: string => unit =
    command => vscode##commands##executeCommand(command);

  let executeCommandWithArg: (string, textCommandArgs) => unit =
    (command, arg) => vscode##commands##executeCommand(command, arg);

  let executeEditorScrollCommand: EditorScrollArguments.t => unit =
    arg => vscode##commands##executeCommand("editorScroll", arg);

  let executeCursorMoveCommand: CursorMoveArguments.t => unit =
    arg => vscode##commands##executeCommand("cursorMove", arg);

  let cancelSelection = () => "cancelSelection" |> executeCommand;
  let expandLineSelection = () => "expandLineSelection" |> executeCommand;

  let moveCharacterRight = () => "cursorRight" |> executeCommand;
  let selectCharacterRight = () => "cursorRightSelect" |> executeCommand;

  let moveCharacterLeft = () => "cursorLeft" |> executeCommand;
  let selectCharacterLeft = () => "cursorLeftSelect" |> executeCommand;

  let moveLineDown = () => "cursorDown" |> executeCommand;
  let moveLineUp = () => "cursorUp" |> executeCommand;
  let selectLineDown = () => "cursorDownSelect" |> executeCommand;
  let selectLineUp = () => "cursorUpSelect" |> executeCommand;

  let moveCursorLineStart = () => "cursorLineStart" |> executeCommand;
  let moveCursorLineEnd = () => "cursorLineEnd" |> executeCommand;
  let selectToLineStart = () => "cursorLineStartSelect" |> executeCommand;
  let selectToLineEnd = () => "cursorLineEndSelect" |> executeCommand;

  let moveCursorHome = () => "cursorHome" |> executeCommand;

  let moveCursorTop = () => "cursorTop" |> executeCommand;
  let moveCursorBottom = () => "cursorBottom" |> executeCommand;
  let selectToTop = () => "cursorTopSelect" |> executeCommand;
  let selectToBottom = () => "cursorBottomSelect" |> executeCommand;

  let selectWordStartRight = () =>
    "cursorWordStartRightSelect" |> executeCommand;

  let selectWordStartLeft = () =>
    "cursorWordStartLeftSelect" |> executeCommand;

  let copy = () => "editor.action.clipboardCopyAction" |> executeCommand;
  let cut = () => "editor.action.clipboardCutAction" |> executeCommand;
  let paste = () => "editor.action.clipboardPasteAction" |> executeCommand;
  let undo = () => "undo" |> executeCommand;
  let redo = () => "redo" |> executeCommand;

  let insertLineBelow = () =>
    "editor.action.insertLineAfter" |> executeCommand;
  let insertLineAbove = () =>
    "editor.action.insertLineBefore" |> executeCommand;

  let scrollHalfPageDown = () =>
    EditorScrollArguments.make(~to_="down", ~by="halfPage", ())
    |> executeEditorScrollCommand;
  let scrollHalfPageUp = () =>
    EditorScrollArguments.make(~to_="up", ~by="halfPage", ())
    |> executeEditorScrollCommand;

  let find = () => "actions.find" |> executeCommand;
  let toggleFindInSelection = () => "toggleFindInSelection" |> executeCommand;
  let toggleFindRegex = () => "toggleFindRegex" |> executeCommand;

  let increaseIndentation = () =>
    "editor.action.indentLines" |> executeCommand;
  let decreaseIndentation = () =>
    "editor.action.outdentLines" |> executeCommand;
};

module Uri = {
  type t = {toString: (. unit) => string};
};

module TextLine = {
  type t = {
    text: string,
    isEmptyOrWhitespace: bool,
    lineNumber: int,
    range: Range.t,
    rangeIncludingLineBreak: Range.t,
    firstNonWhitespaceCharacterIndex: int,
  };
};

module TextDocument = {
  type t = {
    uri: Uri.t,
    fileName: string,
    lineAt: int => TextLine.t,
    lineCount: int,
  };

  let getTextLine = (index, document) =>
    index >= 0 && index < document.lineCount
      ? Some(document.lineAt(index)) : None;

  let getAllTextLines = document =>
    List.range(document.lineCount)
    |> List.map(~f=index => document |> getTextLine(index))
    |> List.map(~f=tl => tl |> Option.map(~f=(t: TextLine.t) => t.text))
    |> List.mapWithIndex(~f=(lineNumber, line) =>
         (lineNumber, line |> Option.unwrap(~default=""))
       );

  [@bs.send] external lineAt: Selection.t => TextLine.t = "lineAt";

  [@bs.send] external getTextInRange: (t, Range.t) => string = "getText";
  [@bs.send] external getAllText: (t, unit) => string = "getText";
};

module TextDocumentChangeEvent = {
  type t = {
    contentChanges: array(textDocumentContentChangeEvent),
    document: TextDocument.t,
  };
};

module TextEditor = {
  type t = {
    document: TextDocument.t,
    selection: Selection.t,
  };

  [@bs.deriving jsConverter]
  type cursorStyle =
    | [@bs.as 1] Line
    | [@bs.as 2] Block
    | [@bs.as 3] Underline
    | [@bs.as 4] LineThin
    | [@bs.as 5] BlockOutline
    | [@bs.as 6] UnderlineThin;

  type textEditorOptions = {mutable cursorStyle: int};

  let options: unit => option(textEditorOptions) =
    () => Js.toOption(vscode##window##activeTextEditor##options);

  let document: unit => option(TextDocument.t) =
    () => Js.toOption(vscode##window##activeTextEditor##document);

  [@bs.get] external getSelection: t => Selection.t = "selection";
  [@bs.set] external setSelection: (t, Selection.t) => unit = "selection";
  [@bs.get] external getSelections: t => array(Selection.t) = "selections";
  [@bs.set]
  external setSelections: (t, array(Selection.t)) => unit = "selections";
};

module InputBox = {
  type t = {value: string};

  [@bs.send] external hide: t => unit = "hide";
  [@bs.send] external dispose: t => unit = "dispose";
  [@bs.send] external show: t => unit = "show";

  [@bs.send]
  external onDidChangeValue: (t, string => unit) => unit = "onDidChangeValue";
  [@bs.send] external onDidAccept: (t, unit => unit) => unit = "onDidAccept";
};

module Window = {
  type event('a) = option('a);

  let activeTextEditor: unit => option(TextEditor.t) =
    () => Js.toOption(vscode##window##activeTextEditor);

  let showError: string => unit =
    message => vscode##window##showErrorMessage(message);

  let onDidChangeActiveTextEditor: (event(TextEditor.t) => unit) => unit =
    event => vscode##window##onDidChangeActiveTextEditor(event);

  let createInputBox: unit => InputBox.t =
    () => vscode##window##createInputBox();
};

module Workspace = {
  let onDidChangeTextDocument: (TextDocumentChangeEvent.t => unit) => unit =
    event => vscode##workspace##onDidChangeTextDocument(event);
};

let overrideCommand = (command, callback, context) => {
  Commands.registerCommand(command, callback)
  ->Js.Array.push(context.subscriptions)
  ->ignore;
};

let setCursorStyle = style => {
  switch (TextEditor.options()) {
  | None => ()
  | Some(o) => o.cursorStyle = style->TextEditor.cursorStyleToJs
  };
};

let setSelections = selections => {
  switch (Window.activeTextEditor()) {
  | Some(ed) => ed->TextEditor.setSelections(selections)
  | None => ()
  };
};
