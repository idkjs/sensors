module type DB = Caqti_lwt.CONNECTION;

[@deriving yojson]
type readings = array(option(float));

[@deriving yojson]
type t = {
  sensor_id: int,
  occurred: Ptime.date,
  readings,
};

let empty: (Sensor.t, Ptime.date) => t;
/* Create an empty set of readings for the input sensor / date combination. */

let insert: (t, (module DB)) => Lwt.t(unit);
/* Record a full set of daily measurments for the input sensor / date combination.
   If a record is already present with the same date, it will be overwritten. */

let read_range:
  (int, int, Ptime.date, Ptime.date, (module DB)) => Lwt.t(list(t));
/* Retrieve all of the readings for a given sensor between two dates. */

let read_day: (int, Ptime.date, (module DB)) => Lwt.t(option(t));
/* Retrieve the readings associated with a given sensor / date
   combination (without validating user ownership). */

let overwrite: (t, readings) => t;
/* Form a new record by replacing existing measurements with ones in
   the input array, provided they are not null.  */
