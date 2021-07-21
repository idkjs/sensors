module type DB = Caqti_lwt.CONNECTION;

[@deriving yojson]
type t = {
  username: string,
  name: string,
  sensors: list(int),
};

let get: (string, string, (module DB)) => Lwt.t(option(int));
/* Recover the ID associated with the input username/password pair, or
   None if the username/password combo is invalid.*/

let sensors: (int, (module DB)) => Lwt.t(list(int));
/* Recover a list of the sensor ids associated with this user. */

let get_user_meta: (int, (module DB)) => Lwt.t(option(t));
/* Recover the metadata associated with a specific user. */
