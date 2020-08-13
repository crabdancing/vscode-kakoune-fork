let toNormalMode = () => {
  Mode.setMode(Normal);
  Vscode.setCursorStyle(Block);
};

let toInsertMode = () => {
  Mode.setMode(Insert);
  Vscode.setCursorStyle(Line);
};

let handleMaybeExitInsertMode = (input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "k" => toNormalMode()
  | _ =>
    ({text: "j"}: Vscode.textCommandArgs)
    |> Vscode.Commands.executeCommandWithArg("default:type");
    input |> Vscode.Commands.executeCommandWithArg("default:type");
    toInsertMode();
  };

let handleInsertMode = (input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "j" => Mode.setMode(MaybeExitInsert)
  | _ => input |> Vscode.Commands.executeCommandWithArg("default:type")
  };

let handleGotoExtendMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.selectToLineStart()
  | "j" => Movements.selectToFileEnd()
  | "k" => Movements.selectToFileStart()
  | "l" => Movements.selectToLineEnd()
  | _ => ()
  };
  toNormalMode();
};

let handleGotoMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.gotoLineStart()
  | "j" => Movements.gotoFileEnd()
  | "k" => Movements.gotoFileStart()
  | "l" => Movements.gotoLineEnd()
  | _ => ()
  };
  toNormalMode();
};

let handleExtendSelectionsToCharacter =
    (editor: Vscode.TextEditor.t, input: Vscode.textCommandArgs) => {
  editor
  |> Vscode.TextEditor.getSelections
  |> Selections.extendSelectionFromActiveToCharacter(
       ~setSelections=Vscode.TextEditor.setSelections(editor),
       ~getTextLine=Vscode.TextDocument.getLineTextAt(_, editor.document),
       ~character=input.text,
     );

  toNormalMode();
};

let handleSelectToCharacter =
    (editor: Vscode.TextEditor.t, input: Vscode.textCommandArgs) => {
  editor
  |> Vscode.TextEditor.getSelections
  |> Selections.selectFromActiveToCharacter(
       ~setSelections=Vscode.TextEditor.setSelections(editor),
       ~getTextLine=Vscode.TextDocument.getLineTextAt(_, editor.document),
       ~character=input.text,
     );

  toNormalMode();
};

let handleExtendSelectionsOnCharacter =
    (editor: Vscode.TextEditor.t, input: Vscode.textCommandArgs) => {
  editor
  |> Vscode.TextEditor.getSelections
  |> Selections.extendSelectionFromActiveOnCharacter(
       ~setSelections=Vscode.TextEditor.setSelections(editor),
       ~getTextLine=Vscode.TextDocument.getLineTextAt(_, editor.document),
       ~character=input.text,
     );

  toNormalMode();
};

let handleSelectOnCharacter =
    (editor: Vscode.TextEditor.t, input: Vscode.textCommandArgs) => {
  editor
  |> Vscode.TextEditor.getSelections
  |> Selections.selectFromActiveOnCharacter(
       ~setSelections=Vscode.TextEditor.setSelections(editor),
       ~getTextLine=Vscode.TextDocument.getLineTextAt(_, editor.document),
       ~character=input.text,
     );

  toNormalMode();
};

