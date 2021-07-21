module type DB = Caqti_lwt.CONNECTION;
module R = Caqti_request;
module T = Caqti_type;

[@deriving yojson]
type t = {
  [@key "username"]
  username: string,
  [@key "name"]
  name: string,
  [@key "sensors"]
  sensors: list(int),
};

/* Note: Storing passwords in cleartext is a bad idea in a production app.*/
let get = {
  let query =
    R.find_opt(
      T.(tup2(T.string, T.string)),
      T.int,
      "SELECT id FROM app_user WHERE username = ? and password = ?",
    );
  (username, password, module Db: DB) => {
    let%lwt user_or_error = Db.find_opt(query, (username, password));
    Caqti_lwt.or_fail(user_or_error);
  };
};

let sensors = {
  let query =
    R.collect(
      T.int,
      T.int,
      "SELECT sensor FROM user_sensor WHERE app_user = ?",
    );
  (user_id, module Db: DB) => {
    let%lwt user_or_error = Db.collect_list(query, user_id);
    Caqti_lwt.or_fail(user_or_error);
  };
};

let details = {
  let query =
    R.find_opt(
      T.int,
      T.(tup2(T.string, T.string)),
      "SELECT username, name FROM app_user WHERE id = ?",
    );
  (user_id, module Db: DB) => {
    let%lwt details_or_error = Db.find_opt(query, user_id);
    Caqti_lwt.or_fail(details_or_error);
  };
};

let get_user_meta = (id, m) => {
  let%lwt details = details(id, m);
  let%lwt sensors = sensors(id, m);
  Lwt.return(
    Option.map(((username, name)) => {username, name, sensors}, details),
  );
};
