let onlyPrimarySelection =
    (~setSelections, selections: array(Vscode.Selection.t)) =>
  selections |> Array.first |> Option.tap(~f=s => [|s|] |> setSelections);

let onlyActiveCursor =
    (~setSelections, selections: array(Vscode.Selection.t)): unit =>
  selections
  |> Array.map(~f=(s: Vscode.Selection.t) =>
       Vscode.Selection.make(~anchor=s.active, ~active=s.active)
     )
  |> setSelections;
