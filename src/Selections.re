/**
 * Returns a VSCode selection that starts from where the active cursor is, and ends
 * at the start of the next word.
 *
 * A word is either a group of characters, or a group of punctuation marks.
 */
let rec makeNextWordSelection =
        (
          getTextLine: int => option(Vscode.TextLine.t),
          currentSelection: Vscode.Selection.t,
        ) => {
  let {active: oldActive}: Vscode.Selection.t = currentSelection;
  let {line}: Vscode.Position.t = oldActive;

  line
  |> getTextLine
  |> Option.map(~f=(l: Vscode.TextLine.t) => l.text)
  |> Option.andThen(~f=lineText => {
       // All we need is to find the new location for the cursor, since we already
       // have the old location, and we use that as the anchor.
       lineText
       |> TextLookup.findNextWordStart(~startIndex=oldActive.character)
       |> Result.map(~f=i => Vscode.Position.make(~line, ~character=i))
       |> Result.map(~f=active =>
            Vscode.Selection.make(~anchor=oldActive, ~active)
          )
       |> (
         // If we couldn't find a new location for the cursor, that means we're
         // probably at the end of the line. Let's try the next line.
         s =>
           switch (s) {
           | Ok(s) => Some(s)
           | Error(_) =>
             let nextLinePos =
               Vscode.Position.make(~line=line + 1, ~character=0);

             Vscode.Selection.make(~anchor=nextLinePos, ~active=nextLinePos)
             |> makeNextWordSelection(getTextLine);
           }
       )
     });
};

/**
 * Returns a VSCode selection that starts from where the active cursor is, and ends
 * at the beginning of the previous word.
 *
 * A word is either a group of characters, or a group of punctuation marks.
 */
let rec makePreviousWordSelection =
        (
          getTextLine: int => option(Vscode.TextLine.t),
          currentSelection: Vscode.Selection.t,
        ) => {
  let {active: oldActive}: Vscode.Selection.t = currentSelection;
  let {line}: Vscode.Position.t = oldActive;

  line
  |> getTextLine
  |> Option.map(~f=(l: Vscode.TextLine.t) => l.text)
  |> Option.andThen(~f=lineText => {
       // All we need is to find the new location for the cursor, since we already
       // have the old location, and we use that as the anchor.
       lineText
       |> TextLookup.findWordStart(~startIndex=oldActive.character)
       |> Result.map(~f=i => Vscode.Position.make(~line, ~character=i))
       |> Result.map(~f=active =>
            Vscode.Selection.make(~anchor=oldActive, ~active)
          )
       |> (
         // If we couldn't find a new location for the cursor, that means we're
         // probably at the start of the line. Let's try the previous line.
         s =>
           switch (s) {
           | Ok(s) => Some(s)
           | Error(_) =>
             "error" |> Js.log;
             let nextLinePos =
               Vscode.Position.make(
                 ~line=line - 1,
                 ~character=
                   line
                   - 1
                   |> getTextLine
                   |> Option.map(~f=(l: Vscode.TextLine.t) => l.text)
                   |> Option.unwrap(~default="")
                   |> String.length,
               );

             Vscode.Selection.make(~anchor=nextLinePos, ~active=nextLinePos)
             |> makePreviousWordSelection(getTextLine);
           }
       )
     });
};

/**
 * Returns a VSCode selection that starts from where the active cursor is, and ends
 * at the start of the next character.
 */
let rec makeNextCharacterSelection =
        (
          getTextLine: int => option(Vscode.TextLine.t),
          currentSelection: Vscode.Selection.t,
        ) => {
  let {active}: Vscode.Selection.t = currentSelection;
  let newAnchor = active;

  switch (newAnchor.line |> getTextLine) {
  | None => None
  | Some(l) =>
    let newCharacterIndex = active.character + 1;
    newCharacterIndex > (l.text |> String.length)
      ? {
        let newPos =
          Vscode.Position.make(~line=newAnchor.line + 1, ~character=0);
        Vscode.Selection.make(~anchor=newPos, ~active=newPos)
        |> makeNextCharacterSelection(getTextLine);
      }
      : {
        let newActive =
          Vscode.Position.make(
            ~line=newAnchor.line,
            ~character=newCharacterIndex,
          );
        Vscode.Selection.make(~anchor=newAnchor, ~active=newActive)
        |> Option.some;
      };
  };
};

/**
 * Returns a VSCode selection that starts from where the active cursor is, and ends
 * at the end of the previous character.
 */
let rec makePreviousCharacterSelection =
        (
          getTextLine: int => option(Vscode.TextLine.t),
          currentSelection: Vscode.Selection.t,
        ) => {
  let {active}: Vscode.Selection.t = currentSelection;
  let newAnchor = active;

  switch (newAnchor.line |> getTextLine) {
  | None => None
  | Some(l) =>
    let newCharacterIndex = active.character - 1;
    newCharacterIndex < 0
      ? {
        let newPos =
          Vscode.Position.make(
            ~line=newAnchor.line - 1,
            ~character=
              newAnchor.line
              - 1
              |> getTextLine
              |> Option.map(~f=(l: Vscode.TextLine.t) => l.text)
              |> Option.unwrap(~default="")
              |> String.length,
          );
        Vscode.Selection.make(~anchor=newPos, ~active=newPos)
        |> makePreviousCharacterSelection(getTextLine);
      }
      : {
        let newActive =
          Vscode.Position.make(
            ~line=newAnchor.line,
            ~character=newCharacterIndex,
          );
        Vscode.Selection.make(~anchor=newAnchor, ~active=newActive)
        |> Option.some;
      };
  };
};
