module type DB = Caqti_lwt.CONNECTION;

type t = {
  id: int,
  uuid: string,
};
/* Note: Yojson isn't used here because an API key is shared with the
   frontend as part of the sensor JSON record.*/

let create: (unit, (module DB)) => Lwt.t(t);
/* Generate a new API key. */

let touch: (string, (module DB)) => Lwt.t(unit);
/* Update the last_used timestamp for this key. */
