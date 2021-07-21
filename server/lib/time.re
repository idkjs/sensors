type date = Ptime.date;
type time = Ptime.time;

let (|>?) = (a, b) => Option.map(b, a);

let seconds_per_day = 24 * 60 * 60;

let readings_per_day = step => seconds_per_day / step;

let date_of_string_opt = text =>
  switch (text ++ "T00:00:00Z" |> Ptime.of_rfc3339) {
  | [@implicit_arity] Ok(t, _, _) => Some(Ptime.to_date(t))
  | _ => None
  };

let string_of_date = ((year, month, day)) =>
  Printf.sprintf("%04d-%02d-%02d", year, month, day);

let today = () => Ptime_clock.now() |> Ptime.to_date;

let yojson_of_date = d => `String(string_of_date(d));
let date_of_yojson = y =>
  switch (y) {
  | `String(x) =>
    switch (date_of_string_opt(x)) {
    | Some(d) => d
    | None => raise(Failure("Invalid date JSON"))
    }
  | _ => raise(Failure("Invalid date JSON"))
  };

let datetime_of_epoch_sec = x =>
  x |> Float.of_int |> Ptime.of_float_s |>? Ptime.to_date_time;

let bin = (((h, m, s), _offset), step) => {
  /* Offset can be ignored because all times in this system are UTC. */
  let total_seconds = h * 3600 + m * 60 + s;
  total_seconds / step;
};
