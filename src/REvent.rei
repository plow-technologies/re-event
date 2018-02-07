type event;

type listener = event => unit;

let emit: event => unit;

let on: (string, listener) => listener;

let off: (string, listener) => unit;

let createEvent: (string, option(Js.Json.t)) => event;

let getPayload: (event, Js.Json.t => 'a) => option('a);