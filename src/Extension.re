let handleMaybeExitInsertMode = (input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "k" => Mode.setMode(Normal)
  | _ =>
    input |> Vscode.Commands.executeCommandWithArg("default:type");
    Mode.setMode(Insert);
  };

let handleInsertMode = (input: Vscode.textCommandArgs) =>
  switch (input.text) {
  | "j" => Mode.setMode(Mode.MaybeExitInsert)
  | _ => input |> Vscode.Commands.executeCommandWithArg("default:type")
  };

let handleSearchMode = (_editor, _input: Vscode.textCommandArgs) => ();

let handleGotoExtendMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.selectToLineStart()
  | "j" => Movements.selectToFileEnd()
  | "k" => Movements.selectToFileStart()
  | "l" => Movements.selectToLineEnd()
  | _ => ()
  };
  Mode.setMode(Mode.Normal);
};

let handleGotoMode = (input: Vscode.textCommandArgs) => {
  switch (input.text) {
  | "h" => Movements.gotoLineStart()
  | "j" => Movements.gotoFileEnd()
  | "k" => Movements.gotoFileStart()
  | "l" => Movements.gotoLineEnd()
  | _ => ()
  };
  Mode.setMode(Mode.Normal);
};

let handleNormalMode = (input: Vscode.textCommandArgs) =>
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
  | "g" => Mode.setMode(Mode.Goto)
  | "G" => Mode.setMode(Mode.GotoExtend)
  // Edits.
  | "d" => Edits.deleteSelections()
  | "p" => Edits.paste()
  | "P" => Edits.clearSelectionsAndPaste()
  | "c" =>
    Edits.deleteSelections();
    Mode.setMode(Mode.Insert);
  | "u" => Edits.undo()
  | "U" => Edits.redo()
  // Insert mode.
  | "i" => Mode.setMode(Mode.Insert)
  | _ => ()
  };

let onType = (args: Vscode.textCommandArgs) => {
  args->Js.log;

  Vscode.Window.activeTextEditor()
  |> Option.tap(~f=e =>
       switch (Mode.getMode()) {
       | Normal => args |> handleNormalMode
       | Insert => args |> handleInsertMode
       | MaybeExitInsert => args |> handleMaybeExitInsertMode
       | Search => args |> handleSearchMode(e)
       | Goto => args |> handleGotoMode
       | GotoExtend => args |> handleGotoExtendMode
       }
     );
};

let activate = context =>
  context
  |> Vscode.overrideCommand("type", args =>
       Vscode.TextEditor.document()
       |> Option.tap(~f=(d: Vscode.TextDocument.t) =>
            d.uri.toString(.) == "debug:input"
              ? Vscode.Commands.executeCommandWithArg("default:type", args)
              : args |> onType
          )
     );
