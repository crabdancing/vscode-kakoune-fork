type t =
  | Normal
  | Insert
  | Search
  | Goto;

let mode = ref(Normal);

let setMode = newMode => mode := newMode;

let getMode = () => mode^;
