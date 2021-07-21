module type DB = Caqti_lwt.CONNECTION;

[@deriving yojson]
type t = {
  id: int,
  name: string,
  description: string,
  api_key: string,
  step: int /* Number of seconds between readings. */
};

let create: (int, string, string, int, (module DB)) => Lwt.t(t);
/* Generate a new sensor record for a given user ID, sensor name,
   description, and step size. */

let get: (int, int, (module DB)) => Lwt.t(option(t));
/* `get user_id sensor_id` retrieves the metadata of a sensor
   with primary key sensor_id, provided that sensor is owned by the user
   identified by user_id. */

let delete: (int, int, (module DB)) => Lwt.t(unit);
/* `delete user_id sensor_id` deletes the sensor record with primary key
   sensor_id, provided that sensor is owned by the user identified by
   user_id. Because of foreign key relationships, this also deletes
   the time series data associated with the sensor.*/

let from_key: (string, (module DB)) => Lwt.t(list(t));
/* Recover the primary keys of all of the sensors accessible from
   the input API key. */
