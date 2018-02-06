type event;

let emit: event => unit;

let on: (string, event => unit) => unit;

let off: string => unit;

let createEvent: (string, option(Js.Json.t)) => event;

let getPayload: (event, Js.Json.t => 'a) => option('a);