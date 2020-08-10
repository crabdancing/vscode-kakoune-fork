[@bs.module] external vscode: Js.t({..}) = "vscode";

type disposable;

module Position = {
  type t = {
    character: int,
    line: int,
  };

  [@bs.module "vscode"] [@bs.new]
  external make: (~line: int, ~character: int) => t = "Position";
  [@bs.get] external character: t => int = "character";
  [@bs.get] external line: t => int = "line";
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

type range = {
  start: Position.t,
  [@bs.as "end"]
  end_: Position.t,
};

type textCommandArgs = {text: string};

type extension_context = {subscriptions: array(disposable)};

type textDocumentContentChangeEvent = {
  range,
  rangeLength: int,
  rangeOffset: int,
  text: string,
};

module Commands = {
  let registerCommand: (string, 'a => unit) => disposable =
    (name, callback) => vscode##commands##registerCommand(name, callback);

  let executeCommand: string => unit =
    command => vscode##commands##executeCommand(command);

  let executeCommandWithArg: (string, textCommandArgs) => unit =
    (command, arg) => vscode##commands##executeCommand(command, arg);
};

module Uri = {
  type t = {toString: (. unit) => string};
};

module TextLine = {
  type t = {
    text: string,
    isEmptyOrWhitespace: bool,
    lineNumber: int,
    range,
    rangeIncludingLineBreak: range,
    firstNonWhitespaceCharacterIndex: int,
  };
};

module TextDocument = {
  type t = {
    uri: Uri.t,
    fileName: string,
    lineAt: int => TextLine.t,
  };
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

module Window = {
  type event('a) = option('a);

  let activeTextEditor: unit => option(TextEditor.t) =
    () => Js.toOption(vscode##window##activeTextEditor);

  let showError: string => unit =
    message => vscode##window##showErrorMessage(message);

  let onDidChangeActiveTextEditor: (event(TextEditor.t) => unit) => unit =
    event => vscode##window##onDidChangeActiveTextEditor(event);
};

module Workspace = {
  let onDidChangeTextDocument: (TextDocumentChangeEvent.t => unit) => unit =
    event => vscode##workspace##onDidChangeTextDocument(event);
};

let overrideCommand = (context, command, callback) => {
  Commands.registerCommand(command, callback)
  ->Js.Array.push(context.subscriptions)
  ->ignore;
};

let overrideTypeCommand = (context, callback) => {
  let command = "type";

  overrideCommand(context, command, args => {
    TextEditor.document()
    |> Option.tap(~f=(d: TextDocument.t) =>
         switch (d.uri.toString(.), Mode.getMode()) {
         | ("debug:input", _currentMode) =>
           Commands.executeCommandWithArg("default:" ++ command, args)
         | (_documentUri, Mode.Insert) =>
           Commands.executeCommandWithArg("default:" ++ command, args);
           callback(args);
         | _ => callback(args)
         }
       )
  });
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
