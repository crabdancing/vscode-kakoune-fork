let handleMaybeExitInsertMode = (input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "k" => Mode.setMode(Normal)
  | _ =>
    input |> Vscode.Commands.executeCommandWithArg("default:type");
    Mode.setMode(Insert);
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
  Mode.setMode(Normal);
};

let handleGotoMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.gotoLineStart()
  | "j" => Movements.gotoFileEnd()
  | "k" => Movements.gotoFileStart()
  | "l" => Movements.gotoLineEnd()
  | _ => ()
  };
  Mode.setMode(Normal);
};

let handleNormalMode =
    (editor: Vscode.TextEditor.t, input: Vscode.textCommandArgs) =>
  switch (input.text) {
  // Movements.
  | "w" => Movements.selectNextWord()
  | "W" => Movements.extendNextWord()
  | "b" => Movements.selectPreviousWord()
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
  // Edits.
  | "d" => editor |> Edits.deleteSelections
  | "p" => Edits.paste()
  | "P" => Edits.clearSelectionsAndPaste()
  | "y" => Edits.copy()
  | "c" =>
    editor |> Edits.deleteSelections;
    Mode.setMode(Insert);
  | "u" => Edits.undo()
  | "U" => Edits.redo()
  // Insert mode.
  | "i" =>
    Vscode.Commands.cancelSelection();
    Mode.setMode(Insert);
  | "I" =>
    Movements.gotoLineHome();
    Mode.setMode(Insert);
  | "r" => Mode.setMode(Insert)
  | "A" =>
    Movements.gotoLineEnd();
    Mode.setMode(Insert);
  | "o" =>
    Edits.insertLineBelow();
    Mode.setMode(Insert);
  | "O" =>
    Edits.insertLineAbove();
    Mode.setMode(Insert);
  // Search mode.
  | "/" => editor |> Search.searchAll
  | "s" =>
    editor
    |> Vscode.TextEditor.getSelections
    |> Search.searchSelections(~editor)
  | _ => ()
  };

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.tap(~f=e =>
       switch (Mode.getMode()) {
       | Normal => args |> handleNormalMode(e)
       | Insert => args |> handleInsertMode
       | MaybeExitInsert => args |> handleMaybeExitInsertMode
       | Goto => args |> handleGotoMode
       | GotoExtend => args |> handleGotoExtendMode
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
    Mode.setMode(Mode.Normal)
  );
};
