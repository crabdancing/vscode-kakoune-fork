type lookupData = ((int, string), int)

let makeLookupData = (~lineNumber, ~textLine, ~offset): lookupData => (
  (lineNumber, textLine),
  offset,
)

let rec matchAll = (regex, str) =>
  switch str |> Js.Re.exec_(regex) {
  | Some(m) => Belt.List.add(str |> matchAll(regex), m)
  | None => list{}
  }

let findAllOccurrances = (regex, textLinesWithOffset: list<lookupData>) =>
  textLinesWithOffset
  |> List.map((((i, l), o)) => (i, o, l |> matchAll(regex)))
  |> List.map(((lineIndex, o, matches)) => matches |> List.map(r => (lineIndex, o, r)))
  |> List.flatten
  |> List.map(((line, characterOffset, result)) => {
    let anchorCharacter = (result |> Js.Re.index) + characterOffset
    (
      Vscode.Position.make(~character=anchorCharacter, ~line),
      Vscode.Position.make(
        ~character=(((result |> Js.Re.captures)[0] |> Js.Nullable.toOption)
          ->Belt.Option.getWithDefault("") |> String.length) + anchorCharacter,
        ~line,
      ),
    )
  })
  |> List.map(((anchor, active)) => Vscode.Selection.make(~anchor, ~active))

let searchAll = (editor: Vscode.TextEditor.t) => {
  let searchBox = Vscode.Window.createInputBox()

  Vscode.InputBox.onDidAccept(searchBox, () => searchBox |> Vscode.InputBox.dispose)

  Vscode.InputBox.onDidChangeValue(searchBox, value =>
    editor.document
    |> Vscode.TextDocument.getAllTextLines
    |> List.map(((lineNumber, textLine)) => makeLookupData(~lineNumber, ~textLine, ~offset=0))
    |> findAllOccurrances(value |> Js.Re.fromStringWithFlags(~flags="g"))
    |> Belt.List.toArray
    |> Vscode.TextEditor.setSelections(editor)
  )
  searchBox |> Vscode.InputBox.show
}

let splitSelectionIntoLines = (~editor: Vscode.TextEditor.t, selection: Vscode.Selection.t) => {
  let {active, anchor}: Vscode.Selection.t = selection

  active.line == anchor.line
    ? [selection]
    : {
        let selectionText =
          editor.document |> Vscode.TextDocument.getTextInRange(
            _,
            Vscode.Range.fromSelection(selection),
          )

        selectionText
        |> String.split_on_char('\n')
        |> List.mapi((index, l) => {
          let anchor = Vscode.Position.make(~line=index + selection.start.line, ~character=0)
          let active = Vscode.Position.make(
            ~line=index + selection.start.line,
            ~character=l |> String.length,
          )
          Vscode.Selection.make(~anchor, ~active)
        })
        |> Belt.List.toArray
      }
}

let splitSelectionsIntoLines = (
  ~editor: Vscode.TextEditor.t,
  selections: array<Vscode.Selection.t>,
) =>
  selections
  |> Array.map((s: Vscode.Selection.t) => s |> splitSelectionIntoLines(~editor) |> Array.to_list)
  |> Array.to_list
  |> Belt.List.flatten

let searchSelections = (~editor: Vscode.TextEditor.t, originalSelections) => {
  let searchBox = Vscode.Window.createInputBox()

  Vscode.InputBox.onDidAccept(searchBox, () => searchBox |> Vscode.InputBox.dispose)

  Vscode.InputBox.onDidChangeValue(searchBox, value =>
    originalSelections
    |> splitSelectionsIntoLines(~editor)
    |> List.map((s: Vscode.Selection.t) =>
      makeLookupData(
        ~lineNumber=s.start.line,
        ~textLine=editor.document |> Vscode.TextDocument.getTextInRange(
          _,
          s |> Vscode.Range.fromSelection,
        ),
        ~offset=s.start.character,
      )
    )
    |> findAllOccurrances(value |> Js.Re.fromStringWithFlags(~flags="g"))
    |> Array.of_list
    |> Vscode.TextEditor.setSelections(editor)
  )

  searchBox |> Vscode.InputBox.show
}
