type date = Ptime.date;
type time = Ptime.time;

let readings_per_day: int => int;
/* Report the number of readings per day, given an input reading step size, in seconds. */

let date_of_string_opt: string => option(Ptime.date);
let string_of_date: date => string;

let today: unit => date;

let yojson_of_date: date => [> | `String(string)];
let date_of_yojson: [> | `String(string)] => date;

let datetime_of_epoch_sec: int => option((date, time));

let bin: (time, int) => int;
/* Given a time record and step size, report the index of the bin containing the time.
      Examples:
         time = (1, 20, 15)  i.e. "01:20:15 AM UTC"
         step = 3600         i.e. "60 minute timestep"
         bin = 1             i.e. the second time bin for the day.
   */
