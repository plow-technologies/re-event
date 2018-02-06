type dom;

type event;

[@bs.val] external document : dom = "document";

[@bs.send] external createEventLegacy : (dom, string) => event = "createEvent";

[@bs.new] external js_createEvent : string => event = "Event";

[@bs.send] external js_addEventListener : (dom, string, event => unit) => unit =
  "addEventListener";

[@bs.send] external js_removeEventListener : (dom, string) => unit = "removeEventListener";

[@bs.send] external js_dispatchEvent : (dom, event) => unit = "dispatchEvent";

[@bs.set] external js_attachPayload : (event, Js.Json.t) => unit = "payload";

[@bs.get] external js_unsafe_getEventPayload : event => Js.null_undefined(Js.Json.t) = "payload";

let emit = (event: event) : unit => js_dispatchEvent(document, event);

let on = (eventName: string, callback: event => unit) =>
  js_addEventListener(document, eventName, callback);

let off = (eventName: string) => js_removeEventListener(document, eventName);

let createEvent = (eventName: string, payload: option(Js.Json.t)) => {
  let ev = js_createEvent(eventName);
  switch payload {
  | None => ev
  | Some(p) =>
    js_attachPayload(ev, p);
    ev
  }
};

let getPayload = (event: event, parseFunc: Js.Json.t => 'a) : option('a) => {
  let payload = js_unsafe_getEventPayload(event) |> Js.toOption;
  let result =
    switch payload {
    | None => None
    | Some(json) =>
      switch (parseFunc(json)) {
      | obj => Some(obj)
      | exception _ => None
      }
    };
  result
};