let handleNormalMode =
    (editor: Vscode.TextEditor.t, input: Vscode.textCommandArgs) =>
  switch (input.text) {
  // Movements.
  | "w" => editor |> Movements.selectNextWord
  | "W" => Movements.extendNextWord()
  | "b" => editor |> Movements.selectPreviousWord
  | "B" => Movements.extendPreviousWord()
  | "h" => Movements.moveCharacterLeft()
  | "H" => Movements.extendCharacterLeft()
  | "j" => Movements.moveLineDown()
  | "J" => Movements.extendLineDown()
  | "k" => Movements.moveLineUp()
  | "K" => Movements.extendLineUp()
  | "l" => Movements.moveCharacterRight()
  | "L" => Movements.extendCharacterRight()
  | "x" => Movements.selectCurrentLine()
  | "X" => Movements.expandLineSelection()
  // Goto.
  | "g" => Mode.setMode(Goto)
  | "G" => Mode.setMode(GotoExtend)
  // Selections manipulation.
  | ";" =>
    editor
    |> Vscode.TextEditor.getSelections
    |> Selections.onlyActiveCursor(
         ~setSelections=Vscode.TextEditor.setSelections(editor),
       )
  | " " =>
    editor
    |> Vscode.TextEditor.getSelections
    |> Selections.onlyPrimarySelection(
         ~setSelections=Vscode.TextEditor.setSelections(editor),
       )
  | "t" => Mode.setMode(SelectToCharacter)
  | "T" => Mode.setMode(ExtendSelectionsToCharacter)
  | "f" => Mode.setMode(SelectOnCharacter)
  | "F" => Mode.setMode(ExtendSelectionsOnCharacter)
  // Edits.
  | "d" => editor |> Edits.deleteSelections
  | "p" => editor |> Edits.pasteAfter
  | "P" => editor |> Edits.pasteBefore
  | "y" => Edits.copy()
  | "c" =>
    editor |> Edits.deleteSelections;
    toInsertMode();
  | "u" => Edits.undo()
  | "U" => Edits.redo()
  | ">" => Edits.increaseSelectionIndentation()
  | "<" => Edits.decreaseSelectionIndentation()
  // Insert mode.
  | "i" =>
    editor |> Movements.moveCursorToSelectionStart;
    toInsertMode();
  | "I" =>
    Movements.gotoLineHome();
    toInsertMode();
  | "r" => toInsertMode()
  | "a" =>
    editor |> Movements.moveCursorToSelectionEnd;
    toInsertMode();
  | "A" =>
    Movements.gotoLineEnd();
    toInsertMode();
  | "o" =>
    Edits.insertLineBelow();
    toInsertMode();
  | "O" =>
    Edits.insertLineAbove();
    toInsertMode();
  // Search mode.
  | "/" => editor |> Search.searchAll
  | "s" =>
    editor
    |> Vscode.TextEditor.getSelections
    |> Search.searchSelections(~editor)
  | _ => ()
  };

let onType = (args: Vscode.textCommandArgs) => {
  Vscode.Window.activeTextEditor()
  |> Option.tap(~f=e =>
       switch (Mode.getMode()) {
       | Normal => args |> handleNormalMode(e)
       | Insert => args |> handleInsertMode
       | MaybeExitInsert => args |> handleMaybeExitInsertMode
       | Goto => args |> handleGotoMode
       | GotoExtend => args |> handleGotoExtendMode
       | SelectToCharacter => args |> handleSelectToCharacter(e)
       | ExtendSelectionsToCharacter =>
         args |> handleExtendSelectionsToCharacter(e)
       | SelectOnCharacter => args |> handleSelectOnCharacter(e)
       | ExtendSelectionsOnCharacter =>
         args |> handleExtendSelectionsOnCharacter(e)
       }
     );
};

let activate = context => {
  context
  |> Vscode.overrideCommand("type", args =>
       Vscode.TextEditor.document()
       |> Option.tap(~f=(d: Vscode.TextDocument.t) =>
            d.uri.toString(.) == "debug:input"
              ? Vscode.Commands.executeCommandWithArg("default:type", args)
              : args |> onType
          )
     );

  Vscode.Commands.registerCommand(
    "vscode-kakoune.scrollDown",
    Vscode.Commands.scrollHalfPageDown,
  );
  Vscode.Commands.registerCommand(
    "vscode-kakoune.scrollUp",
    Vscode.Commands.scrollHalfPageUp,
  );
  Vscode.Commands.registerCommand("vscode-kakoune.toNormalMode", () =>
    toNormalMode()
  );
  Vscode.Commands.registerCommand("vscode-kakoune.toInsertMode", () =>
    toInsertMode()
  );

  toNormalMode();
};
