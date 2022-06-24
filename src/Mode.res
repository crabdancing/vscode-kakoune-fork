type t =
  | Normal
  | Insert
  | MaybeExitInsert
  | Goto
  | GotoExtend
  | SelectOnCharacter
  | ExtendSelectionsOnCharacter
  | SelectToCharacter
  | ExtendSelectionsToCharacter

let mode = ref(Normal)

let setMode = newMode => mode := newMode

let getMode = () => mode.contents
