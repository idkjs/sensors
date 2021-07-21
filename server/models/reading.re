module type DB = Caqti_lwt.CONNECTION;
module R = Caqti_request;
module T = Caqti_type;
module Time = Lib.Time;

[@deriving yojson]
type readings = array(option(float));

[@deriving yojson]
type t = {
  sensor_id: int,
  occurred: Time.date,
  readings,
};

let empty = (s: Sensor.t, occurred) => {
  let n = Time.readings_per_day(900); /* FIXME: Sensor step sizes. */
  let readings = Array.init(n, _ => None);
  {sensor_id: s.id, occurred, readings};
};

let sql_readings = {
  let encode = (a: readings) =>
    Ok(a |> yojson_of_readings |> Yojson.Safe.to_string);

  let decode = text =>
    Ok(text |> Yojson.Safe.from_string |> readings_of_yojson);

  T.(custom(~encode, ~decode, string));
};

let sql_date = {
  let encode = (d: Time.date) => Ok(Time.string_of_date(d));
  let decode = text =>
    switch (Time.date_of_string_opt(text)) {
    | Some(d) => Ok(d)
    | _ => Error(Printf.sprintf("Failed to parse date string '%s'.", text))
    };

  T.(custom(~encode, ~decode, string));
};

let insert = {
  let query =
    R.exec(
      T.(tup3(T.int, sql_date, sql_readings)),
      {|
      INSERT INTO sensor_reading (sensor, occurred, readings)
      VALUES ($1, $2, $3)
      ON CONFLICT (sensor, occurred) DO UPDATE SET readings = $3
      |},
    );

  (r, module Db: DB) => {
    let%lwt result = Db.exec(query, (r.sensor_id, r.occurred, r.readings));
    Caqti_lwt.or_fail(result);
  };
};

let read_range = {
  let query =
    R.collect(
      T.(tup4(T.int, T.int, sql_date, sql_date)),
      T.(tup2(sql_date, sql_readings)),
      {|
      SELECT sr.occurred, sr.readings FROM sensor_reading sr
      INNER JOIN user_sensor us ON
        sr.sensor = $2 AND us.sensor = sr.sensor AND us.app_user = $1
      WHERE
        $3 <= sr.occurred and sr.occurred <= $4
      |},
    );

  (user_id, sensor_id, start_d, end_d, module Db: DB) => {
    let%lwt result =
      Db.collect_list(query, (user_id, sensor_id, start_d, end_d));
    let%lwt tuples = Caqti_lwt.or_fail(result);
    let convert = ((d, readings)) => {sensor_id, occurred: d, readings};
    Lwt.return(List.map(convert, tuples));
  };
};

let read_day = {
  let query =
    R.find_opt(
      T.(tup2(T.int, sql_date)),
      sql_readings,
      {|
      SELECT sr.readings FROM sensor_reading sr
      WHERE
        sr.sensor = $1 AND sr.occurred = $2
      |},
    );

  (sensor_id, d, module Db: DB) => {
    let%lwt result = Db.find_opt(query, (sensor_id, d));
    let%lwt tuples = Caqti_lwt.or_fail(result);
    let convert = readings => {sensor_id, occurred: d, readings};
    Lwt.return(Option.map(convert, tuples));
  };
};

let overwrite = ({sensor_id, occurred, readings}, a: array(option(float))) => {
  let rec optzip = (x, y) =>
    switch (x, y) {
    | ([_, ...xs], [Some(y), ...ys]) => [Some(y), ...optzip(xs, ys)]
    | ([Some(x), ...xs], [None, ...ys]) => [Some(x), ...optzip(xs, ys)]
    | ([None, ...xs], [None, ...ys]) => [None, ...optzip(xs, ys)]
    | _ => []
    };

  let pairs = optzip(Array.to_list(readings), Array.to_list(a));
  let readings = Array.of_list(pairs);
  {sensor_id, occurred, readings};
};
